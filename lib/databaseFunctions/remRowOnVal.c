#include "symLink_90.sl"
#include "symLink_83.sl"
#include "symLink_85.sl"
#include "symLink_86.sl"
#include "symLink_87.sl"
#include "symLink_88.sl"
#include "symLink_89.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_REMROWVAL = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// BAD DATA TYPES
//=================================================================================================
void* badDataTypeRemRowVal(...){ myPerror(F_REMROWVAL, "remRowVal", "Invalid data type at arg 1. Returning null"); return NULL;}
//=================================================================================================


//=================================================================================================
// CODE
//=================================================================================================

// BEGIN acceptedDbDataTypes
struct acceptedDbDataTypes** removeElementFromDbArrBasedOnVal_1(struct acceptedDbDataTypes** objIn, int colNum, void* val){
	// Vars
	char* FF = (char*) __func__;
	int size;
	int numCols = 2; //this num is auto-updated
	int iAdj = 0;
	int newSize = 0;

	// Data Validation
	if (val == NULL){ myPerror(F_REMROWVAL, FF, "Invalid 2nd parameter (value); value is null. Returning 3rd parameter (database)"); return objIn;}
	if (objIn == NULL){ myPerror(F_REMROWVAL, FF, "Invalid 3rd parameter (database); value is null. Returning null"); return NULL;}

