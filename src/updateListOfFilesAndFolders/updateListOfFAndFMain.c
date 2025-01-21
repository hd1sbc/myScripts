#include "updateFAndF.h"
#include "userInputMissingFiles.h"
#include "userInputUnmarkedFiles.h"
#include "userInputWriteToFile.h"

// GLOBAL VARIABLES--------------------------------------------------------------------------------
char* F_FANDFMAIN = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

int main(){
	
	// Vars
	char* FF = (char*) __func__;
	time_t start, end;
	double elapsed;
	char* csvPath = createStr(256);
	char* filesList;
	struct filesAndFoldersDbObj** db;
	struct filesAndFoldersDbObj*** returnUserInputArr;
	struct filesAndFoldersDbObj** missingFiles;
	
	// Getting Values
	strcat(csvPath, filesAndFoldersLoc);	
	filesList = convert(csvPath, "string");
	db = convert(filesList, "filesDb");
	time(&start); //starting timer. Used to see if I need to use a loading bar for finding unmarked files

	// Starting function to find unmarked files in the background
	// Running in the background to save time. If user ends up not want to add marked files, the values will just be free'd
	pthread_t unMarkedFilesThread;
	if (pthread_create(&unMarkedFilesThread, NULL, unMarkedFiles, (void*) filesList) != 0) {
		myPerror(F_FANDFMAIN, FF, "Error creating a thread for unMarkedFilesThread. Returning -1");
		return -1;
	}
	
	// Getting missing files-related user input------------------------------------------------
	returnUserInputArr = userInputFindMissingFiles(filesList, db);
	if (returnUserInputArr == NULL && errorBool == 1){
		myPerror(F_FANDFMAIN, FF, "userInputFindMissingFiles() function returned a null value. Returning -1");
		free(csvPath); free(filesList); myFree(db);
		return -1;
	}
	
	db = returnUserInputArr[0];
	if (db  == NULL){
		myPerror(F_FANDFMAIN, FF, "userInputFindMissingFiles() function at index 0 (database struct array) returned a null value. Returning -1");
		free(csvPath); free(filesList); myFree(db); myFree(returnUserInputArr);
		return -1;
	}
	
	missingFiles = returnUserInputArr[1];
	if (missingFiles  == NULL && errorBool == 1){
		myPerror(F_FANDFMAIN, FF, "userInputFindMissingFiles() function at index 1 (missing files struct array) returned a null value. Returning -1");
		free(csvPath); free(filesList); myFree(db); free(returnUserInputArr);
		return -1;
	}
	//-----------------------------------------------------------------------------------------


	// Getting unmarked files user input-------------------------------------------------------
	puts("Checking system to see if there are any files that are marked as files to be added but aren't in the official list of files and folders");
	
	// Determining loading bar info for unmarked files
	time(&end); //ending timer
	elapsed = difftime(end, start);
	//loading bar creation
	lbParams* l;
	if (elapsed < 8){
		int loadingBarTime = 12 - (int) elapsed;
		l = loadingBarC(loadingBarTime, 1, "");	
	}

	// Getting thread info
	void* status;
	pthread_join(unMarkedFilesThread, &status);
        char* unmarked = (char*)(intptr_t)status;

	if (elapsed < 8){ closeLoadingBarC(l);} //closing loading bar if it was used
	if (unmarked == NULL){
		myPerror(F_FANDFMAIN, FF, "unMarkedFilesThread() function returned a null value. Returning -1");
		free(csvPath); free(filesList); myFree(db); myFree(missingFiles); free(returnUserInputArr);
		return -1;
	}

	// Getting user input for unmarked files
	if (strlen(unmarked) > 0){ db = unmarkedUserInput(db, unmarked, csvPath);}
	else{ puts("No unmarked files found in system");}
	//-----------------------------------------------------------------------------------------
	
	// Getting user input for writing to file
	//	Not putting in separate function cuz too many things to pass and it's just easier here
	if (strlen(unmarked) == 0 && missingFiles == NULL){ puts("Nothing to write to file");}
	else{
		int returnVal = writeToFandFCsv(filesList, db, csvPath);
		if (returnVal != 0){	
			myPerror(F_FANDFMAIN, FF, "writeToFandFCsv() function returned an error value (!=0). Returning -1");
			free(csvPath); free(filesList); myFree(db); myFree(missingFiles); free(returnUserInputArr);

			return -1;
		}
	}
	
       	// Free's
        if (db != NULL){  myFree(db);}
        myFree(unmarked);
        if (missingFiles != NULL){ myFree(missingFiles);}
        else{ free(missingFiles);}
        free(returnUserInputArr);
        free(csvPath);
        free(filesList);
        if (fileLocArr != NULL){ myFree(fileLocArr);}

	return 0;
}
