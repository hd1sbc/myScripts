#include "symLink_50.sl"
#include "symLink_99.sl"
#include "symLink_49.sl"
#include "symLink_100.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
char* tableColTypeLoc = NULL;
int filePathsColTypeListColNum = 2; //HARDCODED_COL_NUM
int dataTypesColTypeListColNum = 4; //HARDCODED_COL_NUM

// Error Message Constant Vars
char* F_TABLEA = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------


// INSTANTIATING GLOBAL VARS-----------------------------------------------------------------------
__attribute__((constructor)) void init_TABLEA() {
	if (tableColTypeLoc == NULL){ tableColTypeLoc = myFindPathC(357); /*tableColTypeList.csv*/}
}

__attribute__((destructor())) void deinit_TABLEA() {
	if (tableColTypeLoc != NULL){ free(tableColTypeLoc); tableColTypeLoc = NULL;}
}
//-------------------------------------------------------------------------------------------------


// This function gets the value at a column number based on the id
char* getColValFromId(int idIn, int colNumIn, char* dbInStrFormatIn){
	// Vars
	char* FF = (char*) __func__;
	char* row;
	char* col;

	// Data Validation-------------------------------------------------------------------------
        if (dbInStrFormatIn == NULL || strlen(dbInStrFormatIn) == 0){
		myPerror(F_TABLEA, FF, "Invalid 3rd parameter (db in string format); the string is either null or empty. Returning null");
		return NULL;
	}
	if (idIn > getNumRows(dbInStrFormatIn)){
		myPerror(F_TABLEA, FF, "Invalid parameter 1 (id of file) is greater than the number of rows in csv. Returning null");
		return NULL;
	}
	if (colNumIn > getNumCols(dbInStrFormatIn)){
		myPerror(F_TABLEA, FF, "Invalid parameter 2 (column number) is greater than the number of columns in csv. Returning null");
		return NULL;
	}
	//-----------------------------------------------------------------------------------------
	
	row = getRow(idIn, dbInStrFormatIn);
	if (row == NULL){ myPerror(F_TABLEA, FF, "getRow() returned a null value. Returning null"); return NULL;}
	if (strlen(row) == 0){ 	myPerror(F_TABLEA, FF, "WARNING: getRow() returned an empty string");}

        col = getCol(colNumIn, row);
	if (col == NULL){ myPerror(F_TABLEA, FF, "getCol() returned a null value. Returning null");}

        free(row);
        return col;
}

// This function works just like VLOOKUP in Excel
char* vLookUp(char* lookUpValIn, int colNumToFindValIn, int colIndexToReturnIn, char* filePathOrFileInStrFormatIn){
	// Vars
	char* FF = (char*) __func__;
        char* fileInStrFormat;
        int numRows;
        char* returnStr = NULL;
        int isMatch = 0;
        char* commandOutput;

	// Data Validation-------------------------------------------------------------------------
	if (lookUpValIn == NULL){
		myPerror(F_TABLEA, FF, "Invalid 1st parameter; value is null; Returning null");
		return NULL;
	}
	if (filePathOrFileInStrFormatIn == NULL){
		myPerror(F_TABLEA, FF, "Invalid 4th parameter; value is null; Returning null");
		return NULL;
	}
	//-----------------------------------------------------------------------------------------

	// Determining if filePathOrFileInStrFormatIn is a file path or the file in string format by checking if there are multiple rows
        commandOutput = sedC("sed -n 'N; /\\n/p'", filePathOrFileInStrFormatIn); //can't use regexMatch() function cuz it adds '/' before the 'N' automatically, causing an error
        if (commandOutput == NULL){ myPerror(F_TABLEA, FF, "sedC() function returned null value. Returning null"); return NULL;}

        if (strlen(commandOutput) > 0){ fileInStrFormat = filePathOrFileInStrFormatIn;}
        else{ fileInStrFormat = convertBasic(filePathOrFileInStrFormatIn, "string");}
        free(commandOutput);

	// Data Validation-------------------------------------------------------------------------
	int numCols = getNumCols(fileInStrFormat);
	if (colNumToFindValIn > numCols){
		myPerror(F_TABLEA, FF, "Invalid 2nd parameter; value is greater than the number of columns; Returning null");
		return NULL;
	}
	if (colIndexToReturnIn > numCols){
		myPerror(F_TABLEA, FF, "Invalid 3rd parameter; value is greater than the number of columns; Returning null");
		return NULL;
	}
	//-----------------------------------------------------------------------------------------

	numRows = getNumRows(fileInStrFormat);
	if (numRows == -1){ myPerror(F_TABLEA, FF, "numRows returned an error (value of -1). Returning null"); return NULL;}
        for (int i = 1; i < numRows; i++){
                char* row = getRow(i, fileInStrFormat);
                if (row == NULL){ 
			myPerror(F_TABLEA, FF, "getRow() function returned a null value. Returning null"); 
			free(fileInStrFormat);
			return NULL;
		}
                char* col = getCol(colNumToFindValIn, row);
                if (col == NULL){
			myPerror(F_TABLEA, FF, "getCol() function returned a null value. Returning null");
			free(row); free(fileInStrFormat);
			return NULL;
		}

                if (strcmp(col, lookUpValIn) == 0){
                        returnStr = getCol(colIndexToReturnIn, row);
                        if (returnStr == NULL){
				myPerror(F_TABLEA, FF, "WARNING: Return string is null. A match was found, but getCol() function returned a null value. Returning null");
                                free(row);
				free(col);
				free(fileInStrFormat);
                                return NULL;
                        }

                        i = numRows;
                        isMatch = 1;
                }
                free(row);
                free(col);
        }

        free(fileInStrFormat);
        if (isMatch == 0){
		myPerror(F_TABLEA, FF, "WARNING: No match found. Returning empty string");
                returnStr = createStr();
        }
        return returnStr;
}

