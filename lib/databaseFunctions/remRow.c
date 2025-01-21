#include "symLink_89.sl"
#include "symLink_83.sl"
#include "symLink_85.sl"
#include "symLink_86.sl"
#include "symLink_87.sl"
#include "symLink_88.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_REMROW= (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// BAD DATA TYPES
//=================================================================================================
void* badDataTypeRemRow(...){ myPerror(F_REMROW, "remRow", "Invalid data type at arg 1. Returning null"); return NULL;}
//=================================================================================================


//=================================================================================================
// CODE
//=================================================================================================

// BEGIN acceptedDbDataTypes
struct acceptedDbDataTypes** removeElementFromDbArr_1(struct acceptedDbDataTypes** objIn, int idIn){
	// Vars
	char* FF = (char*) __func__;
	int size;

	// Data Validation
	if (idIn < 0){ myPerror(F_REMROW, FF, "Invalid 1st parameter (id); value is less than 0. Returning 2nd parameter"); return objIn;}
	if (objIn == NULL){ myPerror(F_REMROW, FF, "Invalid 2nd parameter (database); value is null. Returning null"); return NULL;}

	// Getting Values
	size = getArrSize(objIn);
	if (size < 0){ myPerror(F_REMROW, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
	if (idIn > objIn[size-1]->id){
		myPerror(F_REMROW, FF, "Invalid 1st parameter (id); value is larger than last id value: ", objIn[size-1]->id, ". Returning 2nd parameter");
		return objIn;
	}

	// Iterating over array, removing element with matching id and moving objects to next smaller index
	for (int i = 0; i < size; i++){
		if (objIn[i]->id == idIn){
			myFreeDbObjArr_1(1, (void*) objIn[i], 0);
			objIn[i] = objIn[i+1];
		}

		if (objIn[i]->id > idIn){
			objIn[i] = objIn[i+1];
		}
	}
	objIn[size] = NULL;

	return objIn;
}
// END acceptedDbDataTypes

// BEGIN envVars
struct envVars** removeElementFromDbArr_2(struct envVars** objIn, int idIn){
	// Vars
	char* FF = (char*) __func__;
	int size;

	// Data Validation
	if (idIn < 0){ myPerror(F_REMROW, FF, "Invalid 1st parameter (id); value is less than 0. Returning 2nd parameter"); return objIn;}
	if (objIn == NULL){ myPerror(F_REMROW, FF, "Invalid 2nd parameter (database); value is null. Returning null"); return NULL;}

	// Getting Values
	size = getArrSize(objIn);
	if (size < 0){ myPerror(F_REMROW, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
	if (idIn > objIn[size-1]->id){
		myPerror(F_REMROW, FF, "Invalid 1st parameter (id); value is larger than last id value: ", objIn[size-1]->id, ". Returning 2nd parameter");
		return objIn;
	}

	// Iterating over array, removing element with matching id and moving objects to next smaller index
	for (int i = 0; i < size; i++){
		if (objIn[i]->id == idIn){
			myFreeDbObjArr_2(1, (void*) objIn[i], 0);
			objIn[i] = objIn[i+1];
		}

		if (objIn[i]->id > idIn){
			objIn[i] = objIn[i+1];
		}
	}
	objIn[size] = NULL;

	return objIn;
}
// END envVars

// BEGIN listProgramsToInstall
struct listProgramsToInstall** removeElementFromDbArr_3(struct listProgramsToInstall** objIn, int idIn){
	// Vars
	char* FF = (char*) __func__;
	int size;

	// Data Validation
	if (idIn < 0){ myPerror(F_REMROW, FF, "Invalid 1st parameter (id); value is less than 0. Returning 2nd parameter"); return objIn;}
	if (objIn == NULL){ myPerror(F_REMROW, FF, "Invalid 2nd parameter (database); value is null. Returning null"); return NULL;}

	// Getting Values
	size = getArrSize(objIn);
	if (size < 0){ myPerror(F_REMROW, FF, "getArrSize() function returned an error value (<0). Returning null"); return NULL;}
	if (idIn > objIn[size-1]->id){
		myPerror(F_REMROW, FF, "Invalid 1st parameter (id); value is larger than last id value: ", objIn[size-1]->id, ". Returning 2nd parameter");
		return objIn;
	}

	// Iterating over array, removing element with matching id and moving objects to next smaller index
	for (int i = 0; i < size; i++){
		if (objIn[i]->id == idIn){
			myFreeDbObjArr_3(1, (void*) objIn[i], 0);
			objIn[i] = objIn[i+1];
		}

		if (objIn[i]->id > idIn){
			objIn[i] = objIn[i+1];
		}
	}
	objIn[size] = NULL;

	return objIn;
}
// END listProgramsToInstall


//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define remRow(_1, _2) _Generic((_1), \
		struct acceptedDbDataTypes**: removeElementFromDbArr_1, \
		struct envVars**: removeElementFromDbArr_2, \
		struct listProgramsToInstall**: removeElementFromDbArr_3, \
		default: badDataTypeRemRow \
		) (_1, _2) \
*/
