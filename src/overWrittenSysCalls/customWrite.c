#include "customWrite.h"
#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"
#include "dbValidationSpecial.h"
#include "fileInfoOwSysCalls.h"
#include "sourceIsInCsv.h"
#include "customOpen.h"
#include "customOpenHelpers.h"

// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* csvAsStr;
extern ssize_t (*orig_write)(int, const void*, size_t);

// Error Msg Constant Vars
char* F_CUSTOMWRITE = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function gets the absolute file path from the file descriptor
char* getFilePathFromFd(int fdIn){
	// Vars
	char* FF = (char*) __func__;
	char fdPath[PATH_MAX];
	char* returnPath = malloc(sizeof(char)*PATH_MAX);
	int absPathLen;

	// Getting path of current file descriptor
	snprintf(fdPath, sizeof(fdPath), "/proc/self/fd/%d", fdIn);

	// Reading absolute path of file in current file descriptor path (in proc folder) and storing it in returnPath
	absPathLen = readlink(fdPath, returnPath, PATH_MAX-1);
	if (absPathLen < 0){
		myPerror(F_CUSTOMWRITE, FF, "readlink() (a standard c lib) returned an error value (<0). Returning null");
		free(returnPath);
		return NULL;
	}

	returnPath[absPathLen] = '\0'; //adding end-of-string char to returnPath

	return returnPath;
}

// This function overwrites c standard library/syscall write() function
//not using int cuz overwriting write() requires matching function signatures
ssize_t write(int fd, const void* buf, size_t bufLen){
	// Vars
	char* FF = (char*) __func__;
	char* filePath;
	int setupVal;
	int isTargetPath;
	int origWriteReturnVal = 0;

	// Getting Values--------------------------------------------------------------------------
	filePath = getFilePathFromFd(fd);
	if (filePath == NULL){
		myPerror(F_CUSTOMWRITE, FF, "getFilePathFromFd() function returned a null value. Returning -1");
		unsetPerror(FF);
		free(csvAsStr);
		return -1;
	}

	setupVal = constVarsNewCFuncsSetup();
	if (setupVal != 0){
		myPerror(F_CUSTOMWRITE, FF, "constVarsNewCFuncsSetup() function returned an error value (!=0). Returning -1");
		unsetPerror(FF);
		free(filePath); free(csvAsStr);
		return -1;
	}

	isTargetPath = isTargetPathInCsv((char*) filePath);
	if (isTargetPath < 0){
		myPerror(F_CUSTOMWRITE, FF, "isTargetPathInCsv() function returned an error value (-1). Returning -1");
		unsetPerror(FF);
		free(filePath); free(csvAsStr);
		return -1;
	}
	//-----------------------------------------------------------------------------------------

	// If path is in csv
	if (isTargetPath > 0){
		char* ext = getFileExt((char*) filePath, 2);
		if (ext == NULL){
			myPerror(F_CUSTOMWRITE, FF, "getFileExt() function returned a null value. Returning -1");
			unsetPerror(FF); free(filePath); free(csvAsStr);
			return -1;
		}

		// If extension is a csv (denoting database)
		if (strcmp(ext, "csv") == 0){
			int isValidDbNewCFuncsBool = isValidDbNewCFuncs(filePath, (char*) buf);
			if (isValidDbNewCFuncsBool == 0){ //if not a valid DB, but not an error value
				myPerror(F_CUSTOMWRITE, FF, "The new file contents are an invalid database. It contains incorrect data types. Not writing anything to file. Returning -1");
				unsetPerror(FF);
				free(filePath); free(ext); free(csvAsStr);
				return -1;
			}
			else if (isValidDbNewCFuncsBool < 0){ //if error value
				myPerror(F_CUSTOMWRITE, FF, "isValidDbNewCFuncs() function returned an error value (<0). Not writing anything to file. Returning -1");
				unsetPerror(FF);
				free(filePath); free(ext); free(csvAsStr);
				return -1;
			}
		}
	}

	origWriteReturnVal = orig_write(fd, buf, bufLen);
	free(filePath); free(csvAsStr);
	return origWriteReturnVal;
}
