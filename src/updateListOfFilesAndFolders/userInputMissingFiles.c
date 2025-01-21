#include "userInputMissingFiles.h"
#include "updateFAndF.h"

// GLOBAL VARIABLES--------------------------------------------------------------------------------
// Error Message Constant Vars
char* F_UPDATEFANDFMISSINGFILES = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function takes user input and performs actions related to missing files
struct filesAndFoldersDbObj*** userInputFindMissingFiles(char* filesList, struct filesAndFoldersDbObj** dbIn){
	// Vars
	struct filesAndFoldersDbObj** missingFiles;
	struct filesAndFoldersDbObj*** returnArr = malloc(sizeof(struct filesAndFoldersDbObj**)*3); //creating a FandF return array that contains both the updated dbIn and missing files
	char* FF = (char*) __func__;

	// Data Validation
	if (filesList == NULL){
		myPerror(F_UPDATEFANDFMISSINGFILES, FF, "Invalid 1st parameter (files list); value is null. Returning null");
		errorBool = 1;
		return NULL;
	}
	if (dbIn == NULL){
		myPerror(F_UPDATEFANDFMISSINGFILES, FF, "Invalid 2nd parameter (database); value is null. Returning null");
		errorBool = 1;
		return NULL;
	}


	// MISSING FILES---------------------------------------------------------------------------
	puts("Seaching to see if files marked as in-use from the official list of files and folders are in their expected location");
	missingFiles = missingOfficialFiles(filesList);
	if (missingFiles == NULL && errorBool != 0){
		myPerror(F_UPDATEFANDFMISSINGFILES, FF, "missingOfficialFiles() function returned an error value (null and errorBool set to 1. Returning null");
		errorBool = 1;
		return NULL;
	}
	//-----------------------------------------------------------------------------------------
	
	// Options for checking whole system for missing files and marking files as not in use
	if (missingFiles != NULL){
		printf("Found %d%s\n", getArrSize(missingFiles), " missing files: ");
		for (int i = 0; missingFiles[i]->id != -1; i++){ printf("%s\n", missingFiles[i]->fullPath);} //print full path of each missing file
		

		// CHECKING WHOLE SYSTEM FOR MISSING FILES-----------------------------------------
		puts("Do you want to check the whole file system to see if these files exist in another directory? (Y/n)");
		char checkWholeSys;
		scanf("%c", &checkWholeSys);
		if (checkWholeSys == 'Y' || checkWholeSys == 'y'){
			//loading bar since it will take some time (~11 seconds) to search through the whole filysystem
			lbParams* l;
			l = loadingBarC(11, 1, "Checking whole filesystem. This might take a few seconds");

			missingOfficialFilesWholeSys(missingFiles);
			if (errorBool != 0){
				myPerror(F_UPDATEFANDFMISSINGFILES, FF, "missingOfficialFilesWholeSys() function returned an error. Returning null");
				errorBool = 1;
				return NULL;
			}

			closeLoadingBarC(l); //closing loading bar
			
			// Finding number of missing files (defined by having a non-empty string value in fileLocArr)
			int numMissingFilesFound = 0;
			for (int i = 0; fileLocArr[i] != NULL; i++){ if (strlen(fileLocArr[i]) != 0){ numMissingFilesFound++;}}
			// Printing missing file info
			if (numMissingFilesFound > 0){
				printf("%d%s\n", numMissingFilesFound, " missing files listed above were found. Their file paths have been updated. Their new paths are: ");
				for (int i = 0; fileLocArr[i] != NULL; i++){
					if (strlen(fileLocArr[i]) != 0){ printf("%s\n", fileLocArr[i]);}
				}
				// Updating database with new paths
				dbIn = updateDbWithNewPaths(dbIn);
				if (dbIn == NULL){
					myPerror(F_UPDATEFANDFMISSINGFILES, FF, "updateDbWithNewPaths() function returned a null value. Returning null");
					errorBool = 1;
					return NULL;
				}
			}
			else { puts("No missing files found");}
			
			//removing missing files from missingFiles array
			if (numMissingFilesFound > 0){ missingFiles = removeFoundFilesFromMissingFiles(missingFiles);}

			if (missingFiles == NULL){
				myPerror(F_UPDATEFANDFMISSINGFILES, FF, "removeFoundFilesFromMissingFiles() function returned a null value. Returning null");
				errorBool = 1;
				return NULL;
			}
			// Printing missing files
			if (missingFiles[0]->id != -1){ 
				printf("%s%d%s\n", "The remaining ", getArrSize(missingFiles), " missing files are:");
				for (int i = 0; i < getArrSize(missingFiles); i++){printf("%s\n", missingFiles[i]->fileName);}
			}
			else{ puts("All missing files found in a different location");}
		}
		else if (checkWholeSys == 'N' || checkWholeSys == 'n'){
			puts("WARNING: It is not recommended to skip the step above");
			puts("If you did so unintentionally, press 'n' or 'N' for all of the following prompts to exit gracefully");
		}
		else{
			puts("Invalid input. Not checking the whole system to see if files exist");
			puts("WARNING: It is not recommended to skip the step above");
			puts("If you did so unintentionally, press 'n' or 'N' for all of the following prompts to exit gracefully");
		}
		//-----------------------------------------------------------------------------------------

		// Marking missing files as not in use---------------------------------------------
		puts("Do you want to mark these files as not in use? (Y/n)");
		char markMissingFilesInput;
		scanf(" %c", &markMissingFilesInput);
		if (markMissingFilesInput == 'Y' || markMissingFilesInput == 'y'){
			dbIn = markFilesAsNotInUse(dbIn, missingFiles);
			if (dbIn == NULL){
				myPerror(F_UPDATEFANDFMISSINGFILES, FF, "markFilesAsNotInUse() function returned a null value. Returning null");
				errorBool = 1;
				return NULL;
			}
		}
		else if (markMissingFilesInput != 'N' && markMissingFilesInput != 'n'){ puts("Invalid input. Not marking these files as in use");}
		//-----------------------------------------------------------------------------------------
		
	}
	else{ puts("No missing files found");}


	returnArr[0] = dbIn;
	returnArr[1] = missingFiles;
	returnArr[2] = NULL;

	return returnArr;

}
