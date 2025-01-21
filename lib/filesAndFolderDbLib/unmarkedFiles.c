#include "symLink_79.sl"
#include "symLink_36.sl"
#include "symLink_77.sl"
#include "symLink_78.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Message Constant Vars
char* F_FANDFUNMARK = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function runs the find command to find files beginning with "myScripts"
char* runFindCommandFandFMethods(int homeDirOrRootDir){
	// Vars
	char* command;
	char* output;
	char* FF = (char*) __func__;

	
	if (homeDirOrRootDir == 0){
		command = createStr(strlen(myScriptsLoc)+100);
		strcat(command, "find ");
		strcat(command, myScriptsLoc);
		strcat(command, " ! -path \"");
		strcat(command, backupsLoc);
		strcat(command, "*\" 2>/dev/null");
	}
	else if (homeDirOrRootDir == 1){
		command = createStr(strlen(myScriptsLoc)+100);
		strcat(command, "find / -name \"myScripts*\" ! -path \"");
		strcat(command, myScriptsLoc);
		strcat(command, "*\" ! -path \"");
		strcat(command, backupsLoc);
		strcat(command, "*\" 2>/dev/null");
	}
	else{
		myPerror(F_FANDFUNMARK, FF, "Invalid 1st parameter; expected a value of 1 or 0. Returning null");
		return NULL;
	}
	
	output = runCommand(command);
	if (output == NULL){
		myPerror(F_FANDFUNMARK, FF, "runCommand() function returned a null value. Returning null");
		free(command);
		return NULL;
	}
	free(command);

	return output;
}

// This function appends missing files beggining with "myScripts" to the missing files list string
char* appendToMissingFilesStr(char* findCommandOutput, char* missingFilesList, struct filesAndFoldersDbObj** officialObjArr){ 
	// Vars
	int numRows;
	int isMarked = 0;
	char* FF = (char*) __func__;

	// Data Validation
	if (findCommandOutput == NULL){
		myPerror(F_FANDFUNMARK, FF, "Invalid 1st parameter (find command output); value is null. Returning null");
		return NULL;
	}
	if (missingFilesList == NULL){
		myPerror(F_FANDFUNMARK, FF, "Invalid 2nd parameter (missing files list); value is null. Returning null");
		return NULL;
	}

	// Getting num rows of output to know how many times to iterate
	numRows = getNumRows(findCommandOutput);
	if (numRows < 0){ 
		myPerror(F_FANDFUNMARK, FF, "getNumRows() function returned an error value. Returning null");
		return NULL;
	}

	// For every file from find command
	for (int i = 0; i < numRows; i++){
		//get the row from the output of the command
		char* filePath = getRow(i, findCommandOutput);
		if (filePath == NULL){
			myPerror(F_FANDFUNMARK, FF, "getRow() function return a null value. Returning null");
			return NULL;
		}
	
		//removing trailing '/' to match files and folders csv format	
		if (strlen(filePath) > 0 && filePath[strlen(filePath)-1] == '/'){ 
			filePath[strlen(filePath)-1] = '\0';
		}

		// For every object in the official object array
		for (int j = 0; officialObjArr[j]->id != -1; j++){
			//if the file path = path from obj arr, then isMarked = true (1)
			if (strcmp(filePath, officialObjArr[j]->fullPath) == 0){ isMarked = 1;}
		}

		// If not marked, add to array
		if (isMarked != 1){ 
			missingFilesList = myAppend(missingFilesList, filePath);
			missingFilesList = myAppend(missingFilesList, "\n");
		}

		// Reset values
		isMarked = 0;
		free(filePath);
	}

	//replace trailing newline character with end-of-string char
	if (strlen(missingFilesList) > 0 && missingFilesList[strlen(missingFilesList)-1] == '\n'){
		missingFilesList[strlen(missingFilesList)-1] = '\0';
	}

	return missingFilesList;	

}	

