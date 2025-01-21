#include "userInputUnmarkedFiles.h"
#include "updateFAndF.h"
#include "userInputMissingFiles.h"

// GLOBAL VARIABLES--------------------------------------------------------------------------------
// Error Mssg Constant Vars
char* F_UPDATEFANDFUNMARKED = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function takes user input and performs actions related to unmarked files (files that should be in csv but aren't)
struct filesAndFoldersDbObj** unmarkedUserInput(struct filesAndFoldersDbObj** dbIn, char* unmarkedIn, char* csvPathIn){
	// Vars
	char* FF = (char*) __func__;

	// Data Validation
	if (dbIn == NULL){ myPerror(F_UPDATEFANDFUNMARKED, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (unmarkedIn == NULL){ myPerror(F_UPDATEFANDFUNMARKED, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}
	if (csvPathIn == NULL){ myPerror(F_UPDATEFANDFUNMARKED, FF, "Invalid 3rd parameter; value is null. Returning null"); return NULL;}
	
	myPrint(unmarkedIn); //printing unmarked files
	
	// Option to add unmarked files to csv
	puts("Do you want to add missing/unmarked files to official list of files and folders? (Y/n)");
	char addMissingFilesInput;
	scanf(" %c", &addMissingFilesInput);
	if (addMissingFilesInput == 'Y' || addMissingFilesInput == 'y'){ 
		// Resizing db. Realloc'ing once based on the number of rows in unmarked so I don't need realloc by 1 for every row in unmarked
		int numRows = getNumRows(unmarkedIn);
		if (numRows < 0){ myPerror(F_UPDATEFANDFUNMARKED, FF, "getNumRows() function returned an error value (<0). Returning null"); return NULL;}
		int newDbArrSize = getArrSize(dbIn) + numRows;
		if (numRows < 0){ myPerror(F_UPDATEFANDFUNMARKED, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
		dbIn = realloc(dbIn, sizeof(struct filesAndFoldersDbObj*)*(newDbArrSize+1));
	
		//loading bar for if there are more than 7 items
		//addFileToFilesDbArr takes a while to create each element. I haven't been able to pinpoint what aspect takes so long
		lbParams* l;
		if (numRows > 7){ l = loadingBarC(2, 1, "");}

		// Iterating over every row in unmarked and add to db
		for (int i = 0; i < numRows; i++){
			char* unmarkedFileName = getRow(i, unmarkedIn);
			if (unmarkedFileName == NULL){
				myPerror(F_UPDATEFANDFUNMARKED, FF, "getRow() function returned a null value. Returning null");
				return NULL;
			}
			dbIn = addFileToFilesDbArr(dbIn, unmarkedFileName, csvPathIn);
			if (dbIn == NULL){
				myPerror(F_UPDATEFANDFUNMARKED, FF, "addFileToFilesDbArr() function returned a null value. Returning null");
				return NULL;
			}
			free(unmarkedFileName);
		}

		if (numRows > 7){ closeLoadingBarC(l);} //closing loading bar if it was used
		
	}	
	else if (addMissingFilesInput != 'N' && addMissingFilesInput != 'n'){ puts("Invalid input. Not adding missing/unmarked files");}


	return dbIn;
}
