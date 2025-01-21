#include "symLink_93.sl"
#include "symLink_76.sl"
#include "symLink_31.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
char* sysFilesAndFoldersListLoc;

// Error Msg Constant Vars
char* F_SYSOBJ = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// INSTANTIATING GLOBAL VARS-----------------------------------------------------------------------
__attribute__((constructor)) void init_SYSOBJ() { sysFilesAndFoldersListLoc = myFindPathC(79); /*filesAndFolders.csv*/}
__attribute__((destructor)) void deinit_SYSOBJ() { free(sysFilesAndFoldersListLoc);}
//-------------------------------------------------------------------------------------------------

// This function finds the file path or name based on id
// 2nd parameter (returnFilePathOrNameBool): 0 - file path, 1 - file name
char* getFilePathOrNameById(int idIn, int returnFilePathOrNameBool){
	// Vars
	char* FF = (char*) __func__;
	int colNum = -1;
	char* dbInStrFormat = NULL;
	char* returnFilePathOrNameBoolAsStr;
	char* filePath;

	// Data Validation-------------------------------------------------------------------------
	if (idIn < 1){ 
		myPerror(F_SYSOBJ, FF, "Invalid 1st parameter (file id); value is < 1. Returning null");
		return NULL;
	}
	returnFilePathOrNameBoolAsStr = convert(returnFilePathOrNameBool, "string");
	if (isBool(returnFilePathOrNameBoolAsStr) == 0){
		free(returnFilePathOrNameBoolAsStr);
		myPerror(F_SYSOBJ, FF, "Invalid 2nd parameter (file name or path boolean); value isn't 0 or 1. Returning null");
		return NULL;
	}
	free(returnFilePathOrNameBoolAsStr);
	//-----------------------------------------------------------------------------------------
	
	if (returnFilePathOrNameBool == 0){ colNum = 3;} //HARDCODED_COL_NUM
	else{ colNum = 4;} //HARDCODED_COL_NUM

	dbInStrFormat = convert(sysFilesAndFoldersListLoc, "string");
	// Data Validation Of dbInStrFormat--------------------------------------------------------
	if (dbInStrFormat == NULL){ 
		myPerror(F_SYSOBJ, FF, "convert() function returned null value. Returning null");
		return NULL;
	}
	if (strlen(dbInStrFormat) == 0){
		myPerror(F_SYSOBJ, FF, "convert() function returned an empty string. Returning null");
		free(dbInStrFormat);
		return NULL;
	}
	//-----------------------------------------------------------------------------------------

	filePath = getColValFromId(idIn, colNum, dbInStrFormat); 
	if (filePath == NULL){ myPerror(F_SYSOBJ, FF, "getColValFromId returned null. Returning null");}

	free(dbInStrFormat);
	return filePath;
}

// This function gets the file path based on ID
// This is a duplicate of getFilePathById. Easier to remember this one, so I added it
char* getFileById(int idIn){
	// Vars
	char* FF = (char*) __func__;
	char* returnStr;
	char* dbAsStr;
	
	// Data Validation
	if (idIn < 1){ myPerror(F_SYSOBJ, FF, "Invalid 1st parameter (file id); value is < 1. Returning null"); return NULL;}
	dbAsStr = convert(sysFilesAndFoldersListLoc, "string");
	if (idIn > getNumRows(dbAsStr)){
		free(dbAsStr);
		myPerror(F_SYSOBJ, FF, "Invalid 1st parameter (file id); value is greater than the number of files in files and folders csv. Returning null");
		return NULL;
	}
	free(dbAsStr);


	returnStr = getFilePathOrNameById(idIn, 0);
	if (returnStr == NULL){ myPerror(F_SYSOBJ, FF, "getFilePathOrNameById returned a null value. Returning null");}

	return returnStr;

}

// Alternative definition for getFileById
char* getFilePathById(int idIn){
	// Vars
	char* FF = (char*) __func__;
	char* returnStr = getFileById(idIn);
	if (returnStr == NULL){ myPerror(F_SYSOBJ, FF, "getFileById() function returned a null value. Returning null");}
	return returnStr;
}

