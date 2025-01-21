#include "updateFAndFForNewPathAndOldPath.h"
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

// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* csvAsStr;
extern char* symLinkBasePath;

// Error Msg Constant Vars
char* F_UPDATEDBARR = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function updates files paths in the db struct and writes the updated db to filesAndFolders.csv. Also updates backup
int updateDbArrNewCFuncs(const char* oldFilePathIn, const char* newFilePathIn){
	// Vars
	char* FF = (char*) __func__;
	struct filesAndFoldersDbObj** db;
	char* dbAsStr;

	// Getting Values
	db = (struct filesAndFoldersDbObj**) convert(csvAsStr, "filesDb");
	if (db == NULL){
		myPerror(F_UPDATEDBARR, FF, "convert() function returned a null value error when converting to filesDb. Returning -1");
		free(csvAsStr);
		return -1;
	}

	// Iterating over db until find a path matching oldFilePath
	for (int i = 0; db[i]->id != -1; i++){
		if (strcmp(db[i]->fullPath, oldFilePathIn) == 0){

			// Updating old path
			free(db[i]->oldPath);
			db[i]->oldPath = createStr(strlen(oldFilePathIn)+1);
			strcpy(db[i]->oldPath, (char*) oldFilePathIn);

			// Updating new path
			free(db[i]->fullPath);
			db[i]->fullPath = createStr(strlen(newFilePathIn)+1);
			strcpy(db[i]->fullPath, (char*) newFilePathIn);

			// Updating moved
			db[i]->movedOrChangedName = 1;

			// Updating file name
			char* fileName = getFileName((char*) newFilePathIn);
			if (fileName == NULL){
				myPerror(F_UPDATEDBARR, FF, "getFileExt() function returned a  null value. Returning -1");
				myFree(db);
				return -1;
			}
			free(db[i]->fileName);
			db[i]->fileName = fileName;

			// Updating extension
			char* ext = getFileExt(fileName, 2);
			if (ext == NULL){
				myPerror(F_UPDATEDBARR, FF, "getFileExt() function returned a  null value. Returning -1");
				myFree(db);
				return -1;
			}
			free(db[i]->extension);
			db[i]->extension = ext;

			//update symlink if needed
			if (db[i]->symLinkNum != 0){
				// Converting symlink num to string for use in command
				char* symLinkNumAsStr = convert(db[i]->symLinkNum, "string");
				if (symLinkNumAsStr == NULL){
					myPerror(F_UPDATEDBARR, FF, "convert() function returned a  null value when converting to int. Returning -1");
					myFree(db);
					return -1;
				}

				// Creating command. Updating symlink using "ln -sf" command
				char* command = createStr(strlen(newFilePathIn)+strlen(symLinkBasePath)+40);
				strcat(command, "ln -sf ");
				strcat(command, (char*) newFilePathIn);
				strcat(command, " ");
				strcat(command, symLinkBasePath);
				strcat(command, symLinkNumAsStr);
				strcat(command, ".sl");

				// Running command
				char* output = runCommand(command);
				free(command);
				free(symLinkNumAsStr);

				// Checking if output is an error by either null value or non-empty string
				//ln -sf shouldn't write anything to stdout or stderr unless an error
				if (output == NULL){
					myPerror(F_UPDATEDBARR, FF, "runCommand() function returned a null value. Returning -1");
					myFree(db);
					return -1;
				}
				if (strlen(output) != 0){
					myPerror(F_UPDATEDBARR, FF, "Error updating symlink. Shell output:\n ", output, "\nReturning -1");
					myFree(db); free(output);
					return -1;
				}
				free(output);

			} //end of symlink conditional
		} //end of path comparison

	} //end of for loop

	// Convert db to string
	dbAsStr = convert(db, "string");
	if (dbAsStr == NULL){
		myPerror(F_UPDATEDBARR, FF, "convert() function returned a null value. Returning -1");
		myFree(db);
		return -1;
	}

	// Write to files and folders backup
	int writeReturnValBackup = writeToFile(filesAndFoldersCsvBakLoc, csvAsStr);
	if (writeReturnValBackup != 0){
		myPerror(F_UPDATEDBARR, FF, "writeToFile() function for backup returned an error value (!=0). Nothing written to backup or main file. Returning -1");
		myFree(db); free(csvAsStr);
		return -1;
	}

	// Write to files and folders main
	int writeReturnVal = writeToFile(filesAndFoldersCsvLoc, dbAsStr);
	if (writeReturnVal != 0){
		myPerror(F_UPDATEDBARR, FF, "writeToFile() function returned an error value (!=0). Nothing written to main file, but Backup file was updated. Returning -1");
		myFree(db); free(csvAsStr);
		return -1;
	}

	myFree(db);
	free(dbAsStr);
	return 0;
}
