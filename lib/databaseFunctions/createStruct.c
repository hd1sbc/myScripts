#include "symLink_86.sl"
#include "symLink_83.sl"
#include "symLink_85.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
static char** memberNames = NULL;
static char** dataTypes = NULL;
static char** defaultVals = NULL;

// Error Msg Constant Vars
char* F_CREATSTRUCT = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------


//=================================================================================================
// BAD DATA TYPES
//=================================================================================================
void* badDataTypeCreatStruct(...){ myPerror(F_CREATSTRUCT, "creatStruct", "Invalid data type at arg 1. Returning null"); return NULL;}
//=================================================================================================


//=================================================================================================
// CODE
//=================================================================================================

// BEGIN acceptedDbDataTypes
struct acceptedDbDataTypes* createStruct_1(){
	// Vars
	char* FF = (char*) __func__;
	struct acceptedDbDataTypes* returnStruct;

	// Getting Values------------------------------------------------------------------
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/acceptedDbDataTypes.csv", 3);}
	if (memberNames == NULL){ myPerror(F_CREATSTRUCT, FF, "getTableInfo() function returned a null value. Returning null"); return NULL;}

	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/acceptedDbDataTypes.csv", 4);}
	if (dataTypes == NULL){
		myPerror(F_CREATSTRUCT, FF, "getTableInfo() function returned a null value. Returning null");
		myFree(memberNames);
		return NULL;
	}

	if (defaultVals == NULL){ defaultVals = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/acceptedDbDataTypes.csv", 5);}
	if (defaultVals == NULL){
		myPerror(F_CREATSTRUCT, FF, "getTableInfo() function returned a null value. Returning null");
		myFree(memberNames); myFree(dataTypes);
		return NULL;
	}

	returnStruct = malloc(sizeof(struct acceptedDbDataTypes));
	//---------------------------------------------------------------------------------

	// Iterative over every member of struct and adding default values
	for (int i = 0; i < getArrSize(memberNames); i++){
		void* valAsVoidPtr;
		if (strcmp(dataTypes[i], "string") != 0){
			if (strcmp(dataTypes[i], "bool") == 0){
				valAsVoidPtr = convertBasic(defaultVals[i], "int");
				if (valAsVoidPtr == NULL){
					myPerror(F_CREATSTRUCT, FF, "convertBasic() function returned a null value. Returning null");
					myFree(memberNames); myFree(dataTypes); myFree(defaultVals); free(returnStruct);
					return NULL;
				}
			}
			else if (strcmp(dataTypes[i], "char") == 0){
				valAsVoidPtr = &defaultVals[i][0];
			}
			else{
				valAsVoidPtr = convertBasic(defaultVals[i], dataTypes[i]);
				if (valAsVoidPtr == NULL){
					myPerror(F_CREATSTRUCT, FF, "convertBasic() function returned a null value. Returning null");
					myFree(memberNames); myFree(dataTypes); myFree(defaultVals); free(returnStruct);
					return NULL;
				}
			}
		}
		else{ valAsVoidPtr = defaultVals[i];}

		int setStructValReturnVal = setStructMemberVal_1(returnStruct, memberNames[i], valAsVoidPtr);
		if (setStructValReturnVal != 0){
			myPerror(F_CREATSTRUCT, FF, "setStructMemberVal() function returned an error value (!=0). Returning null");
			myFree(memberNames); myFree(dataTypes); myFree(defaultVals); myFree(returnStruct);
			return NULL;
		}
	}

	return returnStruct;
}
// END acceptedDbDataTypes

// BEGIN envVars
struct envVars* createStruct_2(){
	// Vars
	char* FF = (char*) __func__;
	struct envVars* returnStruct;

	// Getting Values------------------------------------------------------------------
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/envVars.csv", 3);}
	if (memberNames == NULL){ myPerror(F_CREATSTRUCT, FF, "getTableInfo() function returned a null value. Returning null"); return NULL;}

	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/envVars.csv", 4);}
	if (dataTypes == NULL){
		myPerror(F_CREATSTRUCT, FF, "getTableInfo() function returned a null value. Returning null");
		myFree(memberNames);
		return NULL;
	}

