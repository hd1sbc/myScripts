#include "symLink_85.sl"
#include "symLink_83.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
static char** memberNames = NULL;
static char** dataTypes = NULL;

// Error Msg Constant Vars
char* F_SETSTRUCTVAL = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------


//=================================================================================================
// BAD DATA TYPES AND FREE GLOBAL VARS
//=================================================================================================
void* badDataTypeSetStructMemVal(...){ myPerror(F_SETSTRUCTVAL, "setStructVal", "Invalid data type at arg 1. Returning null"); return NULL;}
void freeSetStructVal(){
	if (memberNames != NULL){ myFree(memberNames);}
	if (dataTypes != NULL){ myFree(dataTypes);}
	memberNames = NULL; dataTypes = NULL;
}
//=================================================================================================


//=================================================================================================
// CODE
//=================================================================================================

// BEGIN acceptedDbDataTypes
int setStructMemberVal_1(struct acceptedDbDataTypes* structIn, char* memberNameIn, void* valueIn){
	// Vars
	char* FF = (char*) __func__;

	// Data Validation-----------------------------------------------------------------
	if (structIn == NULL){ myPerror(F_SETSTRUCTVAL, FF, "Invalid 1st parameter; value is null. Returning -1"); return -1;}
	if (memberNameIn == NULL){ myPerror(F_SETSTRUCTVAL, FF, "Invalid 2nd parameter; value is null. Returning -1"); return -1;}
	if (strlen(memberNameIn) == 0){ myPerror(F_SETSTRUCTVAL, FF, "Invalid 2nd parameter; value is an empty string. Returning -1"); return -1;}
	if (valueIn == NULL){ myPerror(F_SETSTRUCTVAL, FF, "Invalid 3rd parameter; value is null. Returning -1"); return -1;}
	//---------------------------------------------------------------------------------

	// Getting Values
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/acceptedDbDataTypes.csv", 3);}
	if (memberNames == NULL){ myPerror(F_SETSTRUCTVAL, FF, "getTableInfo() function returned a null value. Returning -1"); return -1;}

	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/acceptedDbDataTypes.csv", 4);}
	if (dataTypes == NULL){ myPerror(F_SETSTRUCTVAL, FF, "getTableInfo() function returned a null value. Returning -1"); myFree(memberNames); return -1;}

	// Getting values for each member of the struct by matching member name
	// C is a statically-typed language so I can't pass the member name stored in a variable
	if (strcmp(memberNameIn, "id") == 0){ structIn->id = *(typeof(structIn->id)*) valueIn;}
	else if (strcmp(memberNameIn, "value") == 0){ structIn->value = *(typeof(structIn->value)*) valueIn;}
	else {
		myPerror(F_SETSTRUCTVAL, FF, "Invalid 2nd parameter; member name ", memberNameIn, " is not found in struct. Returning -1");
		myFree(memberNames); myFree(dataTypes);
		return -1;
	}

	return 0;
}
// END acceptedDbDataTypes

