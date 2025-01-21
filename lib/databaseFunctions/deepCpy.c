#include "symLink_88.sl"
#include "symLink_83.sl"
#include "symLink_85.sl"
#include "symLink_86.sl"
#include "symLink_87.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_DEEPCPY = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------


//=================================================================================================
// BAD DATA TYPES
//=================================================================================================
void* badDataTypeDeepCpy(...){ myPerror(F_DEEPCPY, "deepCpy", "Invalid data type at arg 1. Returning null"); return NULL;}
//=================================================================================================


//=================================================================================================
// CODE
//=================================================================================================

// BEGIN acceptedDbDataTypes
struct acceptedDbDataTypes* deepCpyDbObj_1(struct acceptedDbDataTypes* objToCopy){
	// Vars
	char* FF = (char*) __func__;
	struct acceptedDbDataTypes* copy;

	// Data Validation
	if (objToCopy == NULL){ myPerror(F_DEEPCPY, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}

	// Getting Values
	copy = malloc(sizeof(struct acceptedDbDataTypes));

	copy->id = objToCopy->id;
	copy->value = (char*) calloc(strlen(objToCopy->value)+1, sizeof(char));
	strcpy(copy->value, objToCopy->value);

	return copy;
}
// END acceptedDbDataTypes

// BEGIN acceptedDbDataTypes
struct acceptedDbDataTypes** deepCpyDbObjArr_1(struct acceptedDbDataTypes** objToCopy){
	// Vars
	char* FF = (char*) __func__;
	int size;
	struct acceptedDbDataTypes** returnObjArr;

	// Data Validation
	if (objToCopy == NULL){ myPerror(F_DEEPCPY, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}

	// Getting Values
	size = acceptedDbDataTypesGetArrSize(objToCopy);
	returnObjArr = malloc(sizeof(struct acceptedDbDataTypes*)*(size+1));

	// Deep copying each object in the array
	for (int i = 0; i < size; i++){
		returnObjArr[i] = deepCpyDbObj_1(objToCopy[i]);
	}
	returnObjArr[size] = createStruct_1(); //adding sentinal struct

	// Freeing colTypeList string arrays
	creatStruct("free");
	freeSetStructVal();

	return returnObjArr;
}
// END acceptedDbDataTypes

// BEGIN envVars
struct envVars* deepCpyDbObj_2(struct envVars* objToCopy){
	// Vars
	char* FF = (char*) __func__;
	struct envVars* copy;

	// Data Validation
	if (objToCopy == NULL){ myPerror(F_DEEPCPY, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}

	// Getting Values
	copy = malloc(sizeof(struct envVars));

	copy->id = objToCopy->id;
	copy->fieldName = (char*) calloc(strlen(objToCopy->fieldName)+1, sizeof(char));
	strcpy(copy->fieldName, objToCopy->fieldName);
	copy->fieldVal = (char*) calloc(strlen(objToCopy->fieldVal)+1, sizeof(char));
	strcpy(copy->fieldVal, objToCopy->fieldVal);

	return copy;
}
// END envVars

// BEGIN envVars
struct envVars** deepCpyDbObjArr_2(struct envVars** objToCopy){
	// Vars
	char* FF = (char*) __func__;
	int size;
	struct envVars** returnObjArr;

	// Data Validation
	if (objToCopy == NULL){ myPerror(F_DEEPCPY, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}

	// Getting Values
	size = envVarsGetArrSize(objToCopy);
	returnObjArr = malloc(sizeof(struct envVars*)*(size+1));

	// Deep copying each object in the array
	for (int i = 0; i < size; i++){
		returnObjArr[i] = deepCpyDbObj_2(objToCopy[i]);
	}
	returnObjArr[size] = createStruct_2(); //adding sentinal struct

	// Freeing colTypeList string arrays
	creatStruct("free");
	freeSetStructVal();

	return returnObjArr;
}
// END envVars

// BEGIN listProgramsToInstall
struct listProgramsToInstall* deepCpyDbObj_3(struct listProgramsToInstall* objToCopy){
	// Vars
	char* FF = (char*) __func__;
	struct listProgramsToInstall* copy;

	// Data Validation
	if (objToCopy == NULL){ myPerror(F_DEEPCPY, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}

	// Getting Values
	copy = malloc(sizeof(struct listProgramsToInstall));

	copy->id = objToCopy->id;
	copy->packageName = (char*) calloc(strlen(objToCopy->packageName)+1, sizeof(char));
	strcpy(copy->packageName, objToCopy->packageName);
	copy->systemsToInstallOn = (char*) calloc(strlen(objToCopy->systemsToInstallOn)+1, sizeof(char));
	strcpy(copy->systemsToInstallOn, objToCopy->systemsToInstallOn);

	return copy;
}
// END listProgramsToInstall

// BEGIN listProgramsToInstall
struct listProgramsToInstall** deepCpyDbObjArr_3(struct listProgramsToInstall** objToCopy){
	// Vars
	char* FF = (char*) __func__;
	int size;
	struct listProgramsToInstall** returnObjArr;

	// Data Validation
	if (objToCopy == NULL){ myPerror(F_DEEPCPY, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}

	// Getting Values
	size = listProgramsToInstallGetArrSize(objToCopy);
	returnObjArr = malloc(sizeof(struct listProgramsToInstall*)*(size+1));

	// Deep copying each object in the array
	for (int i = 0; i < size; i++){
		returnObjArr[i] = deepCpyDbObj_3(objToCopy[i]);
	}
	returnObjArr[size] = createStruct_3(); //adding sentinal struct

	// Freeing colTypeList string arrays
	creatStruct("free");
	freeSetStructVal();

	return returnObjArr;
}
// END listProgramsToInstall


//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define deepCpy(_1) _Generic((_1), \
		struct acceptedDbDataTypes*: deepCpyDbObj_1, \
		struct acceptedDbDataTypes**: deepCpyDbObjArr_1, \
		struct envVars*: deepCpyDbObj_2, \
		struct envVars**: deepCpyDbObjArr_2, \
		struct listProgramsToInstall*: deepCpyDbObj_3, \
		struct listProgramsToInstall**: deepCpyDbObjArr_3, \
		default: badDataTypeDeepCpy \
		) (_1) \
*/
