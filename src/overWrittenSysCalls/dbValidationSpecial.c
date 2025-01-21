#include "dbValidationSpecial.h"
#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"

// GLOBAL VARS-------------------------------------------------------------------------------------
extern int (*orig_open)(const char*, int, ...);

// Error Msg Constant Vars
char* F_DBVALSPECIAL = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

/*
These functions are nearly identical to isValidDb.c, but to avoid circular loop of open() function, these were updated a little to use orig_open instead
*/

// This function gets the expected data type for each column of a database (csv) file. Returns values as a string array
char** getDataTypesArrNewCFuncs(char* filePathIn){
	// Vars
	char* FF = (char*) __func__;
	char* tableColTypeAsStr;
	int numCols = 0;
	char** returnDataTypes;
	int numRowsInTableColType;

	// Data Validation
	if (filePathIn == NULL){ myPerror(F_DBVALSPECIAL, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	
	// Getting Values--------------------------------------------------------------------------
	tableColTypeAsStr = readFileSpecial(tableColTypeListLoc);
	if (tableColTypeAsStr == NULL){ myPerror(F_DBVALSPECIAL, FF, "readFileSpecial() function returned a null value. Returning null"); return NULL;}
	
	numRowsInTableColType = getNumRows(tableColTypeAsStr)-1; //subtracting 1 cuz tableColTypeAsString ends with '\n', adding an empty row
	if (numRowsInTableColType < 0){
		free(tableColTypeAsStr);
		myPerror(F_DBVALSPECIAL, FF, "getNumRows() function returned an error val (<0). Returning null");
		return NULL;
	}
	
	returnDataTypes = malloc(sizeof(char*)*2);
	//-----------------------------------------------------------------------------------------
	
	// Go through all the rows in tableColTypeList
	for (int i = 1; i < numRowsInTableColType; i++){
		// Get the row
		char* currRow = getRow(i, tableColTypeAsStr);
		if (currRow == NULL){
			myPerror(F_DBVALSPECIAL, FF, "getRow() function returned a null value. Returning null");
			myFree(returnDataTypes); free(tableColTypeAsStr);
			return NULL;
		}
		if (strlen(currRow) == 0){ myPerror(F_DBVALSPECIAL, FF, "WARNING: getRow() function returned an empty string. Program will continue but errors will likely occur");}

		// Get the file path from that row
		char* filePathCol = getCol(filePathsColTypeListColNum, currRow);
		if (filePathCol == NULL){
			myPerror(F_DBVALSPECIAL, FF, "getCol() function returned a null value. Returning null");
			free(currRow); free(tableColTypeAsStr); myFree(returnDataTypes);
			return NULL;
		}

		// If the file path at that row == filePathIn
		if (strcmp(filePathCol, filePathIn) == 0){
			// Find the dataType for that col from the row
			char* dataType = getCol(dataTypesColTypeListColNum, currRow);
			if (dataType == NULL){
				myPerror(F_DBVALSPECIAL, FF, "getCol() function returned a null value. Returning null");
				free(filePathCol); free(currRow); free(tableColTypeAsStr); myFree(returnDataTypes);
				return NULL;
			}

			// Add the row to the string array
			if (i == 1){ returnDataTypes[0] = createStr(); returnDataTypes[1] = NULL;}
			else{ returnDataTypes = myRealloc(returnDataTypes);} //realloc string array
		
			if (returnDataTypes == NULL){ //checking if failure to realloc
				myPerror(F_DBVALSPECIAL, FF, "myRealloc() function returned a null value. Returning null");
				free(dataType); free(filePathCol); free(currRow); free(tableColTypeAsStr); myFree(returnDataTypes);
				return NULL;
			}
			strcat(returnDataTypes[numCols], dataType); //add data type to current list
			if (returnDataTypes[numCols][strlen(returnDataTypes[numCols])-1] == '\n'){ //remove newline char if last char has one
				returnDataTypes[numCols][strlen(returnDataTypes[numCols])-1] = '\0';
			}
			free(dataType);
			numCols++;
		}

		free(currRow);
		free(filePathCol);
	}

	free(returnDataTypes[numCols]); //the way I wrote myRealloc for string arrs, it has an extra alloc'd empty string at the end
	free(tableColTypeAsStr);
	return returnDataTypes;

}

// This function compares expected data types based on file path against either the existing csv contents or a string
// 	-if string is empty, it will look at the existing contents in the file path
int compareDbValuesNewCFuncs(const char* filePathIn, char* strIn){
	// Vars
	char* FF = (char*) __func__;
	char** dataTypes;
	char* fileAsStr;
	int numRows;
	int numCols;
       
	// Data Validation
	if (filePathIn == NULL){ myPerror(F_DBVALSPECIAL, FF, "Invalid 1st parameter (file path); value is null. Returning -1"); return -1;}
	if (strIn == NULL){ myPerror(F_DBVALSPECIAL, FF, "Invalid 2nd parameter (new file contents); value is null. Returning -1"); return -1;}
	
	// Getting Values--------------------------------------------------------------------------
	dataTypes = getDataTypesArrNewCFuncs((char*) filePathIn);
	if (dataTypes == NULL){
		myPerror(F_DBVALSPECIAL, FF, "getDataTypesArrNewCFuncs() function returned a null value. Returning -1");
		return -1;
	}

	if (strlen(strIn) > 0){ fileAsStr = createStr(strlen(strIn)); strcat(fileAsStr, strIn);} //if strIn isn't empty, copy it's contents to fileAsStr
	else {fileAsStr = readFileSpecial(filePathIn);}
	if (fileAsStr == NULL){
		myFree(dataTypes);
		myPerror(F_DBVALSPECIAL, FF, "readFileSpecial() function returned a null value. Returning -1");
		return -1;
	}
	
	numRows = getNumRows(fileAsStr)-1; //subtracting 1 to remove header row
	if (numRows < 0){
		free(fileAsStr); myFree(dataTypes);
		myPerror(F_DBVALSPECIAL, FF, "getNumRows() function returned an error val (<0). Returning -1");
		return -1;
	}
	
	numCols = getNumCols(fileAsStr);
	if (numCols < 0){
		free(fileAsStr); myFree(dataTypes);
		myPerror(F_DBVALSPECIAL, FF, "getNumCols() function returned an error val (<0). Returning -1");
		return -1;
	}
	if (numCols != getArrSize(dataTypes)){
		free(fileAsStr); myFree(dataTypes);
		//keep as returning 0. Not an issue with functions, database isn't valid
		myPerror(F_DBVALSPECIAL, FF, "getNumCols() returned a different number of columns from what was found in the tableColType csv. Returning 0");
		return 0;
	}
	//-----------------------------------------------------------------------------------------
	
	// Iterating over every row in fileAsStr and comparing it's data type
	for (int i = 1; i < numRows; i++){
		char* currRow = getRow(i, fileAsStr);
		if (currRow == NULL){
			free(fileAsStr); myFree(dataTypes);
			myPerror(F_DBVALSPECIAL, FF, "getRow() function returned a null value. Returning -1");
			return -1;
		}

		// Iterating over every column in row to check the data type
		for (int j = 0; j < numCols; j++){
			char* currCol = getCol(j, currRow);
			if (strlen(currCol) > 0 && currCol[strlen(currCol)-1] == '\n'){ currCol[strlen(currCol)-1] = '\0';} //removing trailing '\n' usually found in last column of every row			
			if (currCol == NULL){
				free(currRow); free(fileAsStr); myFree(dataTypes);
				myPerror(F_DBVALSPECIAL, FF, "getCol() function returned a null value. Returning 0");
				return 0;
			}
			
			// Checking if int
			if (strcmp(dataTypes[j], "int") == 0 && isInt(currCol) != 1){
				fprintf(stderr, "%s%s%s%s%s", "===", F_DBVALSPECIAL, " - ", FF, "===	");
				fprintf(stderr, "%s%d%s%d", "Invalid database at row ", i, " and col ", j);
				fprintf(stderr, "%s\n", ". Value was expected to be an int but wasn't");
			
				free(currCol); free(currRow); free(fileAsStr); myFree(dataTypes);
				return 0;
			}
			// Checking if bool
			if (strcmp(dataTypes[j], "bool") == 0 && strcmp(currCol, "0") != 0 && strcmp(currCol, "1") != 0){
				fprintf(stderr, "%s%s%s%s%s", "===", F_DBVALSPECIAL, " - ", FF, "===	");
				fprintf(stderr, "%s%d%s%d", "Invalid database at row ", i, " and col ", j);
				fprintf(stderr, "%s\n", ". Value was expected to be a bool but wasn't");
		
				free(currCol); free(currRow); free(fileAsStr); myFree(dataTypes);
				return 0;
			}
			free(currCol);
		}
		free(currRow);
	}

	free(fileAsStr);
	myFree(dataTypes);
	return 1;
}

// This function combines the above functions to check if the db is valid
int isValidDbNewCFuncs(const char* filePathIn, char* strIn){
	// Vars
	char* FF = (char*) __func__;
	char* ext;
	int returnVal = 0;

	// Data Validation
	if (filePathIn == NULL){ myPerror(F_DBVALSPECIAL, FF, "Invalid 1st parameter (file path); value is null. Returning -1"); return -1;}
	if (strIn == NULL){ myPerror(F_DBVALSPECIAL, FF, "Invalid 2nd parameter (new file contents); value is null. Returning -1"); return -1;}
	
	// Getting Values
	ext = getFileExt((char*) filePathIn, 2);
	if (ext == NULL){ myPerror(F_DBVALSPECIAL, FF, "getFileExt() function returned a null value. Returning -1"); return -1;}

	// Checking to see if file name is the tableColType list. There is no data validation for that file
	if (strcmp(filePathIn, tableColTypeListLoc) == 0){ free(ext); return 1;}

	// If csv file, check to see if valid database
	if (strcmp(ext, "csv") == 0){
		//if tableColTypeList file doesn't exist, return valid db (1)
		if (orig_open(tableColTypeListLoc, O_RDONLY) == -1){ free(ext); return 1;}
		
		returnVal = compareDbValuesNewCFuncs(filePathIn, strIn);
		if (returnVal < 0){ myPerror(F_DBVALSPECIAL, FF, "compareDbValuesNewCFuncs() returned an error value (<0). Returning -1"); return -1;}
	}

	free(ext);
	return returnVal;
}
