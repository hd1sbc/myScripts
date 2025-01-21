#include "symLink_91.sl"
#include "symLink_83.sl"
#include "symLink_85.sl"
#include "symLink_86.sl"
#include "symLink_87.sl"
#include "symLink_88.sl"
#include "symLink_89.sl"
#include "symLink_90.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_GETROWSWHERE = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// BAD DATA TYPES
//=================================================================================================
void* badDataTypeGetRowsWhere(...){ myPerror(F_GETROWSWHERE, "getRowsWhere", "Invalid data type at arg 1. Returning null"); return NULL;}
//=================================================================================================


//=================================================================================================
// CODE
//=================================================================================================

// BEGIN acceptedDbDataTypes
struct acceptedDbDataTypes** findAllElementsFromDbArrBasedOnVal_1(struct acceptedDbDataTypes** objIn, int colNum, void* val, char* equality){
	// Vars
	char* FF = (char*) __func__;
	int size;
	int numCols = 2;
	int returnArrSize = 0;
	int equ;
	struct acceptedDbDataTypes** returnArr;

	// Data Validation-------------------------------------------------------------------------
	if (val == NULL){ myPerror(F_GETROWSWHERE, FF, "Invalid 2nd parameter (value); value is null. Returning 3rd parameter (database)"); return objIn;}
	if (objIn == NULL){ myPerror(F_GETROWSWHERE, FF, "Invalid 3rd parameter (database); value is null. Returning null"); return NULL;}
	if (equality == NULL){ myPerror(F_GETROWSWHERE, FF, "Invalid 4th parameter (equality value); value is null. Returning null"); return NULL;}
	//-------------------------------------------------------------------------------------------

