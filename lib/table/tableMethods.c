#include "symLink_49.sl"
#include "symLink_99.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_TABLEM = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function returns the number of rows in a csv-formatted string based on the number of newline characters
// The number of rows includes the header row/column names
int getNumRows(char* fileInStrFormatIn){
	// Vars
	char* FF = (char*) __func__;

	// Data Validation
	if (fileInStrFormatIn == NULL){ myPerror(F_TABLEM, FF, "Input parameter is null. Returning -1"); return -1;}

        int numLines = 1; //starting at 1 cuz the last line doesn't end with a newline char
        for (int i = 0; fileInStrFormatIn[i] != '\0'; i++){ //iterating over string
                if (fileInStrFormatIn[i] == '\n'){ numLines++; } //incrementing every time a new line is created
        }
        return numLines;
}

// This function returns the number of columns in a csv-formatted string of the file or a row in that file
int getNumCols(char* fileInStrFormatIn){
	// Vars
	char* FF = (char*) __func__;
	
	// Data Validation
	if (fileInStrFormatIn == NULL){ myPerror(F_TABLEM, FF, "Input parameter 1 is null. Returning -1"); return -1;}
	
	// Vars
	int numCols = 1; //starting at 1 cuz last col doesn't have a new-column char (',')
	
	for (int i = 0; fileInStrFormatIn[i] != '\0'; i++){
		if (fileInStrFormatIn[i] == '\n'){ return numCols;}
		if (fileInStrFormatIn[i] == ','){ numCols++;}
	}

	return numCols;
}

// This function returns the row index (starting at 0) of a csv-formatted string
char* getRow(int rowNumIn, char* fileInStrFormatIn){
	// Vars
	int arrInc = 128;
        char* returnStr = createStr(arrInc);
        int currRow = 0;
        int returnStrLen = 0;
	char* FF = (char*) __func__;
	
	// Data Validation
	if (rowNumIn < 0){ myPerror(F_TABLEM, FF, "Invalid value for parameter 1. Value is < 0. Returning null"); return NULL;}
	if (fileInStrFormatIn == NULL){ myPerror(F_TABLEM, FF, "Input parameter 2 is null. Returning null"); return NULL;}

	// Iterating over every char, adding each char to a temp variable until end of row
	// If the current row is the row being queried, return that temp variable
        for (int i = 0; fileInStrFormatIn[i] != '\0';i++){
		//if end of row (newline char or end-of-file char) and the curr row is the rowNumIn
                if ((fileInStrFormatIn[i] == '\n' || fileInStrFormatIn[i+1] == '\0') && currRow == rowNumIn){
			//if rowNumIn is last row (index i+1 ends in '\0'), then add the value at i to return string
			if (fileInStrFormatIn[i+1] == '\0'){
				//check if string needs to be realloc'd
	                        if (returnStrLen%arrInc == 0 && returnStrLen != 0){
					returnStr = myRealloc(arrInc, returnStr, returnStrLen);
				}
        	                
				returnStr[returnStrLen] = fileInStrFormatIn[i];
                	        returnStrLen++;
			}

			// Add end-of-string char
                        if (returnStrLen%arrInc == 0){returnStr = myRealloc(1, returnStr, returnStrLen); } //check if string needs to be realloc'd
                        returnStr[returnStrLen] = '\0';
			
			return returnStr;
                }
                else if (fileInStrFormatIn[i] == '\n'){ //if end of row but row doesn't match rowNumIn
                        free(returnStr);
			returnStr = createStr(arrInc); //reset return string
                        currRow++;
                        returnStrLen = 0;
                }
                else { //if not newline char, then add char to return string
                        if (returnStrLen%arrInc == 0 && returnStrLen != 0){ //check if string needs to be realloc'd
				returnStr = myRealloc(arrInc, returnStr, returnStrLen);
			}

			returnStr[returnStrLen] = fileInStrFormatIn[i];
                        returnStrLen++;
                }
        }

        returnStr[returnStrLen] = '\0'; //this is here in case fileInStrFormatIn is an empty string
        return returnStr;
}


// This function returns the value at a given column from an input csv-formatted row (char* data type)
char* getCol(int colNumIn, char* rowIn){
 	// Vars
	int currCol = 0;
        char* currRowStr = createStr(); 
        int currRowStrLen = 0;
	int arrInc = 128;
	char* FF = (char*) __func__;
	
	// Data Validation
	if (colNumIn < 0){
		myPerror(F_TABLEM, FF, "Invalid value for parameter 1. Value is < 0. Returning null");
		free(currRowStr);
		return NULL;
	}
	if (rowIn == NULL){
		myPerror(F_TABLEM, FF, "Input parameter 2 is null. Returning null");
		free(currRowStr);
		return NULL;
	}

	

	// Iterating over every char, adding each char to a temp variable until end of column (comma as delimeter)
	// 	If the curr column is the colNumIn, return that temp variable
        for (int i = 0; rowIn[i] != '\0'; i++){
		//if end of col ("," or end of row) and the curr col is the colNumIn
		if (rowIn[i] == ',' || rowIn[i] == '\n' || rowIn[i+1] == '\0'){
			//if the colNumIn is the last col (index i+1 ends in '\0') and not a comman, then add the value at i to return string
			//if there is a comma followed by end-of-string, that means there is another col after this that is empty
                        if (rowIn[i+1] == '\0' && rowIn[i] != ','){
			       	//check if string needs to be realloc'd
                                if ( (currRowStrLen%arrInc == 0 || (currRowStrLen+1)%arrInc == 0) &&
									strlen(currRowStr) > 0){
					currRowStr = myRealloc(2, currRowStr);
				}
                                
				currRowStr[currRowStrLen] = rowIn[i];
                                currRowStrLen++;
                                currRowStr[currRowStrLen] = '\0'; //add end-of-string char
                                currRowStrLen++;
                        }
			if (currCol == colNumIn){ return currRowStr;} //if current col = colNumIn
			else{ //if not a new row char, then reset return string
                                currRowStrLen = 0;
				currRowStr = resetStr(currRowStr);
                                currCol++;
                        }
		
			//if there is a comman followed by end-of-string char, that means there is another col after currRow that is an empty string
			//this if statement checks for that
			if (currCol == colNumIn && rowIn[i+1] == '\0' && rowIn[i] == ','){ return currRowStr;}
                }
                else{ //if not a new column char, then add char to temp string
			//check if string needs to be realloc'd
                        if ( (currRowStrLen%arrInc == 0 || (currRowStrLen+1)%arrInc == 0) &&
									strlen(currRowStr) > 0){
				currRowStr = myRealloc(currRowStr);
			}
			
                        currRowStr[currRowStrLen] = rowIn[i];
                        currRowStr[currRowStrLen+1] = '\0';
                        currRowStrLen++;
                }

        }
	
	return currRowStr;
}