// This function is the same as getFileById, but 2nd param to getFilePathOrNameById = 1 instead of 0
char* getFileNameById(int idIn){
	// Vars
	char* FF = (char*) __func__;
	char* returnStr;
	char* dbAsStr;
	
	// Data Validation
	if (idIn < 1){ myPerror(F_SYSOBJ, FF, "Invalid 1st parameter (file id); value is < 1. Returning null"); return NULL;}
	dbAsStr = convert(sysFilesAndFoldersListLoc, "string");
	if (idIn > getNumRows(dbAsStr)){
		free(dbAsStr);
		myPerror(F_SYSOBJ, FF, "Invalid 1st parameter (file id); value is greater than the number of files in files and folders csv. Returning null");
		return NULL;
	}
	free(dbAsStr);


	returnStr = getFilePathOrNameById(idIn, 1);
	if (returnStr == NULL){ myPerror(F_SYSOBJ, FF, "getFilePathOrNameById returned a null value. Returning null");}

	return returnStr;
}

// This function gets id from file name or path
int getIdFromFile(char* filePathOrNameIn){
	// Vars
	char* FF = (char*) __func__;
	int colNumToFindVal = -1;
	char* adjustedFilePathOrName = createStr(); 
	char* returnIntAsStr;
	int returnInt;

	// Data Validation
	if (filePathOrNameIn == NULL || strlen(filePathOrNameIn) == 0){
		free(adjustedFilePathOrName);
		myPerror(F_SYSOBJ, FF, "Invalid parameter (file path or file name); the value is null or empty. Returning -1");
		return -1;
	}
	
	strcpy(adjustedFilePathOrName, filePathOrNameIn);

	// Determining if file path or file name by checking for '/' in file name
	if (regexMatch("\\/", adjustedFilePathOrName) == 1){
		colNumToFindVal = 3; //HARDCODED_COL_NUM
		// Adjusting file path to remove trailing '/' to match the format in the csv
		if (adjustedFilePathOrName[strlen(adjustedFilePathOrName)-1] == '/'){
			adjustedFilePathOrName[strlen(adjustedFilePathOrName)-1] = '\0';
		}	
	}
	else{ colNumToFindVal = 4;}
	
	
	returnIntAsStr = vLookUp(adjustedFilePathOrName, colNumToFindVal, 0, sysFilesAndFoldersListLoc); //HARDCODED_COL_NUM
	if (returnIntAsStr == NULL){
		myPerror(F_SYSOBJ, FF, "vLookUp() function returned a null value. Returning -1");
		free(adjustedFilePathOrName);
		return -1;
	}

	if (strlen(returnIntAsStr) > 0){ returnInt = *(int*) convert(returnIntAsStr, "int");}
	else {
		myPerror(F_SYSOBJ, FF, "WARNING: Didn't find a match for ", adjustedFilePathOrName, ". Returning -2");
		myPerror(F_SYSOBJ, FF, "	Trailing '/' were already removed from the function's input to match the standard in the files and folders csv");
		returnInt = -2;
	}
	
	free(adjustedFilePathOrName);
	free(returnIntAsStr);
	return returnInt;
}

// This function gets the file path from the files and folders csv based on the file name input
char* getFilePathFromFileName(char* fileNameIn){
	// Vars
	char* FF = (char*) __func__;
	int id;
	char* returnStr;
       	
	id = getIdFromFile(fileNameIn);
	if (id == -1){ 
		myPerror(F_SYSOBJ, FF, "getIdFromFile() function returned an error value (-1). Returning null");
		return NULL;
	}
	if (id == -2){
		myPerror(F_SYSOBJ, FF, "WARNING: getIdFromFile() function didn't find a match. Returning empty string");
		return createStr();
	}

	returnStr = getFileById(id);
	if (returnStr == NULL){ 
		myPerror(F_SYSOBJ, FF, "getFileById() function returned an error. Returning null");
		return NULL;
	}

	return returnStr;
}
