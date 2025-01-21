#include "customRename.h"
#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"
#include "dbValidationSpecial.h"
#include "fileInfoOwSysCalls.h"
#include "sourceIsInCsv.h"
#include "customOpen.h"
#include "customOpenHelpers.h"
#include "customWrite.h"
#include "updateFAndFForNewPathAndOldPath.h"

// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* csvAsStr;
extern int (*orig_rename)(const char*, const char*);

// Error Msg Constant Vars
char* F_CUSTOMRENAME = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function overwrites c standard library/syscall rename() function
int rename(const char* oldFilePathIn, const char* newFilePathIn){
	// Vars
	char* FF = (char*) __func__;
	int isInCsv;
	int setupVal;
	char* fileName;

	// Data Validation-------------------------------------------------------------------------
	if (oldFilePathIn == NULL){
		myPerror(F_CUSTOMRENAME, FF, "Invalid 1st parameter; value is null. Returning -1");
		unsetPerror(FF);
		return -1;
	}
	if (newFilePathIn == NULL){
		myPerror(F_CUSTOMRENAME, FF, "Invalid 2nd parameter; value is null. Returning -1");
		unsetPerror(FF);
		return -1;
	}
	if (fileExists((char*) oldFilePathIn, 0) != 1){
		myPerror(F_CUSTOMRENAME, FF, (char*) oldFilePathIn, " doesn't exist. Returning -1");
		unsetPerror(FF);
		return -1;
	}
	if (strlen((char*) newFilePathIn) == 0){
		myPerror(F_CUSTOMRENAME, FF, "Invalid 2nd parameter; string is empty. Returning -1");
		unsetPerror(FF);
		return -1;
	}
	if (fileExists((char*) newFilePathIn, 0) == 1){
		int openFileBool = openProtectedFileWarning("-ro", (char*) oldFilePathIn);
		if (openFileBool != 1){ return -1;}
	}
	//-----------------------------------------------------------------------------------------

	// Getting Values
	setupVal = constVarsNewCFuncsSetup();
	if (setupVal != 0){
		myPerror(F_CUSTOMRENAME, FF, "constVarsNewCFuncsSetup() function returned an error value (!=0). Returning -1");
		unsetPerror(FF);
		free(csvAsStr);
		return -1;
	}
	
	isInCsv = isTargetPathInCsv(oldFilePathIn);
	if (isInCsv < 0){
		myPerror(F_CUSTOMRENAME, FF, "isTargetPathInCsv() returned an error value (<0). Returning -1");
		free(csvAsStr);
		return -1;
	}

	fileName = getFileName((char*) newFilePathIn);
	if (fileName == NULL){
		myPerror(F_CUSTOMRENAME, FF, "getFileName() function returned a  null value. Returning -1");
		free(csvAsStr);
		return -1;
	}

	// Actions needed if file is in csv
	if (isInCsv > 0){
		// Check if new file name is valid
		if (isValidFileName(fileName) != 1){
			myPerror(F_CUSTOMRENAME, FF, "Invalid new file name. Returning -1");
			unsetPerror(FF);
			free(csvAsStr); free(fileName);
			return -1;
		}

		// Get file path. Used later to see if renaming file or moving to different dir
		char* filePathNoName = getFilePathNoName((char*) oldFilePathIn);
		if (filePathNoName == NULL){
			myPerror(F_CUSTOMRENAME, FF, "getFilePathNoName() function returned a null value. Returning -1");
			free(csvAsStr); free(fileName);
			return -1;
		}
		
		// Getting perms
		int perms = getPerms(oldFilePathIn, isInCsv, 0);
		if (perms < 0){
			myPerror(F_CUSTOMRENAME, FF, "getPerms() returned an error value (<0). Returning -1");
			free(csvAsStr); free(filePathNoName); free(fileName);
			return -1;
		}

		// Determining if renaming or moving file to different dir
		//if not moving to different dir (i.e. renaming)
		if (isInDir((char*) filePathNoName, (char*) newFilePathIn) == 1){
			//check rename permissions
			if (perms != 4 && perms != 5 && perms != 6 && perms != 7){
				// Warn user
				int openFileBool = openProtectedFileWarning("-r", (char*) oldFilePathIn);
				if (openFileBool != 1){ free(csvAsStr); free(filePathNoName); free(fileName); return -1;}
			}
		}
		//if moving to different dir
		else{
			//check move permissions
			if (perms != 2 && perms != 3 && perms != 6 && perms != 7){
				// Warn user
				int openFileBool = openProtectedFileWarning("-m", (char*) oldFilePathIn);
				if (openFileBool != 1){ free(csvAsStr); free(filePathNoName); free(fileName); return -1;}
			}

		}

		free(filePathNoName);
		free(fileName);
		
		// Update csv
		int updateDbArrReturnVal = updateDbArrNewCFuncs(oldFilePathIn, newFilePathIn);
		if (updateDbArrReturnVal != 0){
			myPerror(F_CUSTOMRENAME, FF, "updateDbArrNewCFuncs() returned an error value (<0). Returning -1");
			unsetPerror(FF);
			free(csvAsStr);
			return -1;
		}
	}


	free(csvAsStr);
	
	// Rename file
	int origRenameReturnVal = orig_rename(oldFilePathIn, newFilePathIn);
	if (origRenameReturnVal < 0){
		myPerror(F_CUSTOMRENAME, FF, "orig_rename() returned an error value (<0). Returning -1");
		unsetPerror(FF);
		return -1;
	}

	return origRenameReturnVal;
}
