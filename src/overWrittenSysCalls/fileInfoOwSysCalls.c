#include "fileInfoOwSysCalls.h"
#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"
#include "dbValidationSpecial.h"

// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* csvAsStr;
extern int rweColNum;
extern int rmdColNum;

// Error Msg Constant Vars
char* F_FILEINFOOWSYS = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function checks if path is in filesAndFolders.csv
int isTargetPathInCsv(const char* pathIn){
	// Vars
	char* FF = (char*) __func__;

	// Data Validation
	if (pathIn == NULL){ myPerror(F_FILEINFOOWSYS, FF, "Invalid 1st parameter; value is null. Returning -1"); return -1;}
	
	// Get file path column for each row in csv and see if it matches path in
	for (int i = 0; i < numRowsInCsv; i++){
		char* currRow = getRow(i, csvAsStr);
	       	if (currRow == NULL){ myPerror(F_FILEINFOOWSYS, FF, "getRow() function returned a null value. Returning -1"); return -1;}

		char* currFilePath = getCol(filePathsColNum, currRow); //get file path col at the curr row
	       	if (currFilePath == NULL){
			myPerror(F_FILEINFOOWSYS, FF, "getCol() function returned a null value. Returning -1");
			free(currRow);
			return -1;
		}
		
		//compare if filePathIn and current file path are the same. If so, return row num
		if (strcmp(currFilePath, pathIn) == 0){ free(currRow); free(currFilePath); return i;}
		
		free(currRow);
		free(currFilePath);
	}
	return 0;
}

// This function gets either the rwe column or the rmd column permissions from filesAndFolders.csv
// 3rd parameter (rweBool) is 1 for rwe column and 0 for rmd column
int getPerms(const char* pathIn, int rowNum, int rweBool){
	// Vars
	char* FF = (char*) __func__;
	int targetCol;
	char* row;
	char* permsAsStr;
	int permsReturnVal;
	
	// Data Validation
	if (pathIn == NULL){ myPerror(F_FILEINFOOWSYS, FF, "Invalid 1st parameter (path); value is null. Returning -1"); return -1;}
	if (rowNum < 0){ myPerror(F_FILEINFOOWSYS, FF, "Invalid 2nd parameter (rowNum); Value is < 0. Returning -1"); return -1;}
	if (rweBool != 0 && rweBool != 1){ myPerror(F_FILEINFOOWSYS, FF, "Invalid 3rd parameter (rweBool); value is not 0 or 1. Returning -1"); return -1;}

	// Getting Values
	if (rweBool == 0){ targetCol = rweColNum;}
	if (rweBool == 1){ targetCol = rmdColNum;}

	row = getRow(rowNum, csvAsStr);
	if (row == NULL){ myPerror(F_FILEINFOOWSYS, FF, "getRow() function returned a null value. Returning -1"); return -1;}
	
	permsAsStr = getCol(targetCol, row);
	if (permsAsStr == NULL){
		myPerror(F_FILEINFOOWSYS, FF, "getCol() function returned a null value. Returning -1");
		free(row);
		return -1;
	}
	
	// Checking if error with convert() before casting as int
	void* permsReturnValHolder = convert(permsAsStr, "int");
	if (permsReturnValHolder == NULL){ 
		myPerror(F_FILEINFOOWSYS, FF, "convert() returned a null value. Returning -1");
		free(row); free(permsAsStr);
		return -1;
	}
	permsReturnVal = *(int*) permsReturnValHolder;
	
	free(row);
	free(permsAsStr);
	return permsReturnVal;
}
