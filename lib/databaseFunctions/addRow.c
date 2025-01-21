#include "symLink_87.sl"
#include "symLink_83.sl"
#include "symLink_85.sl"
#include "symLink_86.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
static char** memberNames = NULL;
static char** dataTypes = NULL;
int numCols = -1;
int currDbSize = -1;

// Error Msg Constant Vars
char* F_ADDROW = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// BAD DATA TYPE FUNCTIONS
void* badDataTypeAddRow(...){ myPerror(F_ADDROW, "addRow", "Invalid data type at arg 1. Returning null"); return NULL;}
//=================================================================================================


//=================================================================================================
// CODE
//=================================================================================================

// BEGIN acceptedDbDataTypes
struct acceptedDbDataTypes** addElementToDbArr_1(struct acceptedDbDataTypes** dbIn, char* rowIn, int flags){
	// Vars
	char* FF = (char*) __func__;
	struct acceptedDbDataTypes* structToAdd;

	// Data Validation
	if (dbIn == NULL){ myPerror(F_ADDROW, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (rowIn == NULL){ myPerror(F_ADDROW, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}
	if (strlen(rowIn) == 0){ myPerror(F_ADDROW, FF, "Invalid 2nd parameter; value is an empty string. Returning null"); return NULL;}

	// Getting Values------------------------------------------------------------------
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/acceptedDbDataTypes.csv", 3);}
	if (memberNames == NULL){ myPerror(F_ADDROW, FF, "getTableInfo() function returned a null value. Returning null"); return NULL;}

	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/acceptedDbDataTypes.csv", 4);}
	if (dataTypes == NULL){
		myPerror(F_ADDROW, FF, "getTableInfo() function returned a null value. Returning null");
		myFree(memberNames);
		return NULL;
	}

	if (numCols == -1){ numCols = getNumCols(rowIn);}
	if (numCols < 0){
		myPerror(F_ADDROW, FF, "getNumCols() function return an error value (<0). Returning null");
		myFree(dataTypes);
		return NULL;
	}

	currDbSize = getArrSize(dbIn);
	if (currDbSize < 0){
		myPerror(F_ADDROW, FF, "getArrSize() function return an error value (<0). Returning null");
		myFree(dataTypes);
		return NULL;
	}
	if (flags == 1 || flags-10 == 1){ dbIn = realloc(dbIn, sizeof(struct acceptedDbDataTypes*) * (currDbSize+2));}

	structToAdd = malloc(sizeof(struct acceptedDbDataTypes));
	//---------------------------------------------------------------------------------

	// For each col, converting the string value to the appropriate data type and adding to the the struct to be added
	for (int j = 0; j < numCols; j++){
		char* currCol = getCol(j, rowIn);
		void* valAsVoidPtr;
		if (strcmp(dataTypes[j], "string") != 0){
			if (strcmp(dataTypes[j], "bool") == 0){
				valAsVoidPtr = convertBasic(currCol, "int");
				if (valAsVoidPtr == NULL){
					myPerror(F_ADDROW, FF, "convertBasic() function returned a null value. Returning null");
					myFree(memberNames); myFree(dataTypes); myFree(structToAdd); free(currCol); myFree(dbIn);
					return NULL;
				}
			}
			else if (strcmp(dataTypes[j], "char") == 0){
				valAsVoidPtr = &currCol[0];
			}
			else{
				valAsVoidPtr = convertBasic(currCol, dataTypes[j]);
				if (valAsVoidPtr == NULL){
					myPerror(F_ADDROW, FF, "convertBasic() function returned a null value. Returning null");
					myFree(memberNames); myFree(dataTypes); myFree(structToAdd); free(currCol); myFree(dbIn);
					return NULL;
				}
			}
		}
		else{
			char* tempCol = createStr(strlen(currCol)+1);
			strcat(tempCol, currCol);
			valAsVoidPtr = &tempCol;
		}

		int setStructValReturnVal = setStructMemberVal_1(structToAdd, memberNames[j], valAsVoidPtr);
		if (setStructValReturnVal != 0){
			myPerror(F_ADDROW, FF, "setStructMemberVal() function returned an error value (!=0). Returning null");
			myFree(memberNames); myFree(dataTypes); free(currCol);
			return NULL;
		}
		free(currCol);
	}

	free(dbIn[currDbSize]); //freeing the old sentinel value
	dbIn[currDbSize] = structToAdd;
	dbIn[currDbSize+1] = createStruct_1(); //adding the new sentinel value at the end

	if (flags >= 10 && flags < 20){
		myFree(dataTypes); myFree(memberNames);
		numCols = -1; currDbSize = -1;
		creatStruct("free"); //freeing tableColList string arrays
		freeSetStructVal(); //freeing tableColList string arrays
	}

	return dbIn;
}
// END acceptedDbDataTypes

// BEGIN envVars
struct envVars** addElementToDbArr_2(struct envVars** dbIn, char* rowIn, int flags){
	// Vars
	char* FF = (char*) __func__;
	struct envVars* structToAdd;

	// Data Validation
	if (dbIn == NULL){ myPerror(F_ADDROW, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (rowIn == NULL){ myPerror(F_ADDROW, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}
	if (strlen(rowIn) == 0){ myPerror(F_ADDROW, FF, "Invalid 2nd parameter; value is an empty string. Returning null"); return NULL;}

	// Getting Values------------------------------------------------------------------
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/envVars.csv", 3);}
	if (memberNames == NULL){ myPerror(F_ADDROW, FF, "getTableInfo() function returned a null value. Returning null"); return NULL;}

	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/envVars.csv", 4);}
	if (dataTypes == NULL){
		myPerror(F_ADDROW, FF, "getTableInfo() function returned a null value. Returning null");
		myFree(memberNames);
		return NULL;
	}

	if (numCols == -1){ numCols = getNumCols(rowIn);}
	if (numCols < 0){
		myPerror(F_ADDROW, FF, "getNumCols() function return an error value (<0). Returning null");
		myFree(dataTypes);
		return NULL;
	}

	currDbSize = getArrSize(dbIn);
	if (currDbSize < 0){
		myPerror(F_ADDROW, FF, "getArrSize() function return an error value (<0). Returning null");
		myFree(dataTypes);
		return NULL;
	}
	if (flags == 1 || flags-10 == 1){ dbIn = realloc(dbIn, sizeof(struct envVars*) * (currDbSize+2));}

	structToAdd = malloc(sizeof(struct envVars));
	//---------------------------------------------------------------------------------

	// For each col, converting the string value to the appropriate data type and adding to the the struct to be added
	for (int j = 0; j < numCols; j++){
		char* currCol = getCol(j, rowIn);
		void* valAsVoidPtr;
		if (strcmp(dataTypes[j], "string") != 0){
			if (strcmp(dataTypes[j], "bool") == 0){
				valAsVoidPtr = convertBasic(currCol, "int");
				if (valAsVoidPtr == NULL){
					myPerror(F_ADDROW, FF, "convertBasic() function returned a null value. Returning null");
					myFree(memberNames); myFree(dataTypes); myFree(structToAdd); free(currCol); myFree(dbIn);
					return NULL;
				}
			}
			else if (strcmp(dataTypes[j], "char") == 0){
				valAsVoidPtr = &currCol[0];
			}
			else{
				valAsVoidPtr = convertBasic(currCol, dataTypes[j]);
				if (valAsVoidPtr == NULL){
					myPerror(F_ADDROW, FF, "convertBasic() function returned a null value. Returning null");
					myFree(memberNames); myFree(dataTypes); myFree(structToAdd); free(currCol); myFree(dbIn);
					return NULL;
				}
			}
		}
		else{
			char* tempCol = createStr(strlen(currCol)+1);
			strcat(tempCol, currCol);
			valAsVoidPtr = &tempCol;
		}

		int setStructValReturnVal = setStructMemberVal_2(structToAdd, memberNames[j], valAsVoidPtr);
		if (setStructValReturnVal != 0){
			myPerror(F_ADDROW, FF, "setStructMemberVal() function returned an error value (!=0). Returning null");
			myFree(memberNames); myFree(dataTypes); free(currCol);
			return NULL;
		}
		free(currCol);
	}

	free(dbIn[currDbSize]); //freeing the old sentinel value
	dbIn[currDbSize] = structToAdd;
	dbIn[currDbSize+1] = createStruct_2(); //adding the new sentinel value at the end

	if (flags >= 10 && flags < 20){
		myFree(dataTypes); myFree(memberNames);
		numCols = -1; currDbSize = -1;
		creatStruct("free"); //freeing tableColList string arrays
		freeSetStructVal(); //freeing tableColList string arrays
	}

	return dbIn;
}
// END envVars

// BEGIN listProgramsToInstall
struct listProgramsToInstall** addElementToDbArr_3(struct listProgramsToInstall** dbIn, char* rowIn, int flags){
	// Vars
	char* FF = (char*) __func__;
	struct listProgramsToInstall* structToAdd;

	// Data Validation
	if (dbIn == NULL){ myPerror(F_ADDROW, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (rowIn == NULL){ myPerror(F_ADDROW, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}
	if (strlen(rowIn) == 0){ myPerror(F_ADDROW, FF, "Invalid 2nd parameter; value is an empty string. Returning null"); return NULL;}

	// Getting Values------------------------------------------------------------------
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/listProgramsToInstall.csv", 3);}
	if (memberNames == NULL){ myPerror(F_ADDROW, FF, "getTableInfo() function returned a null value. Returning null"); return NULL;}

	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/listProgramsToInstall.csv", 4);}
	if (dataTypes == NULL){
		myPerror(F_ADDROW, FF, "getTableInfo() function returned a null value. Returning null");
		myFree(memberNames);
		return NULL;
	}

	if (numCols == -1){ numCols = getNumCols(rowIn);}
	if (numCols < 0){
		myPerror(F_ADDROW, FF, "getNumCols() function return an error value (<0). Returning null");
		myFree(dataTypes);
		return NULL;
	}

	currDbSize = getArrSize(dbIn);
	if (currDbSize < 0){
		myPerror(F_ADDROW, FF, "getArrSize() function return an error value (<0). Returning null");
		myFree(dataTypes);
		return NULL;
	}
	if (flags == 1 || flags-10 == 1){ dbIn = realloc(dbIn, sizeof(struct listProgramsToInstall*) * (currDbSize+2));}

	structToAdd = malloc(sizeof(struct listProgramsToInstall));
	//---------------------------------------------------------------------------------

	// For each col, converting the string value to the appropriate data type and adding to the the struct to be added
	for (int j = 0; j < numCols; j++){
		char* currCol = getCol(j, rowIn);
		void* valAsVoidPtr;
		if (strcmp(dataTypes[j], "string") != 0){
			if (strcmp(dataTypes[j], "bool") == 0){
				valAsVoidPtr = convertBasic(currCol, "int");
				if (valAsVoidPtr == NULL){
					myPerror(F_ADDROW, FF, "convertBasic() function returned a null value. Returning null");
					myFree(memberNames); myFree(dataTypes); myFree(structToAdd); free(currCol); myFree(dbIn);
					return NULL;
				}
			}
			else if (strcmp(dataTypes[j], "char") == 0){
				valAsVoidPtr = &currCol[0];
			}
			else{
				valAsVoidPtr = convertBasic(currCol, dataTypes[j]);
				if (valAsVoidPtr == NULL){
					myPerror(F_ADDROW, FF, "convertBasic() function returned a null value. Returning null");
					myFree(memberNames); myFree(dataTypes); myFree(structToAdd); free(currCol); myFree(dbIn);
					return NULL;
				}
			}
		}
		else{
			char* tempCol = createStr(strlen(currCol)+1);
			strcat(tempCol, currCol);
			valAsVoidPtr = &tempCol;
		}

		int setStructValReturnVal = setStructMemberVal_3(structToAdd, memberNames[j], valAsVoidPtr);
		if (setStructValReturnVal != 0){
			myPerror(F_ADDROW, FF, "setStructMemberVal() function returned an error value (!=0). Returning null");
			myFree(memberNames); myFree(dataTypes); free(currCol);
			return NULL;
		}
		free(currCol);
	}

	free(dbIn[currDbSize]); //freeing the old sentinel value
	dbIn[currDbSize] = structToAdd;
	dbIn[currDbSize+1] = createStruct_3(); //adding the new sentinel value at the end

	if (flags >= 10 && flags < 20){
		myFree(dataTypes); myFree(memberNames);
		numCols = -1; currDbSize = -1;
		creatStruct("free"); //freeing tableColList string arrays
		freeSetStructVal(); //freeing tableColList string arrays
	}

	return dbIn;
}
// END listProgramsToInstall


//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define addRow(_1, _2, _3) _Generic((_1), \
		struct acceptedDbDataTypes**: addElementToDbArr_1, \
		struct envVars**: addElementToDbArr_2, \
		struct listProgramsToInstall**: addElementToDbArr_3, \
		default: badDataTypeAddRow \
		) (_1, _2, _3) \
*/
