#include "symLink_33.sl" //filesAndFoldersBasicFunctions.h
#include "symLink_76.sl" //database.h
#include "symLink_49.sl" //tableMethods.h

// GLOBAL VARS-------------------------------------------------------------------------------------
int arrIncFilesAndFoldersDbObj = 128;
int numElementsInFandFDbObj = 12;

// Error Msg Constant Vars
char* F_FANDFBASIC = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function creates a sentinel database struct
//	this struct has no alloc'd values and an id of -1
struct filesAndFoldersDbObj* sentinelFilesDbStruct(){
        struct filesAndFoldersDbObj* sentinel = malloc(sizeof(struct filesAndFoldersDbObj));

        sentinel->id = -1;
        sentinel->type = '\0';
        sentinel->inUse = 0;
        sentinel->fullPath = "";
        sentinel->fileName = "";
        sentinel->extension = "";
        sentinel->pathInRepo = "";
        sentinel->movedOrChangedName = 0;
        sentinel->oldPath = "";
	sentinel->symLinkNum = 0;
	sentinel->rmd = 7;
	sentinel->rwe = 7;

        return sentinel;
}

// This function gets the number of columns expected in the database
// Using this method instead of getNumCols from csv in case csv isn't accurate or missing cols
int numColsFilesAndFolderObj(){ return numElementsInFandFDbObj;}

// This function adds a value to a db obj at a given position
// 	the position is the "index" of the variable based on the order in the filesAndFoldersDbObj.h file
// 	the position also represents the column the value is expected to be in the csv
void addElementToStruct(struct filesAndFoldersDbObj* objIn, int pos, char* colVal){
        // Vars
	char* FF = (char*) __func__;
	struct filesAndFoldersDbObj* defaultColValues = sentinelFilesDbStruct();

	// Data Validation-------------------------------------------------------------------------
	if (objIn == NULL){
		myPerror(F_FANDFBASIC, FF, "Invalid 1st parameter (files and folder object); value is null. Returning nothing (function return type is void");
		return;
	}
	if (objIn < 0 || pos > numElementsInFandFDbObj){
		myPerror(F_FANDFBASIC, FF, "Invalid 2nd parameter (position); value is either less than 0 or greater than number of elements in struct");
		myPerror(F_FANDFBASIC, FF, "	Returning nothing (function return type is void");
		return;
	}
	if (colVal == NULL){
		myPerror(F_FANDFBASIC, FF, "Invalid 3rd parameter (col value); value is null. Returning nothing (function return type is void");
		return;
	}
	//-----------------------------------------------------------------------------------------


	if (pos == 0){
		if (strlen(colVal) == 0){ objIn->id = defaultColValues->id;}
		else{ objIn->id = atoi(colVal);}
	}
        if (pos == 1){
		if (strlen(colVal) == 0){ objIn->type = defaultColValues->type;}
		else{ objIn->type = colVal[0];}
	}
        if (pos == 2){
		if (strlen(colVal) == 0){ objIn->inUse = defaultColValues->inUse;}
		else{ objIn->inUse = atoi(colVal);}
	}
        if (pos == 3){ 
		int l = strlen(colVal)+1;
                objIn->fullPath = createStr(l); 
                strcpy(objIn->fullPath,colVal);
        }
        if (pos == 4){ 
		int l = strlen(colVal)+1;
                objIn->fileName = createStr(l); 
                strcpy(objIn->fileName,colVal);
        }
        if (pos == 5){ 
		int l = strlen(colVal)+1;
                objIn->extension = createStr(l); 
                strcpy(objIn->extension,colVal);
        }
        if (pos == 6){ 
		int l = strlen(colVal)+1;
                objIn->pathInRepo = createStr(l); 
                strcpy(objIn->pathInRepo,colVal);
        }
        if (pos == 7){
		if (strlen(colVal) == 0){ objIn->movedOrChangedName = defaultColValues->movedOrChangedName;}
		else{ objIn->movedOrChangedName = atoi(colVal);}
	}
        if (pos == 8){ 
		int l = strlen(colVal)+1;
                objIn->oldPath = createStr(l); 
                strcpy(objIn->oldPath,colVal);
        }
	if (pos == 9){
		if (strlen(colVal) == 0){ objIn->symLinkNum = defaultColValues->symLinkNum;}
		else{ objIn->symLinkNum = atoi(colVal);}
	}
	if (pos == 10){
		if (strlen(colVal) == 0){ objIn->rmd = defaultColValues->rmd;}
		else{ objIn->rmd = atoi(colVal);}
	}
	if (pos == 11){
		if (strlen(colVal) == 0){ objIn->rwe = defaultColValues->rwe;}
		else{ objIn->rwe = atoi(colVal);}
	}

	free(defaultColValues);
}

