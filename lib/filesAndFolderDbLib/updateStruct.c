#include "symLink_80.sl"
#include "symLink_36.sl"
#include "symLink_77.sl"
#include "symLink_78.sl"
#include "symLink_79.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Message Constant Vars
char* F_FANDFUPDATE = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function adds files to to struct array
struct filesAndFoldersDbObj** addFileToFilesDbArr(struct filesAndFoldersDbObj** dbIn, char* filePathIn, char* csvPathIn){
	// Vars
	char* FF = (char*) __func__;
	int fileExistsBool;
	int newStructSize;
	int newFileId;
	char newFileType;
	char* newPath;
	char* newFileName;
	char* newExt;
	struct filesAndFoldersDbObj* defaultVals = sentinelFilesDbStruct();

	// Data Validation-------------------------------------------------------------------------
	if (dbIn == NULL){
		myPerror(F_FANDFUPDATE, FF, "Invalid input at parameter 1 (struct array to update); value is null. Returning null");
		return NULL;
	}
	if (filePathIn == NULL){
		myPerror(F_FANDFUPDATE, FF, "Invalid input at parameter 2 (file path); value is null. Returning null");
		return NULL;
	}
	if (csvPathIn == NULL){
		myPerror(F_FANDFUPDATE, FF, "Invalid input at parameter 3 (csv path); value is null. Returning null");
		return NULL;
	}
	//-----------------------------------------------------------------------------------------
		
	
	// Getting Values--------------------------------------------------------------------------
	fileExistsBool = fileExists(filePathIn,0);
	if (fileExistsBool == 0){ 
		myPerror(F_FANDFUPDATE, FF, "Invalid file path ", filePathIn, ". Not adding file and returning parameter 1 (struct array to update)");
		return dbIn;
	}	
	newStructSize = getArrSize(dbIn)+1;
	newFileId = newStructSize; //don't need to add 1 cuz function gets num rows from file, which has a header row
	newFileType = systemObjType(filePathIn);
	if (newFileType == '\0'){
		myPerror(F_FANDFUPDATE, FF, "systemObjType() function returned an error value (empty char). Returning null");
		return NULL;
	}
	newPath = createStr(strlen(filePathIn)+1); strcpy(newPath, filePathIn);
	newFileName = getFileName(filePathIn);
	if (newFileName == NULL){ myPerror(F_FANDFUPDATE, FF, "getFileName() returned a null value. Returning null");return NULL;}
	newExt = getFileExt(filePathIn, 2);
	if (newExt == NULL){ myPerror(F_FANDFUPDATE, FF, "getFileExt() returned a null value. Returning null"); return NULL;}
	//-----------------------------------------------------------------------------------------

	// Creating and adding values to new struct------------------------------------------------
	//using sentinel struct function just to create struct, not to use as sentinel struct
	struct filesAndFoldersDbObj* newFile = sentinelFilesDbStruct();

	newFile->id = newFileId;
	newFile->type = newFileType;
	newFile->inUse = 1; //hardcoding this cuz if adding a file, I can assume it means it is in use
	newFile->fullPath = newPath;
	newFile->fileName = newFileName;
	newFile->extension = newExt;
	newFile->pathInRepo = createStr();
	newFile->movedOrChangedName = defaultVals->movedOrChangedName;
	newFile->oldPath = createStr(); //hardcoding this cuz if adding a file, I can assume it wasn't moved
	newFile->symLinkNum = defaultVals->symLinkNum;
	newFile->rmd = defaultVals->rmd;
	newFile->rwe = defaultVals->rwe;
	//-----------------------------------------------------------------------------------------

	// Adding new struct to struct array
	dbIn[newStructSize] = dbIn[newStructSize-1]; //moving the sentinel struct to new end index
	dbIn[newStructSize-1] = newFile;

	free(defaultVals);
	return dbIn;
}

// This function is a helper function for update struct array with new paths for files that have changed paths
struct filesAndFoldersDbObj** updateDbWithNewPathsHelper(char* filePathIn, struct filesAndFoldersDbObj** dbIn){
	// Vars
	char* FF = (char*) __func__;
	char* fileName;

