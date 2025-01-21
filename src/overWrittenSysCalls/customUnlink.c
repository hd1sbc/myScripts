#include "customUnlink.h"
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
extern int (*orig_unlink)(const char*);
extern int (*orig_rmdir)(const char*);

// Error Msg Constant Vars
char* F_CUSTOMUNLINK = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function sets the inUse value in the filesAndFoldersCsv to 0 for the given path
int setInUseToZero(const char* pathIn){
	// Vars
	char* FF = (char*) __func__;
	struct filesAndFoldersDbObj** db;
	char* dbAsStr;

	// Data Validation
	if (pathIn == NULL){ myPerror(F_CUSTOMUNLINK, FF, "Invalid 1st argument; value is null. Returning -1"); return -1;}

	// Getting Values
	db = (struct filesAndFoldersDbObj**) convert(csvAsStr, "filesDb");
	if (db == NULL){
		myPerror(F_CUSTOMUNLINK, FF, "convert() function returned a null value error when converting to filesDb. Returning -1");
		free(csvAsStr);
		return -1;
	}

	// Iterating over db until find math with pathIn. Updating the struct's inUse value
	for (int i = 0; db[i]->id != -1; i++){
		if (strcmp(db[i]->fullPath, pathIn) == 0){ db[i]->inUse = 0;}
	}

	// Converting database back to string
	dbAsStr = convert(db, "string");
	if (dbAsStr == NULL){
		myPerror(F_CUSTOMUNLINK, FF, "convert() function returned a null value. Returning -1");
		myFree(db);
		return -1;
	}

	// Writing to filesAndFolders.csv backup
	int writeReturnValBackup = writeToFile(filesAndFoldersCsvBakLoc, csvAsStr);
	if (writeReturnValBackup != 0){
		myPerror(F_CUSTOMUNLINK, FF, "writeToFile() function for backup returned an error value (!=0). Nothing written to backup or main file. Returning -1");
		myFree(db); free(csvAsStr);
		return -1;
	}

	// Writing to filesAndFolders.csv main
	int writeReturnVal = writeToFile(filesAndFoldersCsvLoc, dbAsStr);
	if (writeReturnVal != 0){
		myPerror(F_CUSTOMUNLINK, FF, "writeToFile() function returned an error value (!=0). Nothing written to main file, but Backup file was updated. Returning -1");
		myFree(db); free(csvAsStr);
		return -1;
	}

	myFree(db);
	free(dbAsStr);
	return 0;
}

// This function overwrites c standard library/syscall rename() function
int unlink(const char* pathIn){
	// Vars
	char* FF = (char*) __func__;
	int setupVal;
	int isInCsv;

	// Data Validation
	if (pathIn == NULL){ myPerror(F_CUSTOMUNLINK, FF, "Invalid 1st parameter; value is null. Returning -1"); unsetPerror(FF); return -1;}
	if (fileExists((char*) pathIn, 0) != 1){myPerror(F_CUSTOMUNLINK, FF, (char*) pathIn, " doesn't exist. Returning -1"); return -1;}

	// Getting Values--------------------------------------------------------------------------
	setupVal = constVarsNewCFuncsSetup();
	if (setupVal != 0){
		myPerror(F_CUSTOMUNLINK, FF, "constVarsNewCFuncsSetup() function returned an error value (!=0). Returning -1");
		unsetPerror(FF);
		free(csvAsStr);
		return -1;
	}

	isInCsv = isTargetPathInCsv((char*) pathIn);
	if (isInCsv < 0){
		myPerror(F_CUSTOMUNLINK, FF, "isTargetPathInCsv() returned an error value (<0). Returning -1");
		free(csvAsStr);
		return -1;
	}
	//-----------------------------------------------------------------------------------------

	// If path is in csv, take necessary actions
	if (isInCsv > 0){
		// Get perms
		int perms = getPerms(pathIn, isInCsv, 0);
		if (perms < 0){
			myPerror(F_CUSTOMUNLINK, FF, "getPerms() returned an error value (<0). Returning -1");
			free(csvAsStr);
			return -1;
		}

		// If protected file
		if (perms != 1 && perms != 3 && perms != 5 && perms != 7){
			// Warn user
			int unlinkFileBool = openProtectedFileWarning("-d", (char*) pathIn);
			if (unlinkFileBool != 1){ free(csvAsStr); return -1;}
		}

		// If user wants to delete, update csv
		int setInUseReturnVal = setInUseToZero(pathIn);
		if (setInUseReturnVal != 0){
			myPerror(F_CUSTOMUNLINK, FF, "setInUseToZero() returned an error value (!=0). Returning -1");
			free(csvAsStr);
			return -1;
		}
	}

	// If a directory, remove it. This assumes the dir is empty
	if (systemObjType((char*) pathIn) == 'd'){
		int origRmdirReturnVal = orig_rmdir(pathIn);
		if (origRmdirReturnVal < 0){
			myPerror(F_CUSTOMUNLINK, FF, "orig_rmdir() function returned an error value (<0). Returning -1");
			free(csvAsStr);
			return -1;
		}

		free(csvAsStr);
		return origRmdirReturnVal;
	}

	// If not a directory, remove file normally
	int unlinkReturnVal = orig_unlink(pathIn);
	if (unlinkReturnVal < 0){
		myPerror(F_CUSTOMUNLINK, FF, "unlinkReturnVal() returned an error value (<0). Returning -1");
		unsetPerror(FF);
		return -1;
	}

	free(csvAsStr);
	return unlinkReturnVal;
}