// This function removes a row/struct from a struct array based on a column value (serving as the key of the struct to remove)
struct filesAndFoldersDbObj** remRowFromDbArr(int colNum, void* colVal, struct filesAndFoldersDbObj** dbIn){
	// Using separate db so I don't interfere with original struct array in case it's used elsewhere
	// This is contradicted by the free's of the struct[i] matching the colVal, but it was needed for use in "filesAndFoldersMethods.c - removeFoundFilesFromMissingFiles"
	
	// Vars
	int x = -1;
	char* y = NULL;
	int movePtrsDownOneIndex = 0;
	struct filesAndFoldersDbObj** db = dbIn; 
	char* FF = (char*) __func__;

	// Data Validation-------------------------------------------------------------------------
	if (colNum < 0 || colNum > numElementsInFandFDbObj){
		myPerror(F_FANDFBASIC, FF, "Invalid 1st parameter (col number); value is either less than 0 or greater than number of elements in struct. Returning null");
		return NULL;
	}
	if (colVal == NULL){
		myPerror(F_FANDFBASIC, FF, "Invalid 2nd parameter (col value); value is null. Returning null");
		return NULL;
	}
	if (dbIn == NULL){
		myPerror(F_FANDFBASIC, FF, "Invalid 3rd parameter (struct array); value is null. Returning null");
		return NULL;
	}
	//-----------------------------------------------------------------------------------------

	if (colNum == 0){ x = *(int*) colVal;}
	else if (colNum == 3 || colNum == 4 || colNum == 6 || colNum == 8){ char* y = (char*) colVal;}
	else {
		myPerror(F_FANDFBASIC, FF, "Invalid value for 2nd parameter (col value); value is not an expected number. Accepted values are: ");
		myPerror(F_FANDFBASIC, FF, "	3, 4, 6, 8");
		myPerror(F_FANDFBASIC, FF, "	Returning null");
		return NULL;
	}


	// Iterating over every element in the db until find matching key
	int i = 0; //keeping this var outside for loop for later use
	for (; db[i]->id != -1; i++){
		if (x != -1 && db[i]->id == x){
			myFree(db[i]);
			movePtrsDownOneIndex = 1;
		}
		else if (y != NULL && colNum == 3 && strcmp(db[i]->fullPath, y) == 0){
			myFree(db[i]);
			movePtrsDownOneIndex = 1;
		}
		else if (y != NULL && colNum == 4 && strcmp(db[i]->fileName, y) == 0){
			myFree(db[i]);
			movePtrsDownOneIndex = 1;
		}
		else if (y != NULL && colNum == 6 && strcmp(db[i]->pathInRepo, y) == 0){
			myFree(db[i]);
			movePtrsDownOneIndex = 1;
		}
		else if (y != NULL && colNum == 8 && strcmp(db[i]->oldPath, y) == 0){
			myFree(db[i]);
			movePtrsDownOneIndex = 1;
		}
		
		//move values down an index after removing the other one
		if (movePtrsDownOneIndex > 0 && dbIn[i+1]->id != -1){ db[i] = db[i+1];}

	}

	//freeing the old sentinel struct
	free(db[i]);
	db[i-1] = sentinelFilesDbStruct(); //adding a new sentinal struct at the new last index
	return db;
}


// This function converts csv to files and folders struct array
struct filesAndFoldersDbObj** csvToDbObjArr(char* fileInStrFormat){
	// Vars
	char* FF = (char*) __func__;
        int numRows;
	int numCols = numColsFilesAndFolderObj();
	struct filesAndFoldersDbObj** db;

	// Data Validation
	if (fileInStrFormat == NULL){
		myPerror(F_FANDFBASIC, FF, "Invalid 1st parameter (fileInStrFormat); value is null. Returning null");
		return NULL;
	}
	numRows = getNumRows(fileInStrFormat)-1; //subtracting 1 cuz extra newline char in file that convert() doesn't remove
	if (numRows < 0){
		myPerror(F_FANDFBASIC, FF, "Error with getNumRows() function. Returned a negative value. Retruning NULL");
		return NULL;
	}
        
        db = malloc(sizeof(struct filesAndFoldersDbObj*)*(numRows));

	// Iterating over every row in the csv
        for (int i = 1; i < numRows; i++){
		struct filesAndFoldersDbObj* rowStruct = malloc(sizeof(struct filesAndFoldersDbObj));
		char* row = getRow(i, fileInStrFormat);
		if (row == NULL){
			myPerror(F_FANDFBASIC, FF, "getRow() function returned a null value. Returning null");
			free(rowStruct);
			return NULL;
		}
		
		// Finding the col value at each row and adding it to rowStruct
		for (int j = 0; j < numCols; j++){
			char* col = getCol(j, row);
			if (col == NULL){
				myPerror(F_FANDFBASIC, FF, "getCol() function returned a null value. Returning null");
				free(row);
				free(rowStruct);
				free(db);
				return NULL;
			}
			addElementToStruct(rowStruct, j, col);
			free(col);
		}
		db[i-1] = rowStruct; //add rowStruct to database struct
		free(row);
        }

