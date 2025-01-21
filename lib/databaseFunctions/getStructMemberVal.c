#include "symLink_84.sl"
#include "symLink_83.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_GETSTRUCTVAL = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------


//=================================================================================================
// BAD DATA TYPES
//=================================================================================================
void* badDataTypeGetStructMemVal(...){ myPerror(F_GETSTRUCTVAL, "getStructVal", "Invalid data type at arg 1. Returning null"); return NULL;}
//=================================================================================================


//=================================================================================================
// CODE
//=================================================================================================

// BEGIN acceptedDbDataTypes
void* getStructMemberVal_1(struct acceptedDbDataTypes* structIn, char* memberNameIn){
	// Vars
	char* FF = (char*) __func__;
	void* returnVal;

	// Data Validation
	if (structIn == NULL){ myPerror(F_GETSTRUCTVAL, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (memberNameIn == NULL){ myPerror(F_GETSTRUCTVAL, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}

	// Getting values for each member of the struct by matching member name
	// C is a statically-typed language so I can't pass the member name stored in a variable
	if (strcmp(memberNameIn, "id") == 0){ returnVal = (void*) &structIn->id;}
	else if (strcmp(memberNameIn, "value") == 0){ returnVal = (void*) &structIn->value;}
	else {
		myPerror(F_GETSTRUCTVAL, FF, "Invalid 2nd parameter; member name is not found in struct. Returning null");
		return NULL;
	}

	return returnVal;
}
// END acceptedDbDataTypes

// BEGIN envVars
void* getStructMemberVal_2(struct envVars* structIn, char* memberNameIn){
	// Vars
	char* FF = (char*) __func__;
	void* returnVal;

	// Data Validation
	if (structIn == NULL){ myPerror(F_GETSTRUCTVAL, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (memberNameIn == NULL){ myPerror(F_GETSTRUCTVAL, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}

	// Getting values for each member of the struct by matching member name
	// C is a statically-typed language so I can't pass the member name stored in a variable
	if (strcmp(memberNameIn, "id") == 0){ returnVal = (void*) &structIn->id;}
	else if (strcmp(memberNameIn, "fieldName") == 0){ returnVal = (void*) &structIn->fieldName;}
	else if (strcmp(memberNameIn, "fieldVal") == 0){ returnVal = (void*) &structIn->fieldVal;}
	else {
		myPerror(F_GETSTRUCTVAL, FF, "Invalid 2nd parameter; member name is not found in struct. Returning null");
		return NULL;
	}

	return returnVal;
}
// END envVars

// BEGIN listProgramsToInstall
void* getStructMemberVal_3(struct listProgramsToInstall* structIn, char* memberNameIn){
	// Vars
	char* FF = (char*) __func__;
	void* returnVal;

	// Data Validation
	if (structIn == NULL){ myPerror(F_GETSTRUCTVAL, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (memberNameIn == NULL){ myPerror(F_GETSTRUCTVAL, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}

	// Getting values for each member of the struct by matching member name
	// C is a statically-typed language so I can't pass the member name stored in a variable
	if (strcmp(memberNameIn, "id") == 0){ returnVal = (void*) &structIn->id;}
	else if (strcmp(memberNameIn, "packageName") == 0){ returnVal = (void*) &structIn->packageName;}
	else if (strcmp(memberNameIn, "systemsToInstallOn") == 0){ returnVal = (void*) &structIn->systemsToInstallOn;}
	else {
		myPerror(F_GETSTRUCTVAL, FF, "Invalid 2nd parameter; member name is not found in struct. Returning null");
		return NULL;
	}

	return returnVal;
}
// END listProgramsToInstall


//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define getStructVal(_1, _2) _Generic((_1), \
		struct acceptedDbDataTypes*: getStructMemberVal_1, \
		struct envVars*: getStructMemberVal_2, \
		struct listProgramsToInstall*: getStructMemberVal_3, \
		default: badDataTypeGetStructMemVal \
		) (_1, _2) \
*/
