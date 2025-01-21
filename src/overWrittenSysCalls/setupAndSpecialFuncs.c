#include "setupAndSpecialFuncs.h"
#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"

// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* F_NEWCFUNCS;
extern char* csvAsStr;
extern int (*orig_open)(const char*, int, ...);

// Error Msg Constant Vars
char* F_OWSYSCALLSSETUP = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function is just a condensed way of printing a specific error message about unsetting LD_PRELOAD
void unsetPerror(char* FF){
	// Maintaining generic NEWCFUNCS var for ease of use
	myPerror(F_NEWCFUNCS, FF, "If this prevents necessary action, you can block this behavior by running the command: unset LD_PRELOAD");
}

// This function reads a file and converts its contents to a string
//	Uses orig_open() instead of open() or fopen() to bypass overwritten open() function
char* readFileSpecial(const char* filePathIn){
	// Vars
	char* FF = (char*) __func__;
	int origOpenReturnVal;
	char* returnStr = createStr(); 
	char buffer[128];
	int bytesRead;

	// Data Validation
	if (filePathIn == NULL){ myPerror(F_OWSYSCALLSSETUP, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}

	// Open file using original open function to bypass an infinite loop of the modified open() function
	// In "convert.c", I use fopen(), which returns a file type. This makes it easier to use, especially when reading it.
	// 	However, fopen() uses open() function call, creating infinite loop, so I have to use the original open func
	origOpenReturnVal = orig_open((const char*) filePathIn, O_RDONLY);
	if (origOpenReturnVal == -1) {
		myPerror(F_OWSYSCALLSSETUP, FF, "Original open function (orig_open()) returned an error result for ", filePathIn, ". Returning null");
		free(returnStr);
		return NULL;
	}

	// Read from the file in 128 byte chunks. Subtracting 1 to make room for end-of-string char
	// Can't use fgetc() like in "convert.h" cuz not reading from FILE* datatype
	while ((bytesRead = read(origOpenReturnVal, buffer, sizeof(buffer) - 1)) > 0) {
		if (strlen(returnStr) > 0){ returnStr = myRealloc(bytesRead+1, returnStr);}
		if (returnStr == NULL){	myPerror(F_OWSYSCALLSSETUP, FF, "myRealloc returned a null value. Returning null"); return NULL;} //don't need to free anything
		buffer[bytesRead] = '\0'; //adding end-of-string char
		strcat(returnStr, buffer); //appending 128 byte buffer to return string
	}

	// Close the file
	if (close(origOpenReturnVal) == -1) {
		myPerror(F_OWSYSCALLSSETUP, FF, "Error closing file", (char*) filePathIn,". Returning null");
		free(returnStr);
		return NULL;
	}

	return returnStr;

}

// This function gets global var values anytime open() or write() functions are called
int constVarsNewCFuncsSetup(){
	// Vars
	char* FF = (char*) __func__;
	
	// Get files and folders contents
	csvAsStr = readFileSpecial(filesAndFoldersCsvLoc);
	if (csvAsStr == NULL){
		myPerror(F_OWSYSCALLSSETUP, FF, "readFileSpecial() returned a null value. Returning -1");
		return -1;
	}
	
	numRowsInCsv = getNumRows(csvAsStr);
	if (numRowsInCsv < 0){
		myPerror(F_OWSYSCALLSSETUP, FF, "getNumRows() returned an error value (<0). Returning -1");
		return -1;
	}

	numColsInCsv = getNumCols(csvAsStr);
	if (numColsInCsv < 0){
		myPerror(F_OWSYSCALLSSETUP, FF, "getNumCols() returned an error value (<0). Returning -1");
		return -1;
	}

	return 0;
}