	if (defaultVals == NULL){ defaultVals = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/envVars.csv", 5);}
	if (defaultVals == NULL){
		myPerror(F_CREATSTRUCT, FF, "getTableInfo() function returned a null value. Returning null");
		myFree(memberNames); myFree(dataTypes);
		return NULL;
	}

	returnStruct = malloc(sizeof(struct envVars));
	//---------------------------------------------------------------------------------

	// Iterative over every member of struct and adding default values
	for (int i = 0; i < getArrSize(memberNames); i++){
		void* valAsVoidPtr;
		if (strcmp(dataTypes[i], "string") != 0){
			if (strcmp(dataTypes[i], "bool") == 0){
				valAsVoidPtr = convertBasic(defaultVals[i], "int");
				if (valAsVoidPtr == NULL){
					myPerror(F_CREATSTRUCT, FF, "convertBasic() function returned a null value. Returning null");
					myFree(memberNames); myFree(dataTypes); myFree(defaultVals); free(returnStruct);
					return NULL;
				}
			}
			else if (strcmp(dataTypes[i], "char") == 0){
				valAsVoidPtr = &defaultVals[i][0];
			}
			else{
				valAsVoidPtr = convertBasic(defaultVals[i], dataTypes[i]);
				if (valAsVoidPtr == NULL){
					myPerror(F_CREATSTRUCT, FF, "convertBasic() function returned a null value. Returning null");
					myFree(memberNames); myFree(dataTypes); myFree(defaultVals); free(returnStruct);
					return NULL;
				}
			}
		}
		else{ valAsVoidPtr = defaultVals[i];}

		int setStructValReturnVal = setStructMemberVal_2(returnStruct, memberNames[i], valAsVoidPtr);
		if (setStructValReturnVal != 0){
			myPerror(F_CREATSTRUCT, FF, "setStructMemberVal() function returned an error value (!=0). Returning null");
			myFree(memberNames); myFree(dataTypes); myFree(defaultVals); myFree(returnStruct);
			return NULL;
		}
	}

	return returnStruct;
}
// END envVars

// BEGIN listProgramsToInstall
struct listProgramsToInstall* createStruct_3(){
	// Vars
	char* FF = (char*) __func__;
	struct listProgramsToInstall* returnStruct;

	// Getting Values------------------------------------------------------------------
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/listProgramsToInstall.csv", 3);}
	if (memberNames == NULL){ myPerror(F_CREATSTRUCT, FF, "getTableInfo() function returned a null value. Returning null"); return NULL;}

	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/listProgramsToInstall.csv", 4);}
	if (dataTypes == NULL){
		myPerror(F_CREATSTRUCT, FF, "getTableInfo() function returned a null value. Returning null");
		myFree(memberNames);
		return NULL;
	}

	if (defaultVals == NULL){ defaultVals = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/listProgramsToInstall.csv", 5);}
	if (defaultVals == NULL){
		myPerror(F_CREATSTRUCT, FF, "getTableInfo() function returned a null value. Returning null");
		myFree(memberNames); myFree(dataTypes);
		return NULL;
	}

	returnStruct = malloc(sizeof(struct listProgramsToInstall));
	//---------------------------------------------------------------------------------

	// Iterative over every member of struct and adding default values
	for (int i = 0; i < getArrSize(memberNames); i++){
		void* valAsVoidPtr;
		if (strcmp(dataTypes[i], "string") != 0){
			if (strcmp(dataTypes[i], "bool") == 0){
				valAsVoidPtr = convertBasic(defaultVals[i], "int");
				if (valAsVoidPtr == NULL){
					myPerror(F_CREATSTRUCT, FF, "convertBasic() function returned a null value. Returning null");
					myFree(memberNames); myFree(dataTypes); myFree(defaultVals); free(returnStruct);
					return NULL;
				}
			}
			else if (strcmp(dataTypes[i], "char") == 0){
				valAsVoidPtr = &defaultVals[i][0];
			}
			else{
				valAsVoidPtr = convertBasic(defaultVals[i], dataTypes[i]);
				if (valAsVoidPtr == NULL){
					myPerror(F_CREATSTRUCT, FF, "convertBasic() function returned a null value. Returning null");
					myFree(memberNames); myFree(dataTypes); myFree(defaultVals); free(returnStruct);
					return NULL;
				}
			}
		}
		else{ valAsVoidPtr = defaultVals[i];}

		int setStructValReturnVal = setStructMemberVal_3(returnStruct, memberNames[i], valAsVoidPtr);
		if (setStructValReturnVal != 0){
			myPerror(F_CREATSTRUCT, FF, "setStructMemberVal() function returned an error value (!=0). Returning null");
			myFree(memberNames); myFree(dataTypes); myFree(defaultVals); myFree(returnStruct);
			return NULL;
		}
	}

	return returnStruct;
}
// END listProgramsToInstall


//=================================================================================================


//=================================================================================================
// PRIMARY FUNCTION
//=================================================================================================
void* creatStruct(char* structNameIn){
	// Vars
	char* FF = (char*) __func__;

	// Data Validation
	if (structNameIn == NULL){ myPerror(F_CREATSTRUCT, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}

	if (strcmp(structNameIn, "free") == 0){
		if (memberNames != NULL){ myFree(memberNames);}
		if (dataTypes != NULL){ myFree(dataTypes);}
		if (defaultVals != NULL){ myFree(defaultVals);}
		memberNames = NULL; dataTypes = NULL; defaultVals = NULL;
		return NULL;
	}
	else if (strcmp(structNameIn, "acceptedDbDataTypes") == 0){ return createStruct_1();}
	else if (strcmp(structNameIn, "envVars") == 0){ return createStruct_2();}
	else if (strcmp(structNameIn, "listProgramsToInstall") == 0){ return createStruct_3();}
	else{
		myPerror(F_CREATSTRUCT, FF, "Invalid 1st parameter; value is not a valid struct. Returning null");
		return NULL;
	}
}
//=================================================================================================