// BEGIN envVars
int setStructMemberVal_2(struct envVars* structIn, char* memberNameIn, void* valueIn){
	// Vars
	char* FF = (char*) __func__;

	// Data Validation-----------------------------------------------------------------
	if (structIn == NULL){ myPerror(F_SETSTRUCTVAL, FF, "Invalid 1st parameter; value is null. Returning -1"); return -1;}
	if (memberNameIn == NULL){ myPerror(F_SETSTRUCTVAL, FF, "Invalid 2nd parameter; value is null. Returning -1"); return -1;}
	if (strlen(memberNameIn) == 0){ myPerror(F_SETSTRUCTVAL, FF, "Invalid 2nd parameter; value is an empty string. Returning -1"); return -1;}
	if (valueIn == NULL){ myPerror(F_SETSTRUCTVAL, FF, "Invalid 3rd parameter; value is null. Returning -1"); return -1;}
	//---------------------------------------------------------------------------------

	// Getting Values
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/envVars.csv", 3);}
	if (memberNames == NULL){ myPerror(F_SETSTRUCTVAL, FF, "getTableInfo() function returned a null value. Returning -1"); return -1;}

	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/envVars.csv", 4);}
	if (dataTypes == NULL){ myPerror(F_SETSTRUCTVAL, FF, "getTableInfo() function returned a null value. Returning -1"); myFree(memberNames); return -1;}

	// Getting values for each member of the struct by matching member name
	// C is a statically-typed language so I can't pass the member name stored in a variable
	if (strcmp(memberNameIn, "id") == 0){ structIn->id = *(typeof(structIn->id)*) valueIn;}
	else if (strcmp(memberNameIn, "fieldName") == 0){ structIn->fieldName = *(typeof(structIn->fieldName)*) valueIn;}
	else if (strcmp(memberNameIn, "fieldVal") == 0){ structIn->fieldVal = *(typeof(structIn->fieldVal)*) valueIn;}
	else {
		myPerror(F_SETSTRUCTVAL, FF, "Invalid 2nd parameter; member name ", memberNameIn, " is not found in struct. Returning -1");
		myFree(memberNames); myFree(dataTypes);
		return -1;
	}

	return 0;
}
// END envVars

// BEGIN listProgramsToInstall
int setStructMemberVal_3(struct listProgramsToInstall* structIn, char* memberNameIn, void* valueIn){
	// Vars
	char* FF = (char*) __func__;

	// Data Validation-----------------------------------------------------------------
	if (structIn == NULL){ myPerror(F_SETSTRUCTVAL, FF, "Invalid 1st parameter; value is null. Returning -1"); return -1;}
	if (memberNameIn == NULL){ myPerror(F_SETSTRUCTVAL, FF, "Invalid 2nd parameter; value is null. Returning -1"); return -1;}
	if (strlen(memberNameIn) == 0){ myPerror(F_SETSTRUCTVAL, FF, "Invalid 2nd parameter; value is an empty string. Returning -1"); return -1;}
	if (valueIn == NULL){ myPerror(F_SETSTRUCTVAL, FF, "Invalid 3rd parameter; value is null. Returning -1"); return -1;}
	//---------------------------------------------------------------------------------

	// Getting Values
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/listProgramsToInstall.csv", 3);}
	if (memberNames == NULL){ myPerror(F_SETSTRUCTVAL, FF, "getTableInfo() function returned a null value. Returning -1"); return -1;}

	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/listProgramsToInstall.csv", 4);}
	if (dataTypes == NULL){ myPerror(F_SETSTRUCTVAL, FF, "getTableInfo() function returned a null value. Returning -1"); myFree(memberNames); return -1;}

	// Getting values for each member of the struct by matching member name
	// C is a statically-typed language so I can't pass the member name stored in a variable
	if (strcmp(memberNameIn, "id") == 0){ structIn->id = *(typeof(structIn->id)*) valueIn;}
	else if (strcmp(memberNameIn, "packageName") == 0){ structIn->packageName = *(typeof(structIn->packageName)*) valueIn;}
	else if (strcmp(memberNameIn, "systemsToInstallOn") == 0){ structIn->systemsToInstallOn = *(typeof(structIn->systemsToInstallOn)*) valueIn;}
	else {
		myPerror(F_SETSTRUCTVAL, FF, "Invalid 2nd parameter; member name ", memberNameIn, " is not found in struct. Returning -1");
		myFree(memberNames); myFree(dataTypes);
		return -1;
	}

	return 0;
}
// END listProgramsToInstall


//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define setStructVal(_1, _2) _Generic((_1), \
		struct acceptedDbDataTypes*: setStructMemberVal_1, \
		struct envVars*: setStructMemberVal_2, \
		struct listProgramsToInstall*: setStructMemberVal_3, \
		default: badDataTypeSetStructMemVal \
		) (_1, _2) \
*/
