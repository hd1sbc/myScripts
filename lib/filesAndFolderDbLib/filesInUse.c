#include "symLink_77.sl"
#include "symLink_36.sl"
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// GLOBAL VARS-------------------------------------------------------------------------------------
char** fileLocArr = NULL;
int errorBool = 0; //some functions return null even if no errors occurred. This var helps differentiate errors nulls from non-error ones

char* myScriptsLoc;
char* backupsLoc;
char* filesAndFoldersLoc;
char* filesAndFoldersBackupLoc;
char* symLinksFolderLoc;

// Error Message Constant Vars
char* F_FANDFFILEUSE = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// INSTANTIATING GLOBAL VARS-----------------------------------------------------------------------
__attribute__((constructor)) void init_FANDFFILEUSE() {
	myScriptsLoc = myFindPathC(1); strcat(myScriptsLoc, "/"); //myScripts dir
	backupsLoc = myFindPathC(43); strcat(backupsLoc, "/"); //backups dir
	filesAndFoldersLoc = myFindPathC(79); //filesAndFolders.csv
	filesAndFoldersBackupLoc = myFindPathC(207); //filesAndFolders.csv.bak
	symLinksFolderLoc = myFindPathC(146); strcat(backupsLoc, "/"); //symLinks dir
}
__attribute__((destructor)) void deinit_FANDFFILEUSE() {
	free(myScriptsLoc); free(backupsLoc); free(filesAndFoldersLoc); free(filesAndFoldersBackupLoc); free(symLinksFolderLoc);
}
//-------------------------------------------------------------------------------------------------

// This function adds the current_full_file_path field from the filesAndFolders.csv to the official arrays
struct filesAndFoldersDbObj** filesInUse(char* filesAndFoldersAsStrIn) {
	// Vars
	char* FF = (char*) __func__;
	int numRows;
	int currNumFilesInUse = 0;
	struct filesAndFoldersDbObj** filesInUseObjArr;

	// Data Validation
	if (filesAndFoldersAsStrIn == NULL){
		myPerror(F_FANDFFILEUSE, FF, "Invalid 1st parameter; value is null. Returning null");
		errorBool = 1;
		return NULL;
	}
	
	// Getting the number of rows in the csv
	numRows = getNumRows(filesAndFoldersAsStrIn);
	if (numRows < 0){
		myPerror(F_FANDFFILEUSE, FF, "numRows() function returned an error value (-1). Returning null");
		errorBool = 1;
		return NULL;
	}
	
	// Converting csv-formatted string to struct
	struct filesAndFoldersDbObj** officialFilesAsDbObj = convert(filesAndFoldersAsStrIn, "filesDb");
	if (officialFilesAsDbObj == NULL){
		myPerror(F_FANDFFILEUSE, FF, "convert() function returned a null value. Returning null");
		errorBool = 1;
		return NULL;
	}
	
	// Alloc'ing return array
	filesInUseObjArr = malloc(sizeof(struct filesAndFoldersDbObj*)*numRows);
	
	// Iterating over every struct (representing a row in the csv) and adding to filesInUse if inUse var != 0
	int i = 0; //keeping i outside the for loop for later use
	for (; officialFilesAsDbObj[i]->id != -1; i++){
		if (officialFilesAsDbObj[i]->inUse != 0){
			filesInUseObjArr[currNumFilesInUse] = officialFilesAsDbObj[i];
			currNumFilesInUse++;
		}
		else{ myFree(officialFilesAsDbObj[i]);}
	}

	// If no files are in use, return null
	if (currNumFilesInUse == 0){
		free(officialFilesAsDbObj);
		free(filesInUseObjArr);
		myPerror(F_FANDFFILEUSE, FF, "WARNING: No files are in use. Returning null");
		return NULL;
	}
	else{
		filesInUseObjArr[currNumFilesInUse] = officialFilesAsDbObj[i]; //adding sentinel struct
		free(officialFilesAsDbObj);
		return filesInUseObjArr;
	}
}

// This function checks if files that are in use in the csv actually exist
struct filesAndFoldersDbObj** missingOfficialFiles(char* filesAndFoldersAsStrIn){
	// Vars
	struct filesAndFoldersDbObj** listOfFilesInUse;
	int size;
	struct filesAndFoldersDbObj** missingFiles;
	int numMissingFiles = 0;
	char* FF = (char*) __func__;

	// Data Validation
	if (filesAndFoldersAsStrIn == NULL){
		myPerror(F_FANDFFILEUSE, FF, "Invalid 1st parameter; value is null. Returning null");
		errorBool = 1;
		return NULL;
	}

	// Finding list of files in use
	listOfFilesInUse = filesInUse(filesAndFoldersAsStrIn);
	if (listOfFilesInUse == NULL){
	       myPerror(F_FANDFFILEUSE, FF, "WARNING: filesInUse() returned a null value. Returning null");
	       myPerror(F_FANDFFILEUSE, FF, "	This could be an error or could be because no files are in use");
	       errorBool = 1;
	       return NULL;
	}

	// Getting size of array
	size = getArrSize(listOfFilesInUse);
	if (size < 0){
		myPerror(F_FANDFFILEUSE, FF, "getArrSize returned an error value. Returning null");
		errorBool = 1;
		return NULL;
	}

	// Alloc'ing return array
	missingFiles = malloc(sizeof(struct filesAndFoldersDbObj*)*(size+1));

	// Iterating over every file in use and checking to see if it exists
	int i = 0; //keeping i outside the for loop for later use
	for (; listOfFilesInUse[i]->id != -1; i++){
		//if file isn't in its expected path, add to list of missing files
		if(fileExists(listOfFilesInUse[i]->fullPath,0) == 0){
			missingFiles[numMissingFiles] = listOfFilesInUse[i];
			numMissingFiles++;
		}
		else{ myFree(listOfFilesInUse[i]);}
	}
	
	// Free
	if (numMissingFiles == 0){ 
		free(missingFiles);
		free(listOfFilesInUse[i]);
		free(listOfFilesInUse);
		puts("All files were found in their expected locations");
		return NULL;
	}

	missingFiles[numMissingFiles] = listOfFilesInUse[i];
	free(listOfFilesInUse);
	return missingFiles;
}