	// Data Validation
	if (filePathIn == NULL){
		myPerror(F_FANDFUPDATE, FF, "Invalid 1st parameter (file path); value is null. Returning null");
		return NULL;
	}
	if (dbIn == NULL){
		myPerror(F_FANDFUPDATE, FF, "Invalid 2nd parameter (struct array); value is null. Returning null");
		return NULL;
	}

	// Getting values
	fileName = getFileName(filePathIn);
	
	// Iterating over every file in struct array until I find the struct with the same file name
	for (int i = 0; dbIn[i]->id != -1; i++){
		if (strcmp(dbIn[i]->fileName, fileName) == 0){ //if matching file name
			// Update old path			
			free(dbIn[i]->oldPath);
			dbIn[i]->oldPath = createStr(strlen(dbIn[i]->fullPath)+1);
			strcat(dbIn[i]->oldPath, dbIn[i]->fullPath);
			
			// Update full path  (current path)
			free(dbIn[i]->fullPath);
			dbIn[i]->fullPath = createStr(strlen(filePathIn)+1);
			strcat(dbIn[i]->fullPath, filePathIn);

			dbIn[i]->movedOrChangedName = 1;
		}
	}

	free(fileName);
	return dbIn;
}

// This function updates struct array with new paths for files that aren't in their current path from the csv
struct filesAndFoldersDbObj** updateDbWithNewPaths(struct filesAndFoldersDbObj** dbIn){
	// Vars
	char* FF = (char*) __func__;

	// Data Validation
	if (dbIn == NULL){
		myPerror(F_FANDFUPDATE, FF, "Invalid 1st parameter (struct array); value is null. Returning null");
		return NULL;
	}
	
	// Iterating over every values in the global string array "fileLocArr"
	// 	-this var was populated by the run command threads
	for (int i = 0; fileLocArr[i] != NULL; i++){
		// If the fileLocArr isn't empty (denoting another path was found)
		if (strcmp(fileLocArr[i], "") != 0){
			//update struct arr with new file paths
			dbIn = updateDbWithNewPathsHelper(fileLocArr[i], dbIn);
			if (dbIn == NULL){
				myPerror(F_FANDFUPDATE, FF, "updateDb helper function returned null. Returning null");
				return NULL;
			}
		}
	}
	
	return dbIn;
}

// This function removes files found in a different path from the missingFiles struct array
struct filesAndFoldersDbObj** removeFoundFilesFromMissingFiles(struct filesAndFoldersDbObj** missingFilesIn){
	// Vars
	char* FF = (char*) __func__;
	//using a separate return string to not mess with the iteration in the for loop below
	struct filesAndFoldersDbObj** missingFilesReturnObjArr;

	// Data Validation
	if (missingFilesIn == NULL){
		myPerror(F_FANDFUPDATE, FF, "Invalid 1st parameter (struct array of missing fiels); value is null. Returning null");
		return NULL;
	}
	
	// Iterating over every values in the global string array "fileLocArr"
	// 	-this var was populated by the run command threads
	for (int i = 0; fileLocArr[i] != NULL; i++){
		// If the fileLocArr isn't empty (denoting another path was found)
		if (strcmp(fileLocArr[i], "") != 0){
			//get file id. Need to convert to int ptr so I can pass it as a void pointer to remRowFromDbArr()
			//I can use the "i" var with missingFilesIn cuz fileLocArr is just the new path of all files from missingFilesIn
			int* missingFileIdPtr = &missingFilesIn[i]->id;
			//remove struct from struct arr
			missingFilesReturnObjArr = remRowFromDbArr(0, (void*) missingFileIdPtr, missingFilesIn);
			if (missingFilesReturnObjArr == NULL){
				myPerror(F_FANDFUPDATE, FF, "remRowFromDbArr() function returned a null value. Returning null");
				return NULL;
			}

		}
	}


	return missingFilesReturnObjArr;
}