// Alternate names for vLookUp
char* vlookup(char* lookUpValIn, int colNumToFindValIn, int colIndexToReturnIn, char* filePathOrFileInStrFormatIn){
	return vLookUp(lookUpValIn, colNumToFindValIn, colIndexToReturnIn, filePathOrFileInStrFormatIn);
}
char* vLookup(char* lookUpValIn, int colNumToFindValIn, int colIndexToReturnIn, char* filePathOrFileInStrFormatIn){
	return vLookUp(lookUpValIn, colNumToFindValIn, colIndexToReturnIn, filePathOrFileInStrFormatIn);
}

char** getTableInfo(char* filePathIn, int colNumIn){
        // Vars
        char* FF = (char*) __func__;
        char* tableColTypeAsStr;
        int numCols = 0;
        char** returnDataTypes;
        int numRowsInTableColType;


        // Data Validation
        if (filePathIn == NULL){ myPerror(F_TABLEA, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
        if (fileExists(filePathIn, 0) == 0){ myPerror(F_TABLEA, FF, "Invalid 1st parameter; file path doesn't exist. Returning null"); return NULL;}

        // Getting Values--------------------------------------------------------------------------
        tableColTypeAsStr = convertBasic(tableColTypeLoc, "string");
        if (tableColTypeAsStr == NULL){ myPerror(F_TABLEA, FF, "readFileSpecial() function returned a null value. Returning null"); return NULL;}

        numRowsInTableColType = getNumRows(tableColTypeAsStr)-1; //subtracting 1 cuz tableColTypeAsString ends with '\n', adding an empty row
        if (numRowsInTableColType < 0){
                free(tableColTypeAsStr);
                myPerror(F_TABLEA, FF, "getNumRows() function returned an error val (<0). Returning null");
                return NULL;
        }
        //-----------------------------------------------------------------------------------------

        // Go through all the rows in tableColTypeList
        for (int i = 1; i < numRowsInTableColType+1; i++){
                // Get the row
                char* currRow = getRow(i, tableColTypeAsStr);
                if (currRow == NULL){
                        myPerror(F_TABLEA, FF, "getRow() function returned a null value. Returning null");
                        myFree(returnDataTypes); free(tableColTypeAsStr);
                        return NULL;
                }
                if (strlen(currRow) == 0){ myPerror(F_TABLEA, FF, "WARNING: getRow() function returned an empty string. Program will continue but errors will likely occur");}

                // Get the file path from that row
                char* filePathCol = getCol(filePathsColTypeListColNum, currRow);
                if (filePathCol == NULL){
                        myPerror(F_TABLEA, FF, "getCol() function returned a null value. Returning null");
                        free(currRow); free(tableColTypeAsStr); myFree(returnDataTypes);
                        return NULL;
                }

                // If the file path at that row == filePathIn
                if (strcmp(filePathCol, filePathIn) == 0){
                        // Find the dataType for that col from the row
                        char* dataType = getCol(colNumIn, currRow);
                        if (dataType == NULL){
                                myPerror(F_TABLEA, FF, "getCol() function returned a null value. Returning null");
                                free(filePathCol); free(currRow); free(tableColTypeAsStr); myFree(returnDataTypes);
                                return NULL;
                        }

                        // Add the row to the string array
                        if (numCols == 0){ returnDataTypes = malloc(sizeof(char*)*2);}
                        else{ returnDataTypes = realloc(returnDataTypes, (numCols+2)*sizeof(char*));} //realloc string array

                        returnDataTypes[numCols] = createStr(strlen(dataType)+1);
                        returnDataTypes[numCols+1] = NULL;
                        strcat(returnDataTypes[numCols], dataType); //add data type to current list

                        //remove newline char if last char has one
                        if (strlen(returnDataTypes[numCols]) > 0 && returnDataTypes[numCols][strlen(returnDataTypes[numCols])-1] == '\n'){
                                returnDataTypes[numCols][strlen(returnDataTypes[numCols])-1] = '\0';
                        }

                        free(dataType);
                        numCols++;
                }

                free(currRow);
                free(filePathCol);
        }

        free(tableColTypeAsStr);
        return returnDataTypes;

}