// This function looks for files not in the csv that are:
// 	1) in the myScripts dir
// 	2) throughout the system and begin with "myScripts"
void* unMarkedFiles(void* officialListOfFilesIn){
	// Vars
	struct filesAndFoldersDbObj** officialObjArr; 
	char* returnStr = createStr();
	char* output;
	char* FF = (char*) __func__;
	
	// Data Validation
	if (officialListOfFilesIn == NULL){
		myPerror(F_FANDFUNMARK, FF, "Invalid 1st parameter; value is null. Returning null");
		return NULL;
	}
	
	// Getting Values
	officialObjArr = convert((char*) officialListOfFilesIn, "filesDb");
	if (officialObjArr == NULL){
		myPerror(F_FANDFUNMARK, FF, "convert() function returned a null value. Returning null");
		return NULL;
	}
	
	// Comparing csv files to what is in myScripts dir-----------------------------------------
	
	//getting output of find command only looking in myScript dir
	output = runFindCommandFandFMethods(0);
	if (output == NULL){
		myPerror(F_FANDFUNMARK, FF, "runFindCommandFandFMethods() function returned null value. Returning null");
		return NULL;
	}
	//comparing all files in myScripts dir with those in csv
	returnStr = appendToMissingFilesStr(output, returnStr, officialObjArr);
	if (returnStr == NULL){
		myPerror(F_FANDFUNMARK, FF, "appendToMissingFilesStr() function returned null value. Returning null");
		return NULL;
	}
	free(output);
	//-----------------------------------------------------------------------------------------
	
	// Comparing csv files to what is in entire file system------------------------------------
	// 	-only for scripts beginning with "myScripts"
	
	//getting output of find command looking in root dir
	output = runFindCommandFandFMethods(1);
	if (output == NULL){
		myPerror(F_FANDFUNMARK, FF, "runFindCommandFandFMethods returned null value. Returning null");
		return NULL;
	}
	//comparing all files in root dir with those in csv
	if (strlen(returnStr) > 0){ returnStr = myAppend(returnStr, "\n");}
	returnStr = appendToMissingFilesStr(output, returnStr, officialObjArr);
	if (returnStr == NULL){
		myPerror(F_FANDFUNMARK, FF, "appendToMissingFilesStr() function returned null value. Returning null");
		return NULL;
	}
	free(output);
	//-----------------------------------------------------------------------------------------

	myFree(officialObjArr);

	//replace trailing newline character with end-of-string char
	if (strlen(returnStr) > 0 && returnStr[strlen(returnStr)-1] == '\n'){ returnStr[strlen(returnStr)-1] = '\0';}
	return returnStr;
}


// This function marks files as not in use by changing the inUse value in the struct
struct filesAndFoldersDbObj** markFilesAsNotInUse(struct filesAndFoldersDbObj** dbToUpdate, struct filesAndFoldersDbObj** listFilesNotInUse){

	// Vars
	int sizeFilesNotInUse;
	int prevSymLinkNum;
	char* FF = (char*) __func__;

	// Data Validation
	if (dbToUpdate == NULL){
		myPerror(F_FANDFUNMARK, FF, "Invalid input at parameter 1 (struct array to update); value is null. Returning null");
		return NULL;
	}
	if (listFilesNotInUse == NULL){
		myPerror(F_FANDFUNMARK, FF, "Invalid input at parameter 2 (list of files not in use); value is null. Returning null");
		return NULL;
	}

	sizeFilesNotInUse = getArrSize(listFilesNotInUse);
	
	// Updating different elements in each file that is not in use
	for (int i = 0; i < sizeFilesNotInUse; i++){
		int dbIndexFileNotInUse = listFilesNotInUse[i]->id;

		dbToUpdate[dbIndexFileNotInUse-1]->inUse = 0; //-1 is necessary cuz id values in csv start at 1
		
		//updating the symLinkNum in the csv
		prevSymLinkNum = dbToUpdate[dbIndexFileNotInUse-1]->symLinkNum;
		dbToUpdate[dbIndexFileNotInUse-1]->symLinkNum = 0; //-1 is necessary cuz id values in csv start at 1
		
		//updating corresponding symlink if it's not 0
		if (prevSymLinkNum != 0){
			// Vars
			int strLen = strlen(symLinksFolderLoc) + 100;
			char prevSymLinkNumAsStr[12];
			snprintf(prevSymLinkNumAsStr, sizeof(prevSymLinkNumAsStr), "%d", prevSymLinkNum);
			
			// Running command to test if symlink_prevSymLinkNum exists in symLinks folder, and if so, set it to /dev/null
			char* command = createStr(strLen);
			strcat(command, "if [ -f ");
			strcat(command, symLinksFolderLoc);
			strcat(command, " ]; then ln -sf /dev/null symLink_");
			strcat(command, prevSymLinkNumAsStr);
			strcat(command, ".sl; fi");

			char* output = runCommand(command);
			
			free(command); free(output);
		}
	}

	return dbToUpdate;
}