        db[numRows-1] = sentinelFilesDbStruct(); //add sentinel value
        return db;

}

// This function converts files and folders struct to a csv-formatting string
char* dbObjToStr(struct filesAndFoldersDbObj* objIn){
        // Vars
	char* returnStr = createStr();
        int size = 0;
	char* FF = (char*) __func__;
	
	// Data Validation
	if (objIn == NULL) {myPerror(F_FANDFBASIC, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}

        // Converting id to string
	char idAsStr[128]; snprintf(idAsStr, sizeof(idAsStr), "%d", objIn->id);
	//don't need to realloc cuz createStr() alloc's a string of size 128 by default
        strcat(returnStr, idAsStr);
        strcat(returnStr, ",");

	// Converting type to string
        char typeAsStr[2]; typeAsStr[0] = objIn->type; typeAsStr[1] = '\0';
	//don't need to realloc cuz createStr() alloc's a string of size 128 by default
        strcat(returnStr, typeAsStr);
        strcat(returnStr, ",");

        // Converting inUse to string
	char inUseAsStr[2]; snprintf(inUseAsStr, sizeof(inUseAsStr), "%d", objIn->inUse);
	//don't need to realloc cuz createStr() alloc's a string of size 128 by default
        strcat(returnStr, inUseAsStr);
        strcat(returnStr, ",");

        // Adding full path
	size = strlen(objIn->fullPath)+1; //+1 for the comma
        returnStr = myRealloc(size, returnStr);
        strcat(returnStr, objIn->fullPath);
        strcat(returnStr, ",");

	// Adding file name
        size = strlen(objIn->fileName)+1;
        returnStr = myRealloc(size, returnStr);
        strcat(returnStr, objIn->fileName);
        strcat(returnStr, ",");

	// Adding extension
        size = strlen(objIn->extension)+1;
        returnStr = myRealloc(size, returnStr);
        strcat(returnStr, objIn->extension);
        strcat(returnStr, ",");

	// Adding path in repo
        size = strlen(objIn->fullPath)+1;
        returnStr = myRealloc(size, returnStr);
        strcat(returnStr, objIn->pathInRepo);
        strcat(returnStr, ",");

	// Converting moved to string
        char movedAsStr[2]; snprintf(movedAsStr, sizeof(movedAsStr), "%d", objIn->movedOrChangedName);
        size = strlen(movedAsStr)+1;
        returnStr = myRealloc(size, returnStr);
        strcat(returnStr, movedAsStr);
        strcat(returnStr, ",");

	// Adding old path
        size = strlen(objIn->oldPath)+1;
        returnStr = myRealloc(size, returnStr);
        strcat(returnStr, objIn->oldPath);
        strcat(returnStr, ",");
       
	// Adding symlink number
        char symLinkNumAsStr[128]; snprintf(symLinkNumAsStr, sizeof(symLinkNumAsStr), "%d", objIn->symLinkNum);
	size = strlen(symLinkNumAsStr)+1;
	returnStr = myRealloc(size, returnStr);
	strcat(returnStr, symLinkNumAsStr);	
        strcat(returnStr, ",");

	// Adding rename, move, and delete permissions number
        char rmdAsStr[128]; snprintf(rmdAsStr, sizeof(rmdAsStr), "%d", objIn->rmd);
	size = strlen(rmdAsStr)+1;
	returnStr = myRealloc(size, returnStr);
	strcat(returnStr, rmdAsStr);	
        strcat(returnStr, ",");

	// Adding read, write, and execute permissions number
        char rweAsStr[128]; snprintf(rweAsStr, sizeof(rweAsStr), "%d", objIn->rwe);
	size = strlen(rweAsStr)+1;
	returnStr = myRealloc(size, returnStr);
	strcat(returnStr, rweAsStr);	

	// Adding newline char for next col
	strcat(returnStr, "\n");

        return returnStr;

}

// This function converts files and folders struct array to csv-formatting string
char* dbObjArrToCsv(struct filesAndFoldersDbObj** objArrIn){
	// Vars
	char* FF = (char*) __func__;
        char* returnStr;
	
	//Data Validation
	if (objArrIn == NULL) {myPerror(F_FANDFBASIC, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
        
	// Iterating over every struct in the arr and converting to string
	for (int i = 0; objArrIn[i]->id != -1; i++){
                char* row = dbObjToStr(objArrIn[i]);
		if (row == NULL){
			myPerror(F_FANDFBASIC, FF, "dbObjToStr() function returned null. Returning null");
			return NULL;
		}

                int size = strlen(row);
               
	       	if (i == 0){ returnStr = createStr(size);}
		else{ returnStr = myRealloc(size, returnStr);}
	       	if (returnStr == NULL){
			myPerror(F_FANDFBASIC, FF, "myRealloc() function returned null. Returning null");
			free(row);
			free(returnStr);
			return NULL;
		}
                
		strcat(returnStr, row);
                free(row);
        }
        return returnStr;
}

