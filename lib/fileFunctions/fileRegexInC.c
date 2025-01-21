#include "symLink_96.sl"
#include "symLink_95.sl"
#include <limits.h>
#include <sys/stat.h>

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_FILEREGEX = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function gets the file name (including extension) from a file path
char* getFileName(char* filePathIn){
	// Vars
	char* FF = (char*) __func__;
	char* fileName;

	// Data Validation
	if (filePathIn == NULL){ 
		myPerror(F_FILEREGEX, FF, "Invalid parameter 1 (file path); the value is null. Returning null");
		return NULL;
	}
	if (strlen(filePathIn) == 0){
		myPerror(F_FILEREGEX, FF, "Invalid parameter 1 (file path) is empty. Returning null");
		return NULL;
	}
	
	//regex: remove everything before the last '/'
	//two backslashes are required. 1st is to let the c program know the next backslash is a literal and the 2nd is to let the sed program in the shell know the '/' is a literal
	fileName = sedC("sed 's/.*\\///g'", filePathIn);
	
	if (fileName == NULL){ myPerror(F_FILEREGEX, FF, "sedC() returned a null value");}
	return fileName;
}

// This function gets the file path without the basename. If the basename includes "/", this function will return a faulty result
char* getFilePathNoName(char* filePathIn){
	// Vars
	char* FF = (char*) __func__;
	char* pathNoName;
	int pathLen;
	int numSlashes = 0;
	int currNumSlashes = 0;

	// Data Validation
	if (filePathIn == NULL){myPerror(F_FILEREGEX, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (strlen(filePathIn) == 0){ myPerror(F_FILEREGEX, FF, "Invalid 1st parameter; string is empty. Returning null"); return NULL;}

	// Getting Values
	pathLen = strlen(filePathIn);
	pathNoName = createStr(pathLen+1);

	// Counting how many '/' in name
	for (int i = 0; i < pathLen; i++){ if (filePathIn[i] == '/'){ numSlashes++;}}

	for (int i = 0; i < pathLen; i++){
		if (filePathIn[i] == '/'){ currNumSlashes++;}

		if (currNumSlashes == numSlashes){ pathNoName[i] = '/'; pathNoName[i+1] = '\0'; return pathNoName;}
		else{ pathNoName[i] = filePathIn[i];}
	}

	return pathNoName;
}

// This function gets file extension from a file name or file path
//surpress errors values: 0 - surpress no errors, 1 - surpress all errors, 2 - surpress all warnings
char* getFileExt(char* filePathIn, int surpressErrors){
	// Vars
	char* FF = (char*) __func__;
	char* fileName;
	int hasPeriod = 0;
	char* returnStr;

	// Data Validation
	if (filePathIn == NULL){ 
		myPerror(F_FILEREGEX, FF, "Invalid parameter 1 (file path); the value is null. Returning null");
		return NULL;
	}
	if (strlen(filePathIn) == 0){
		myPerror(F_FILEREGEX, FF, "Invalid parameter 1 (file path) is empty. Returning null");
		return NULL;
	}

	fileName = getFileName(filePathIn);

	// Data Validation-------------------------------------------------------------------------
	if (fileName == NULL){ 
		myPerror(F_FILEREGEX, FF, "getFileName() returned a null value. Returning null");
		return NULL;
	}
	for (int i = 0; filePathIn[i] != '\0'; i++){ if (filePathIn[i] == '.'){ hasPeriod = 1;}	} //checking if the file name includes a period
	if (hasPeriod == 0){
		if (surpressErrors != 2){
			myPerror(F_FILEREGEX, FF, "WARNING: file name doesn't have an extension. Returning blank string");
		}
		char* blankStr = createStr();
		free(fileName);
		return blankStr;
	}
	//-----------------------------------------------------------------------------------------
	
	//regex: removes everything after (and including) the period
	returnStr = sedC("sed 's/.*\\.//g'", fileName);
	free(fileName);
	return returnStr;
}

// This function determines the data type of a system object
char systemObjType(char* filePathIn){
	// Vars
	char* FF = (char*) __func__;
	struct stat pathStat;
	int statReturnVal;
	mode_t mode;

	// Data Validation-------------------------------------------------------------------------
	if (filePathIn == NULL){
		myPerror(F_FILEREGEX, FF, "Invalid parameter; value is null. Returning empty char character");
		return '\0';
	}
	if (strlen(filePathIn) == 0){
		myPerror(F_FILEREGEX, FF, "Invalid parameter; file path is empty. Returning empty char character");
		return '\0';
	}
	//-----------------------------------------------------------------------------------------

	// Getting Values
	statReturnVal = stat(filePathIn, &pathStat);
	if (statReturnVal != 0){
		myPerror(F_FILEREGEX, FF, "stat() function from sys/stat.h library returned an error value (!=0). Returning empty char character");
		return '\0';
	}
	
	mode = pathStat.st_mode; //getting mode (file type) of file

	// Converting mode to char, where the char is what the 1st char of the "ls -la $filePathIn" command would be
	if (S_ISREG(mode)){ return '-';}
	else if (S_ISDIR(mode)){ return 'd';}
	else if (S_ISCHR(mode)){ return 'c';}
	else if (S_ISBLK(mode)){ return 'b';}
	else if (S_ISFIFO(mode)){ return 'p';}
	else if (S_ISLNK(mode)){ return 'l';}
	else if (S_ISSOCK(mode)){ return 's';}
	else{ return '\0';}
}

// This function determines if a file path (parameter 2) is in a directory (parameter 1)
int isInDir(char* dirIn, char* filePathIn){
	// Vars
	char* FF = (char*) __func__;
	int dirLen;
	char* filePathToCompare;
	
	// Data Validation-------------------------------------------------------------------------
	if (dirIn == NULL){
		myPerror(F_FILEREGEX, FF, "Invalid 1st parameter (directory); value is null. Returning 0");
		return 0;
	}
	if (filePathIn == NULL){
		myPerror(F_FILEREGEX, FF, "Invalid 2nd parameter (file path); value is null. Returning 0");
		return 0;
	}
	if (strlen(dirIn) == 0){
		myPerror(F_FILEREGEX, FF, "Invalid 1st parameter (directory); value is empty string. Returning 0");
		return 0;
	}
	if (strlen(filePathIn) == 0){
		myPerror(F_FILEREGEX, FF, "Invalid 2nd parameter (file path); value is empty string. Returning 0");
		return 0;
	}
	//-----------------------------------------------------------------------------------------
	
	// Getting Values
	dirLen = strlen(dirIn);
	filePathToCompare = createStr(dirLen+1);

	// Getting the path of filePathIn at the length of the dirIn to compare the two values
	for (int i = 0; i < dirLen; i++){ filePathToCompare[i] = dirIn[i];}
	filePathToCompare[dirLen] = '\0';

	if (strcmp(dirIn, filePathToCompare) == 0){ free(filePathToCompare); return 1;}
	else{ free(filePathToCompare); return 0;}

}
