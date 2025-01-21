#include "customOpenHelpers.h"
#include "updateFAndF.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"
#include "dbValidationSpecial.h"
#include "fileInfoOwSysCalls.h"
#include "sourceIsInCsv.h"
#include "customOpen.h"

// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* csvAsStr;
extern char* myScriptsDirLoc;
extern int (*orig_open)(const char*, int, ...);

// Error Msg Constant Vars
char* F_CUSTOMOPENHELPERS = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function checks if file is a valid name
int isValidFileName(char* pathIn){
	// Vars
	char* FF = (char*) __func__;
	char* fileName;
	int returnVal;
	//regex: 0 or 1 leading '.', any (upper case or lower case letter or number or '_' or '-') 1 or more times, 0 or 1 (0 or 1 period, any (upper case or lower case letter or number) 1 or more times
	char fileNameRegex[128] = "^\\.\\?[a-zA-Z0-9_\\-]\\+\\(\\.\\?[a-zA-Z0-9]\\+\\)\\?$";

	// Data Validation
	if (pathIn == NULL){ myPerror(F_CUSTOMOPENHELPERS, FF, "Invalid 1st parameter; value is null. Returning 0"); return 0;}

       	// Getting Values
	fileName = getFileName(pathIn);
	if (fileName == NULL){ myPerror(F_CUSTOMOPENHELPERS, FF, "getFileName returned a null value. Returning 0"); return 0;}

	// Checking file name constraints
	//length restriction. I might have set the limit for file name length to be 128 somewhere in my code by hardcoding path length instead of alloc'ing it
	if (strlen(fileName) > 128){
		myPerror(F_CUSTOMOPENHELPERS, FF, "Invalid file name; file length is too long (>128). Returning 0");
		returnVal = 0;
	}
	else if (strlen(fileName) == 0){
		myPerror(F_CUSTOMOPENHELPERS, FF, "Invalid file name; file length is too short (0 characters long). Returning 0");
		returnVal = 0;
	}
	else if (regexMatch(fileNameRegex, pathIn) != 1){ //checking regex
		myPerror(F_CUSTOMOPENHELPERS, FF, "Invalid file name; file doesn't match accepted pattern");
		myPerror(F_CUSTOMOPENHELPERS, FF, "	Accepted pattern includes any letter (case insensitive) or number with only special characters '.', '-', and '_' allowed");
		returnVal = 0;
	}
	else{ returnVal = 1;}

	free(fileName);
	return returnVal;
}

// This function adds a newly created file to files and folders csv
int addFileToCsvNewCFuncs(char* pathIn){
	// Vars
	char* FF = (char*) __func__;
	struct filesAndFoldersDbObj** db;
	char* updatedCsv;
	int writeReturnVal;

	// Data Validation
	if (pathIn == NULL){ myPerror(F_CUSTOMOPENHELPERS, FF, "Invalid 1st parameter; value is null. Returning 0"); return 0;}

	// Getting Values
	db = (struct filesAndFoldersDbObj**) convert(csvAsStr, "filesDb");
	if (db == NULL){
		myPerror(F_CUSTOMOPENHELPERS, FF, "convert() function (to filesDb object) returned a null value. Returning -1");
		return -1;
	}

	// Adding pathIn to db struct
	db = addFileToFilesDbArr(db, pathIn, filesAndFoldersCsvLoc);
	if (db == NULL){ myPerror(F_CUSTOMOPENHELPERS, FF, "addFileToFilesDbArr() function returned a null value. Returning -1"); return -1;}

	// Converting db to string
	updatedCsv = convert(db, "string");
	if (db == NULL){
		myPerror(F_CUSTOMOPENHELPERS, FF, "convert() function (filesDb object to string) returned a null value. Returning -1");
		myFree(db);
		return -1;
	}

	// Write to file
	writeReturnVal = writeToFile(filesAndFoldersCsvLoc, updatedCsv);
	if (writeReturnVal != 0){
		myPerror(F_CUSTOMOPENHELPERS, FF, "writeToFile() function returned an error value (!=0). Returning -1");
		myFree(db); free(updatedCsv);
		return -1;
	}

	// Writing the original file (csvAsStr) to backup csv location
	writeReturnVal = writeToFile(filesAndFoldersCsvBakLoc, csvAsStr);
	if (writeReturnVal != 0){
		myPerror(F_CUSTOMOPENHELPERS, FF, "writeToFile() function for backup csv location returned an error value (!=0). Returning -1");
		myFree(db); free(updatedCsv);
		return -1;
	}

	myFree(db);
	myFree(updatedCsv);
	return 0;
}

// This function overwrites c standard library/syscall open() function
int open(const char* pathname, int flags, ...){
	// Vars
	char* FF = (char*) __func__;
	char absolutePath[PATH_MAX];
	int returnInt;
	int setupVal;

	// Get absolute path, if it exists. If not, open file normally
	//I had to open file normally if realpath didn't exist cuz vim wouldn't work otherwise
	if (realpath(pathname, absolutePath) == NULL){ return orig_open(pathname, flags);}

	// Getting global var values
	setupVal = constVarsNewCFuncsSetup();
	if (setupVal != 0){
		myPerror(F_CUSTOMOPENHELPERS, FF, "constVarsNewCFuncsSetup() function returned an error value (!=0) for ", pathname, ". Returning -1");
		unsetPerror(FF);
		free(csvAsStr);
		return -1;
	}

	// Checking if creating a file and if that file is in myScripts dir
	if ((flags & O_CREAT) && isInDir(myScriptsDirLoc, (char*) pathname) == 1){
		if (isValidFileName((char*) pathname) != 1){ //if an invalid name
			myPerror(F_CUSTOMOPENHELPERS, FF, "Invalid file name. Returning -1");
			unsetPerror(FF);
			free(csvAsStr);
			return -1;
		}

		if (fileExists((char*) pathname, 0) != 1){ //if file doesn't already exist
			int addFileReturnVal = addFileToCsvNewCFuncs((char*) pathname);
			if (addFileReturnVal != 0){ //if unable to add file to csv
				myPerror(F_CUSTOMOPENHELPERS, FF, "addFileToCsvNewCFuncs() function returned an error value (!=0). Returning -1");
				unsetPerror(FF);
				free(csvAsStr);
				return -1;
			}
		}
	}

	// Opening file
	returnInt = customOpen(absolutePath, flags);

	free(csvAsStr);
	return returnInt;
}