	// Getting Values
	size = getArrSize(objIn);
	if (size < 0){ myPerror(F_REMROWVAL, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
	if (size == 0){ myPerror(F_REMROWVAL, FF, "WARNING: 1st parameter has a size of 0. Returning 1st parameter"); return objIn;}
	if (colNum > numCols - 1){
		myPerror(F_REMROWVAL, FF, "Invalid 1st parameter (id); value is larger than the number of columns: ", numCols, ". Returning 3rd parameter (database)");
		return objIn;
	}

	// Iterating over every row and comparing values based on column num
	for (int i = 0; i < size; i++){
		//since potentially removing rows, the db could be smaller than the original size
		//this conditional statement prevents buffer overflows from that
		if (objIn[iAdj]->id == -1){ return objIn;}
		else if(colNum == 0 && objIn[iAdj]->id == *(typeof(objIn[iAdj]->id)*) val){
			objIn = removeElementFromDbArr_1(objIn, objIn[iAdj]->id);
		}
		else if(colNum == 1 && strcmp(objIn[iAdj]->value, (char*) val) == 0){
			objIn = removeElementFromDbArr_1(objIn, objIn[iAdj]->id);
		}
		else{ iAdj++;}
	}

	newSize = getArrSize(objIn);
	if (newSize < 0){ myPerror(F_REMROWVAL, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
	if (newSize == 0){ myPerror(F_REMROWVAL, FF, "WARNING: All elements removed; struct is empty. Returning object with only sentinel struct");}
	else if (newSize == size){ myPerror(F_REMROWVAL, FF, "No elements removed. Returning parameter 3 (database)");}

	return objIn;
}
// END acceptedDbDataTypes

// BEGIN envVars
struct envVars** removeElementFromDbArrBasedOnVal_2(struct envVars** objIn, int colNum, void* val){
	// Vars
	char* FF = (char*) __func__;
	int size;
	int numCols = 3; //this num is auto-updated
	int iAdj = 0;
	int newSize = 0;

	// Data Validation
	if (val == NULL){ myPerror(F_REMROWVAL, FF, "Invalid 2nd parameter (value); value is null. Returning 3rd parameter (database)"); return objIn;}
	if (objIn == NULL){ myPerror(F_REMROWVAL, FF, "Invalid 3rd parameter (database); value is null. Returning null"); return NULL;}

	// Getting Values
	size = getArrSize(objIn);
	if (size < 0){ myPerror(F_REMROWVAL, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
	if (size == 0){ myPerror(F_REMROWVAL, FF, "WARNING: 1st parameter has a size of 0. Returning 1st parameter"); return objIn;}
	if (colNum > numCols - 1){
		myPerror(F_REMROWVAL, FF, "Invalid 1st parameter (id); value is larger than the number of columns: ", numCols, ". Returning 3rd parameter (database)");
		return objIn;
	}

	// Iterating over every row and comparing values based on column num
	for (int i = 0; i < size; i++){
		//since potentially removing rows, the db could be smaller than the original size
		//this conditional statement prevents buffer overflows from that
		if (objIn[iAdj]->id == -1){ return objIn;}
		else if(colNum == 0 && objIn[iAdj]->id == *(typeof(objIn[iAdj]->id)*) val){
			objIn = removeElementFromDbArr_2(objIn, objIn[iAdj]->id);
		}
		else if(colNum == 1 && strcmp(objIn[iAdj]->fieldName, (char*) val) == 0){
			objIn = removeElementFromDbArr_2(objIn, objIn[iAdj]->id);
		}
		else if(colNum == 2 && strcmp(objIn[iAdj]->fieldVal, (char*) val) == 0){
			objIn = removeElementFromDbArr_2(objIn, objIn[iAdj]->id);
		}
		else{ iAdj++;}
	}

	newSize = getArrSize(objIn);
	if (newSize < 0){ myPerror(F_REMROWVAL, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
	if (newSize == 0){ myPerror(F_REMROWVAL, FF, "WARNING: All elements removed; struct is empty. Returning object with only sentinel struct");}
	else if (newSize == size){ myPerror(F_REMROWVAL, FF, "No elements removed. Returning parameter 3 (database)");}

	return objIn;
}
// END envVars

// BEGIN listProgramsToInstall
struct listProgramsToInstall** removeElementFromDbArrBasedOnVal_3(struct listProgramsToInstall** objIn, int colNum, void* val){
	// Vars
	char* FF = (char*) __func__;
	int size;
	int numCols = 3; //this num is auto-updated
	int iAdj = 0;
	int newSize = 0;

	// Data Validation
	if (val == NULL){ myPerror(F_REMROWVAL, FF, "Invalid 2nd parameter (value); value is null. Returning 3rd parameter (database)"); return objIn;}
	if (objIn == NULL){ myPerror(F_REMROWVAL, FF, "Invalid 3rd parameter (database); value is null. Returning null"); return NULL;}

	// Getting Values
	size = getArrSize(objIn);
	if (size < 0){ myPerror(F_REMROWVAL, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
	if (size == 0){ myPerror(F_REMROWVAL, FF, "WARNING: 1st parameter has a size of 0. Returning 1st parameter"); return objIn;}
	if (colNum > numCols - 1){
		myPerror(F_REMROWVAL, FF, "Invalid 1st parameter (id); value is larger than the number of columns: ", numCols, ". Returning 3rd parameter (database)");
		return objIn;
	}

	// Iterating over every row and comparing values based on column num
	for (int i = 0; i < size; i++){
		//since potentially removing rows, the db could be smaller than the original size
		//this conditional statement prevents buffer overflows from that
		if (objIn[iAdj]->id == -1){ return objIn;}
		else if(colNum == 0 && objIn[iAdj]->id == *(typeof(objIn[iAdj]->id)*) val){
			objIn = removeElementFromDbArr_3(objIn, objIn[iAdj]->id);
		}
		else if(colNum == 1 && strcmp(objIn[iAdj]->packageName, (char*) val) == 0){
			objIn = removeElementFromDbArr_3(objIn, objIn[iAdj]->id);
		}
		else if(colNum == 2 && strcmp(objIn[iAdj]->systemsToInstallOn, (char*) val) == 0){
			objIn = removeElementFromDbArr_3(objIn, objIn[iAdj]->id);
		}
		else{ iAdj++;}
	}

	newSize = getArrSize(objIn);
	if (newSize < 0){ myPerror(F_REMROWVAL, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
	if (newSize == 0){ myPerror(F_REMROWVAL, FF, "WARNING: All elements removed; struct is empty. Returning object with only sentinel struct");}
	else if (newSize == size){ myPerror(F_REMROWVAL, FF, "No elements removed. Returning parameter 3 (database)");}

	return objIn;
}
// END listProgramsToInstall


//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define remRowVal(_1, _2, _3) _Generic((_1), \
		struct acceptedDbDataTypes**: removeElementFromDbArrBasedOnVal_1, \
		struct envVars**: removeElementFromDbArrBasedOnVal_2, \
		struct listProgramsToInstall**: removeElementFromDbArrBasedOnVal_3, \
		default: badDataTypeRemRowVal \
		) (_1, _2, _3) \
*/