	// Getting Values--------------------------------------------------------------------------
	size = getArrSize(objIn);
	if (size < 0){ myPerror(F_GETROWSWHERE, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
	if (colNum > numCols){
		myPerror(F_GETROWSWHERE, FF, "Invalid 1st parameter (id); value is larger than the number of columns: ", numCols, ". Returning 3rd parameter (database)");
		return objIn;
	}

	if (strcmp(equality, "==") == 0){ equ = 0;}
	else if (strcmp(equality, "!=") == 0){ equ = 1;}
	else if (strcmp(equality, ">") == 0){ equ = 2;}
	else if (strcmp(equality, "<") == 0){ equ = 3;}
	else if (strcmp(equality, ">=") == 0){ equ = 4;}
	else if (strcmp(equality, "<=") == 0){ equ = 5;}
	else{
		myPerror(F_GETROWSWHERE, FF, "Invalid 4th parameter (equality value); value is not valid. Can either be ==, !=, >, <, >=, or <=. Returning null");
		return NULL;
	}
	returnArr = malloc(sizeof(struct acceptedDbDataTypes*)*(size+1));
	//-------------------------------------------------------------------------------------------

	// Iterating over every row and comparing values based on column num
	for (int i = 0; i < size; i++){
		if (colNum == 0){
			if (equ == 0 && objIn[i]->id == *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_1(objIn[i]); returnArrSize++;
			}
			else if (equ == 1 && objIn[i]->id != *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_1(objIn[i]); returnArrSize++;
			}
			else if (equ == 2 && objIn[i]->id > *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_1(objIn[i]); returnArrSize++;
			}
			else if (equ == 3 && objIn[i]->id < *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_1(objIn[i]); returnArrSize++;
			}
			else if (equ == 4 && objIn[i]->id >= *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_1(objIn[i]); returnArrSize++;
			}
			else if (equ == 5 && objIn[i]->id <= *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_1(objIn[i]); returnArrSize++;
			}
		}
		else if (colNum == 1){
			if (equ == 0 && strcmp(objIn[i]->value, (char*) val) == 0){
				returnArr[returnArrSize] = deepCpyDbObj_1(objIn[i]); returnArrSize++;
			}
			else if (equ == 1 && strcmp(objIn[i]->value, (char*) val) != 0){
				returnArr[returnArrSize] = deepCpyDbObj_1(objIn[i]); returnArrSize++;
			}
			else if (equ == 2 && strcmp(objIn[i]->value, (char*) val) > 0){
				returnArr[returnArrSize] = deepCpyDbObj_1(objIn[i]); returnArrSize++;
			}
			else if (equ == 3 && strcmp(objIn[i]->value, (char*) val) < 0){
				returnArr[returnArrSize] = deepCpyDbObj_1(objIn[i]); returnArrSize++;
			}
			else if (equ == 4 && strcmp(objIn[i]->value, (char*) val) >= 0){
				returnArr[returnArrSize] = deepCpyDbObj_1(objIn[i]); returnArrSize++;
			}
			else if (equ == 5 && strcmp(objIn[i]->value, (char*) val) <= 0){
				returnArr[returnArrSize] = deepCpyDbObj_1(objIn[i]); returnArrSize++;
			}
		}
	}

	if (returnArrSize == 0){ myPerror(F_GETROWSWHERE, FF, "WARNING: No matches found. Returning sentinel struct");}
	returnArr[returnArrSize] = createStruct_1();

	creatStruct("free");
	freeSetStructVal();

	return returnArr;
}
// END acceptedDbDataTypes

// BEGIN envVars
struct envVars** findAllElementsFromDbArrBasedOnVal_2(struct envVars** objIn, int colNum, void* val, char* equality){
	// Vars
	char* FF = (char*) __func__;
	int size;
	int numCols = 3;
	int returnArrSize = 0;
	int equ;
	struct envVars** returnArr;

	// Data Validation-------------------------------------------------------------------------
	if (val == NULL){ myPerror(F_GETROWSWHERE, FF, "Invalid 2nd parameter (value); value is null. Returning 3rd parameter (database)"); return objIn;}
	if (objIn == NULL){ myPerror(F_GETROWSWHERE, FF, "Invalid 3rd parameter (database); value is null. Returning null"); return NULL;}
	if (equality == NULL){ myPerror(F_GETROWSWHERE, FF, "Invalid 4th parameter (equality value); value is null. Returning null"); return NULL;}
	//-------------------------------------------------------------------------------------------

	// Getting Values--------------------------------------------------------------------------
	size = getArrSize(objIn);
	if (size < 0){ myPerror(F_GETROWSWHERE, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
	if (colNum > numCols){
		myPerror(F_GETROWSWHERE, FF, "Invalid 1st parameter (id); value is larger than the number of columns: ", numCols, ". Returning 3rd parameter (database)");
		return objIn;
	}

	if (strcmp(equality, "==") == 0){ equ = 0;}
	else if (strcmp(equality, "!=") == 0){ equ = 1;}
	else if (strcmp(equality, ">") == 0){ equ = 2;}
	else if (strcmp(equality, "<") == 0){ equ = 3;}
	else if (strcmp(equality, ">=") == 0){ equ = 4;}
	else if (strcmp(equality, "<=") == 0){ equ = 5;}
	else{
		myPerror(F_GETROWSWHERE, FF, "Invalid 4th parameter (equality value); value is not valid. Can either be ==, !=, >, <, >=, or <=. Returning null");
		return NULL;
	}
	returnArr = malloc(sizeof(struct envVars*)*(size+1));
	//-------------------------------------------------------------------------------------------

	// Iterating over every row and comparing values based on column num
	for (int i = 0; i < size; i++){
		if (colNum == 0){
			if (equ == 0 && objIn[i]->id == *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 1 && objIn[i]->id != *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 2 && objIn[i]->id > *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 3 && objIn[i]->id < *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 4 && objIn[i]->id >= *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 5 && objIn[i]->id <= *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
		}
		else if (colNum == 1){
			if (equ == 0 && strcmp(objIn[i]->fieldName, (char*) val) == 0){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 1 && strcmp(objIn[i]->fieldName, (char*) val) != 0){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 2 && strcmp(objIn[i]->fieldName, (char*) val) > 0){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 3 && strcmp(objIn[i]->fieldName, (char*) val) < 0){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 4 && strcmp(objIn[i]->fieldName, (char*) val) >= 0){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 5 && strcmp(objIn[i]->fieldName, (char*) val) <= 0){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
		}
		else if (colNum == 2){
			if (equ == 0 && strcmp(objIn[i]->fieldVal, (char*) val) == 0){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 1 && strcmp(objIn[i]->fieldVal, (char*) val) != 0){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 2 && strcmp(objIn[i]->fieldVal, (char*) val) > 0){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 3 && strcmp(objIn[i]->fieldVal, (char*) val) < 0){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 4 && strcmp(objIn[i]->fieldVal, (char*) val) >= 0){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
			else if (equ == 5 && strcmp(objIn[i]->fieldVal, (char*) val) <= 0){
				returnArr[returnArrSize] = deepCpyDbObj_2(objIn[i]); returnArrSize++;
			}
		}
	}

	if (returnArrSize == 0){ myPerror(F_GETROWSWHERE, FF, "WARNING: No matches found. Returning sentinel struct");}
	returnArr[returnArrSize] = createStruct_2();

	creatStruct("free");
	freeSetStructVal();

	return returnArr;
}
// END envVars

// BEGIN listProgramsToInstall
struct listProgramsToInstall** findAllElementsFromDbArrBasedOnVal_3(struct listProgramsToInstall** objIn, int colNum, void* val, char* equality){
	// Vars
	char* FF = (char*) __func__;
	int size;
	int numCols = 3;
	int returnArrSize = 0;
	int equ;
	struct listProgramsToInstall** returnArr;

	// Data Validation-------------------------------------------------------------------------
	if (val == NULL){ myPerror(F_GETROWSWHERE, FF, "Invalid 2nd parameter (value); value is null. Returning 3rd parameter (database)"); return objIn;}
	if (objIn == NULL){ myPerror(F_GETROWSWHERE, FF, "Invalid 3rd parameter (database); value is null. Returning null"); return NULL;}
	if (equality == NULL){ myPerror(F_GETROWSWHERE, FF, "Invalid 4th parameter (equality value); value is null. Returning null"); return NULL;}
	//-------------------------------------------------------------------------------------------

	// Getting Values--------------------------------------------------------------------------
	size = getArrSize(objIn);
	if (size < 0){ myPerror(F_GETROWSWHERE, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
	if (colNum > numCols){
		myPerror(F_GETROWSWHERE, FF, "Invalid 1st parameter (id); value is larger than the number of columns: ", numCols, ". Returning 3rd parameter (database)");
		return objIn;
	}

	if (strcmp(equality, "==") == 0){ equ = 0;}
	else if (strcmp(equality, "!=") == 0){ equ = 1;}
	else if (strcmp(equality, ">") == 0){ equ = 2;}
	else if (strcmp(equality, "<") == 0){ equ = 3;}
	else if (strcmp(equality, ">=") == 0){ equ = 4;}
	else if (strcmp(equality, "<=") == 0){ equ = 5;}
	else{
		myPerror(F_GETROWSWHERE, FF, "Invalid 4th parameter (equality value); value is not valid. Can either be ==, !=, >, <, >=, or <=. Returning null");
		return NULL;
	}
	returnArr = malloc(sizeof(struct listProgramsToInstall*)*(size+1));
	//-------------------------------------------------------------------------------------------

	// Iterating over every row and comparing values based on column num
	for (int i = 0; i < size; i++){
		if (colNum == 0){
			if (equ == 0 && objIn[i]->id == *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 1 && objIn[i]->id != *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 2 && objIn[i]->id > *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 3 && objIn[i]->id < *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 4 && objIn[i]->id >= *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 5 && objIn[i]->id <= *(typeof(objIn[i]->id)*) val){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
		}
		else if (colNum == 1){
			if (equ == 0 && strcmp(objIn[i]->packageName, (char*) val) == 0){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 1 && strcmp(objIn[i]->packageName, (char*) val) != 0){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 2 && strcmp(objIn[i]->packageName, (char*) val) > 0){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 3 && strcmp(objIn[i]->packageName, (char*) val) < 0){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 4 && strcmp(objIn[i]->packageName, (char*) val) >= 0){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 5 && strcmp(objIn[i]->packageName, (char*) val) <= 0){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
		}
		else if (colNum == 2){
			if (equ == 0 && strcmp(objIn[i]->systemsToInstallOn, (char*) val) == 0){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 1 && strcmp(objIn[i]->systemsToInstallOn, (char*) val) != 0){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 2 && strcmp(objIn[i]->systemsToInstallOn, (char*) val) > 0){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 3 && strcmp(objIn[i]->systemsToInstallOn, (char*) val) < 0){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 4 && strcmp(objIn[i]->systemsToInstallOn, (char*) val) >= 0){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
			else if (equ == 5 && strcmp(objIn[i]->systemsToInstallOn, (char*) val) <= 0){
				returnArr[returnArrSize] = deepCpyDbObj_3(objIn[i]); returnArrSize++;
			}
		}
	}

	if (returnArrSize == 0){ myPerror(F_GETROWSWHERE, FF, "WARNING: No matches found. Returning sentinel struct");}
	returnArr[returnArrSize] = createStruct_3();

	creatStruct("free");
	freeSetStructVal();

	return returnArr;
}
// END listProgramsToInstall


//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define getRowsWhere(_1, _2, _3, _4) _Generic((_1), \
		struct acceptedDbDataTypes**: findAllElementsFromDbArrBasedOnVal_1, \
		struct envVars**: findAllElementsFromDbArrBasedOnVal_2, \
		struct listProgramsToInstall**: findAllElementsFromDbArrBasedOnVal_3, \
		default: badDataTypeGetRowsWhere \
		) (_1, _2, _3, _4) \
*/
