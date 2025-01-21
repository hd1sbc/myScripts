#include "symLink_39.sl"
#include "symLink_30.sl"
#include "symLink_48.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_GETARRSIZE = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// PRINT STATEMENTS - ERRORS
//=================================================================================================
int wrongDataTypeGetArrSize(...){
	fprintf(stderr, "%s%s%s", "===", F_GETARRSIZE, "===	Invalid data type at parameter position 1. Accepted data types are:\n");
	fprintf(stderr, "%s%s%s", "===", F_GETARRSIZE, "===		'char*', 'char**', and 'filesAndFolderDbObj**'\n"); 
	return -1;
}

int nullInput(){ fprintf(stderr, "%s%s%s", "===", F_GETARRSIZE, "===	Invalid input - pointer is NULL\n"); return -1;}
//=================================================================================================


//=================================================================================================
// CODE
//=================================================================================================
int strGetArrSize(char* strIn){
	int size = 0;
	if (strIn == NULL){ return nullInput();} //data validation
	while (strIn[size] != '\0'){ size++;}
	return size;
}

int strArrGetArrSize(char** strArrIn){
	int size = 0;
	if (strArrIn == NULL){ return nullInput();} //data validation
	while (strArrIn[size] != NULL){ size++;}
	return size;
}

int filesAndFoldersDbObjArrGetArrSize(struct filesAndFoldersDbObj** objIn){
	int size = 0;
	if (objIn == NULL){ return nullInput();} //data validation
	while (objIn[size]->id != -1){ size++;} //id set to -1 is sentinel value
	return size;
}
// BEGIN acceptedDbDataTypes
int acceptedDbDataTypesGetArrSize(struct acceptedDbDataTypes** objIn){
	int size = 0;
	if (objIn == NULL){ return nullInput();} //data validation
	while (objIn[size]->id != -1){ size++;}
	return size;
}
// END acceptedDbDataTypes
// BEGIN envVars
int envVarsGetArrSize(struct envVars** objIn){
	int size = 0;
	if (objIn == NULL){ return nullInput();} //data validation
	while (objIn[size]->id != -1){ size++;}
	return size;
}
// END envVars
// BEGIN listProgramsToInstall
int listProgramsToInstallGetArrSize(struct listProgramsToInstall** objIn){
	int size = 0;
	if (objIn == NULL){ return nullInput();} //data validation
	while (objIn[size]->id != -1){ size++;}
	return size;
}
// END listProgramsToInstall


//=================================================================================================



//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define getArrSize(_1) _Generic((_1), \
		char*: strGetArrSize, \
		char**: strArrGetArrSize, \
		struct filesAndFoldersDbObj**: filesAndFoldersDbObjArrGetArrSize, \
		default: wrongDataTypeGetArrSize \
		)(_1)

*/
//=================================================================================================
