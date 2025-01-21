#include "symLink_42.sl"
#include "symLink_30.sl"
#include "symLink_29.sl"
#include "symLink_48.sl"
#include "symLink_46.sl"

// INPUT STRUCTURE---------------------------------------------------------------------------------
// <datatype* arr>, [int printBool], [char* dataType]
//-------------------------------------------------------------------------------------------------

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_MYFREE = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// CODE
//=================================================================================================

// This function is the recursion function for char-based arrays
void myFreeChar(int numPtrsIn, void* arrIn, int printBoolIn){
	if (numPtrsIn == 1){ 
		char* strPtr = (char*) arrIn;
		if (printBoolIn > 0){ printf("%s\n", strPtr);}
		if (printBoolIn != 2 && strPtr != NULL){free(strPtr);}
	}

	// Recursion
	// Have to separate by pointers because I need to convert arrIn to the correct data type
	else {
		if (numPtrsIn == 2){				
			char** ptr = (char**) arrIn;
			for (int i = 0; ptr[i] != NULL; i++){
				myFreeChar(numPtrsIn-1, (void*) ptr[i], printBoolIn);
			}
			if (printBoolIn != 2){free(ptr);}
		}
		if (numPtrsIn == 3){ 
			char*** ptr = (char***) arrIn;
			for (int i = 0; ptr[i] != NULL; i++){
				myFreeChar(numPtrsIn-1, (void*) ptr[i], printBoolIn);
			}
			if (printBoolIn != 2){free(ptr);}
		}
		if (numPtrsIn == 4){
			char**** ptr = (char****) arrIn;
			for (int i = 0; ptr[i] != NULL; i++){
				myFreeChar(numPtrsIn-1, (void*) ptr[i], printBoolIn);
			}
			if (printBoolIn != 2){free(ptr);}
		}
		if (numPtrsIn == 5){ 
			char***** ptr = (char*****) arrIn;
			for (int i = 0; ptr[i] != NULL; i++){ 
				myFreeChar(numPtrsIn-1, (void*) ptr[i], printBoolIn);
			}
			if (printBoolIn != 2){free(ptr);}
		}
		
	}

}

// This function is the recursion function for filesAndFolders database object arrays
void myFreeFilesAndFoldersDbObj(int numPtrsIn, void* arrIn, int printBoolIn){
	// Recursion base case
	if (numPtrsIn == 1){ 
		struct filesAndFoldersDbObj* objPtr = (struct filesAndFoldersDbObj*) arrIn;
		if (printBoolIn > 0 && objPtr->id != -1){ 
			printFilesDbObj(objPtr);
		}
		if (printBoolIn != 2){ //freeing each dynamically alloc'd element in the struct and the pointer
			if (objPtr->fullPath != NULL){free(objPtr->fullPath);}
			if (objPtr->fileName != NULL){free(objPtr->fileName);}
			if (objPtr->extension != NULL){free(objPtr->extension);}
			if (objPtr->pathInRepo != NULL){free(objPtr->pathInRepo);}
			if (objPtr->oldPath != NULL){free(objPtr->oldPath);}
			
			free(objPtr); //free the pointer
		}
	}

	// Recursion
	else {
		if (numPtrsIn == 2){		
			struct filesAndFoldersDbObj** ptr = (struct filesAndFoldersDbObj**) arrIn;
			if (printBoolIn != 0){ printHeaderRowFilesAndFolders();}
			int i = 0;
			for (; ptr[i]->id != -1; i++){
				myFreeFilesAndFoldersDbObj(numPtrsIn-1, (void*) ptr[i], printBoolIn);
			}
			
			//need to free the sentinel struct since it isn't part of the for loop
			//the values of the default sentinel struct don't need to freed cuz they weren't dynamically allocated, unlike the other structs in the array
			if (printBoolIn != 2){ free(ptr[i]); free(ptr);}
		}
		if (numPtrsIn == 3){ 
			struct filesAndFoldersDbObj*** ptr = (struct filesAndFoldersDbObj***) arrIn;
			int i = 0;
			for (; ptr[i][0]->id != -1; i++){
				myFreeFilesAndFoldersDbObj(numPtrsIn-1, (void*) ptr[i], printBoolIn);
			}

			//need to free the sentinel struct since it isn't part of the for loop
			//the values of the default sentinel struct don't need to freed cuz they weren't dynamically allocated, unlike the other structs in the array
			if (printBoolIn != 2){ free(ptr[i]); free(ptr);}
		}
	}
}

// BEGIN acceptedDbDataTypes
int myFreeDbObjArr_1(int numPtrsIn, void* arrIn, int printBoolIn){
	// Recursion base case
	if (numPtrsIn == 1){
		struct acceptedDbDataTypes* objPtr = (struct acceptedDbDataTypes*) arrIn;
		if (printBoolIn > 0 && objPtr->id != -1){ printTable(objPtr);}
		if (printBoolIn != 2){ //freeing each dynamically alloc'd element in the struct and the pointer
			if (objPtr->value != NULL){free(objPtr->value);}

			free(objPtr); //free the pointer
		}
	}

	// Recursion
	else {
		if (numPtrsIn == 2){
			struct acceptedDbDataTypes** ptr = (struct acceptedDbDataTypes**) arrIn;
			if (printBoolIn != 0){ printHeaderRowFilesAndFolders();}
			int i = 0;
			for (; ptr[i]->id != -1; i++){ myFreeDbObjArr_1(numPtrsIn-1, (void*) ptr[i], printBoolIn);}

			//need to free the sentinel struct since it isn't part of the for loop
			//the values of the default sentinel struct don't need to freed cuz they weren't dynamically allocated, unlike the other structs in the array
			if (printBoolIn != 2){ free(ptr[i]); free(ptr);}
		}
		if (numPtrsIn == 3){
			struct acceptedDbDataTypes*** ptr = (struct acceptedDbDataTypes***) arrIn;
			int i = 0;
			for (; ptr[i][0]->id != -1; i++){ myFreeDbObjArr_1(numPtrsIn-1, (void*) ptr[i], printBoolIn);}
			//need to free the sentinel struct since it isn't part of the for loop
			//the values of the default sentinel struct don't need to freed cuz they weren't dynamically allocated, unlike the other structs in the array
			if (printBoolIn != 2){ free(ptr[i]); free(ptr);}
		}
	}

	return 0;
}
// END acceptedDbDataTypes

// BEGIN envVars
int myFreeDbObjArr_2(int numPtrsIn, void* arrIn, int printBoolIn){
	// Recursion base case
	if (numPtrsIn == 1){
		struct envVars* objPtr = (struct envVars*) arrIn;
		if (printBoolIn > 0 && objPtr->id != -1){ printTable(objPtr);}
		if (printBoolIn != 2){ //freeing each dynamically alloc'd element in the struct and the pointer
			if (objPtr->fieldName != NULL){free(objPtr->fieldName);}
			if (objPtr->fieldVal != NULL){free(objPtr->fieldVal);}

			free(objPtr); //free the pointer
		}
	}

	// Recursion
	else {
		if (numPtrsIn == 2){
			struct envVars** ptr = (struct envVars**) arrIn;
			if (printBoolIn != 0){ printHeaderRowFilesAndFolders();}
			int i = 0;
			for (; ptr[i]->id != -1; i++){ myFreeDbObjArr_2(numPtrsIn-1, (void*) ptr[i], printBoolIn);}

			//need to free the sentinel struct since it isn't part of the for loop
			//the values of the default sentinel struct don't need to freed cuz they weren't dynamically allocated, unlike the other structs in the array
			if (printBoolIn != 2){ free(ptr[i]); free(ptr);}
		}
		if (numPtrsIn == 3){
			struct envVars*** ptr = (struct envVars***) arrIn;
			int i = 0;
			for (; ptr[i][0]->id != -1; i++){ myFreeDbObjArr_2(numPtrsIn-1, (void*) ptr[i], printBoolIn);}
			//need to free the sentinel struct since it isn't part of the for loop
			//the values of the default sentinel struct don't need to freed cuz they weren't dynamically allocated, unlike the other structs in the array
			if (printBoolIn != 2){ free(ptr[i]); free(ptr);}
		}
	}

	return 0;
}
// END envVars

// BEGIN listProgramsToInstall
int myFreeDbObjArr_3(int numPtrsIn, void* arrIn, int printBoolIn){
	// Recursion base case
	if (numPtrsIn == 1){
		struct listProgramsToInstall* objPtr = (struct listProgramsToInstall*) arrIn;
		if (printBoolIn > 0 && objPtr->id != -1){ printTable(objPtr);}
		if (printBoolIn != 2){ //freeing each dynamically alloc'd element in the struct and the pointer
			if (objPtr->packageName != NULL){free(objPtr->packageName);}
			if (objPtr->systemsToInstallOn != NULL){free(objPtr->systemsToInstallOn);}

			free(objPtr); //free the pointer
		}
	}

	// Recursion
	else {
		if (numPtrsIn == 2){
			struct listProgramsToInstall** ptr = (struct listProgramsToInstall**) arrIn;
			if (printBoolIn != 0){ printHeaderRowFilesAndFolders();}
			int i = 0;
			for (; ptr[i]->id != -1; i++){ myFreeDbObjArr_3(numPtrsIn-1, (void*) ptr[i], printBoolIn);}

			//need to free the sentinel struct since it isn't part of the for loop
			//the values of the default sentinel struct don't need to freed cuz they weren't dynamically allocated, unlike the other structs in the array
			if (printBoolIn != 2){ free(ptr[i]); free(ptr);}
		}
		if (numPtrsIn == 3){
			struct listProgramsToInstall*** ptr = (struct listProgramsToInstall***) arrIn;
			int i = 0;
			for (; ptr[i][0]->id != -1; i++){ myFreeDbObjArr_3(numPtrsIn-1, (void*) ptr[i], printBoolIn);}
			//need to free the sentinel struct since it isn't part of the for loop
			//the values of the default sentinel struct don't need to freed cuz they weren't dynamically allocated, unlike the other structs in the array
			if (printBoolIn != 2){ free(ptr[i]); free(ptr);}
		}
	}

	return 0;
}
// END listProgramsToInstall

// This function takes an unknown data type in and free's and/or prints the values
void myFreeHelper(int numPtrsIn, void* arrIn, int printBoolIn, char* charTypeIn){
	// Vars	
	char* FF = (char*) __func__;

	// Data Validation
	if (arrIn == NULL){ 
		fprintf(stderr, "%s%s%s%s%s", "===", F_MYFREE, " - ", FF, "===	Array is null, nothing to free\n"); 
		return;
	}
	if (numPtrsIn < 1){
		fprintf(stderr, "%s%s%s%s%s", "===", F_MYFREE, " - ", FF, "===	Invalid number of pointers. Aborting\n");
		return;
	}


	// For char-based arrays
	if (strcmp(charTypeIn, "char") == 0){
		myFreeChar(numPtrsIn, arrIn, printBoolIn);
	}


	// For filesAndFoldersDbObj-based arrays
	// This struct is designed to always be dynamically allocated, i.e. a struct is expected to be associated with a pointer
	// 	-this means a two-pointer struct is really representing a 1D-array of filesAndFoldersDbObj since each object is associated with a pointer
	// 	-the reasoning behind this is to make it faster to pass the struct across functions and methods without having to copy the data each time
	else if (strcmp(charTypeIn, "filesAndFoldersDbObj") == 0){
		myFreeFilesAndFoldersDbObj(numPtrsIn, arrIn, printBoolIn);
	}
		

		
	else{
		fprintf(stderr, "%s%s%s%s%s", "===", F_MYFREE, " - ", FF, "===	Unaccepted (but not invalid) data type. Accepted data categories are:\n");
		fprintf(stderr, "%s%s%s%s%s", "===", F_MYFREE, " - ", FF, "===		char-based arrays (up to 4-pointer arrays) and filesAndFoldersDbObj-based arrays (up to 2-points arrays)\n");
		fprintf(stderr, "%s%s%s%s%s", "===", F_MYFREE, " - ", FF, "===		Make sure to remove all astericks before argument 4. It should just include the array category\n");
		return;
	}


}
//=================================================================================================


//=================================================================================================
// CHAR-BASED ARRAYS
//=================================================================================================

// ONE ARG
void oneArgsFreeStr(char* arrIn, ...){ myFreeHelper(1, (void*) arrIn, 0, "char");}
void oneArgsFreeStr2(char** arrIn, ...){ myFreeHelper(2, (void*) arrIn, 0, "char");}
void oneArgsFreeStr3(char*** arrIn, ...){ myFreeHelper(3, (void*) arrIn, 0, "char");}
void oneArgsFreeStr4(char**** arrIn, ...){ myFreeHelper(4, (void*) arrIn, 0, "char");}

// TWO ARGS
void twoArgsFreeStrPrint(char* arrIn, int printBoolIn, ...){ myFreeHelper(1, (void*) arrIn, printBoolIn, "char");}
void twoArgsFreeOther(char* arrIn, char* typeIn, ...){ myFreeHelper(1, (void*) arrIn, 0, typeIn);}

void twoArgsFreeStrPrint2(char** arrIn, int printBoolIn, ...){ myFreeHelper(2, (void*) arrIn, printBoolIn, "char");}
void twoArgsFreeOther2(char** arrIn, char* typeIn, ...){ myFreeHelper(2, (void*) arrIn, 0, typeIn);}

void twoArgsFreeStrPrint3(char*** arrIn, int printBoolIn, ...){ myFreeHelper(3, (void*) arrIn, printBoolIn, "char");}
void twoArgsFreeOther3(char*** arrIn, char* typeIn, ...){ myFreeHelper(3, (void*) arrIn, 0, typeIn);}

void twoArgsFreeStrPrint4(char**** arrIn, int printBoolIn, ...){ myFreeHelper(4, (void*) arrIn, printBoolIn, "char");}
void twoArgsFreeOther4(char**** arrIn, char* typeIn, ...){ myFreeHelper(4, (void*) arrIn, 0, typeIn);}

// THREE ARGS
void threeArgsFree1(char* arrIn, int printBoolIn, char* typeIn, ...){ myFreeHelper(1, (void*) arrIn, printBoolIn, typeIn);}
void threeArgsFree2(char** arrIn, int printBoolIn, char* typeIn, ...){ myFreeHelper(2, (void*) arrIn, printBoolIn, typeIn);}
void threeArgsFree3(char*** arrIn, int printBoolIn, char* typeIn, ...){ myFreeHelper(3, (void*) arrIn, printBoolIn, typeIn);}
void threeArgsFree4(char**** arrIn, int printBoolIn, char* typeIn, ...){ myFreeHelper(4, (void*) arrIn, printBoolIn, typeIn);}
//=================================================================================================


//=================================================================================================
// PRINT STATEMENTS - ERRORS
//=================================================================================================
void badDataMyFree(...){ fprintf(stderr, "%s%s%s", "===", F_MYFREE, "===	Invalid data type at arg 1\n");}
void badDataTypeMyFreePos2(...){ fprintf(stderr, "%s%s%s", "===", F_MYFREE, "===	Invalid data type at arg 2\n");}
void badDataTypeMyFreePos3(...){ fprintf(stderr, "%s%s%s", "===", F_MYFREE, "===	Invalid data type at arg 3\n");}
//=================================================================================================


//=================================================================================================
// GENERIC TYPE TO FUNCTION
//=================================================================================================
void oneArgsFreeFilesDbObj1(struct filesAndFoldersDbObj* objIn,...){ myFreeHelper(1, (void*) objIn, 0, "filesAndFoldersDbObj");}
void oneArgsFreeFilesDbObj2(struct filesAndFoldersDbObj** objIn,...){ myFreeHelper(2, (void*) objIn, 0, "filesAndFoldersDbObj");}
void twoArgsFreeFilesDbObjPrint1(struct filesAndFoldersDbObj* objIn, int printBoolIn,...){ myFreeHelper(1, (void*) objIn, printBoolIn, "filesAndFoldersDbObj");}
void twoArgsFreeFilesDbObjPrint2(struct filesAndFoldersDbObj** objIn, int printBoolIn,...){ myFreeHelper(2, (void*) objIn, printBoolIn, "filesAndFoldersDbObj");}
//=================================================================================================


//=================================================================================================
// DATABASE TYPE TO FUNCTION
//=================================================================================================
// BEGIN listProgramsToInstall
void oneArgsFreelistProgramsToInstallOne(struct listProgramsToInstall* objIn, ...){ myFreeDbObjArr_3(1, (void*) objIn, 0);}
void oneArgsFreelistProgramsToInstallTwo(struct listProgramsToInstall** objIn, ...){ myFreeDbObjArr_3(2, (void*) objIn, 0);}
void twoArgsFreelistProgramsToInstallOne(struct listProgramsToInstall* objIn, int printBoolIn, ...){ myFreeDbObjArr_3(1, (void*) objIn, printBoolIn);}
void twoArgsFreelistProgramsToInstallTwo(struct listProgramsToInstall** objIn, int printBoolIn, ...){ myFreeDbObjArr_3(2, (void*) objIn, printBoolIn);}
void threeArgsFreelistProgramsToInstallOne(struct listProgramsToInstall* objIn, int printBoolIn, ...){ myFreeDbObjArr_3(1, (void*) objIn, printBoolIn);}
void threeArgsFreelistProgramsToInstallTwo(struct listProgramsToInstall** objIn, int printBoolIn, ...){ myFreeDbObjArr_3(2, (void*) objIn, printBoolIn);}
// END listProgramsToInstall
// BEGIN envVars
void oneArgsFreeenvVarsOne(struct envVars* objIn, ...){ myFreeDbObjArr_2(1, (void*) objIn, 0);}
void oneArgsFreeenvVarsTwo(struct envVars** objIn, ...){ myFreeDbObjArr_2(2, (void*) objIn, 0);}
void twoArgsFreeenvVarsOne(struct envVars* objIn, int printBoolIn, ...){ myFreeDbObjArr_2(1, (void*) objIn, printBoolIn);}
void twoArgsFreeenvVarsTwo(struct envVars** objIn, int printBoolIn, ...){ myFreeDbObjArr_2(2, (void*) objIn, printBoolIn);}
void threeArgsFreeenvVarsOne(struct envVars* objIn, int printBoolIn, ...){ myFreeDbObjArr_2(1, (void*) objIn, printBoolIn);}
void threeArgsFreeenvVarsTwo(struct envVars** objIn, int printBoolIn, ...){ myFreeDbObjArr_2(2, (void*) objIn, printBoolIn);}
// END envVars
// BEGIN acceptedDbDataTypes
void oneArgsFreeacceptedDbDataTypesOne(struct acceptedDbDataTypes* objIn, ...){ myFreeDbObjArr_1(1, (void*) objIn, 0);}
void oneArgsFreeacceptedDbDataTypesTwo(struct acceptedDbDataTypes** objIn, ...){ myFreeDbObjArr_1(2, (void*) objIn, 0);}
void twoArgsFreeacceptedDbDataTypesOne(struct acceptedDbDataTypes* objIn, int printBoolIn, ...){ myFreeDbObjArr_1(1, (void*) objIn, printBoolIn);}
void twoArgsFreeacceptedDbDataTypesTwo(struct acceptedDbDataTypes** objIn, int printBoolIn, ...){ myFreeDbObjArr_1(2, (void*) objIn, printBoolIn);}
void threeArgsFreeacceptedDbDataTypesOne(struct acceptedDbDataTypes* objIn, int printBoolIn, ...){ myFreeDbObjArr_1(1, (void*) objIn, printBoolIn);}
void threeArgsFreeacceptedDbDataTypesTwo(struct acceptedDbDataTypes** objIn, int printBoolIn, ...){ myFreeDbObjArr_1(2, (void*) objIn, printBoolIn);}
// END acceptedDbDataTypes

//=================================================================================================


// INPUT STRUCTURE---------------------------------------------------------------------------------
// <datatype* arr>, [int printBool], [char* dataType]
//-------------------------------------------------------------------------------------------------


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define oneArgsDefineMyFree(...) _Generic((FIRST(__VA_ARGS__)), \
                char*: oneArgsFreeStr, \
                char**: oneArgsFreeStr2, \
                char***: oneArgsFreeStr3, \
                char****: oneArgsFreeStr4, \
                default: badDataMyFree \
                )(__VA_ARGS__) \


#define twoArgsDefineMyFree(...) _Generic((FIRST(__VA_ARGS__)), \
                char*: _Generic((SECOND(__VA_ARGS__)), \
                        int: twoArgsFreeStrPrint, \
                        char*: twoArgsFreeOther), \
                char**:  _Generic((SECOND(__VA_ARGS__)), \
                        int: twoArgsFreeStrPrint2, \
                        char*: twoArgsFreeOther2), \
                char***: _Generic((SECOND(__VA_ARGS__)), \
                        int: twoArgsFreeStrPrint3, \
                        char*: twoArgsFreeOther3), \
                char****: _Generic((SECOND(__VA_ARGS__)), \
                        int: twoArgsFreeStrPrint4, \
                        char*: twoArgsFreeOther4), \
                struct filesAndFoldersDbObj*: _Generic((SECOND(__VA_ARGS__)), \
                        int: twoArgsFreeFilesDbObjPrint1, \
                        default: badDataTypeMyFreePos2), \
                struct filesAndFoldersDbObj**: _Generic((SECOND(__VA_ARGS__)), \
                        int: twoArgsFreeFilesDbObjPrint2, \
                        default: badDataTypeMyFreePos2), \
                default: badDataMyFree \
                )(__VA_ARGS__) \


#define threeArgsDefineMyFree(...) _Generic((FIRST(__VA_ARGS__)), \
                char*: _Generic((SECOND(__VA_ARGS__)), \
                        int: _Generic((THIRD(__VA_ARGS__)), \
                                char*: threeArgsFree1, \
                                default: badDataTypeMyFreePos2 ), \
                        default: badDataTypeMyFreePos2), \
                char**:  _Generic((SECOND(__VA_ARGS__)), \
                        int: _Generic((THIRD(__VA_ARGS__)), \
                                char*: threeArgsFree2, \
                                default: badDataTypeMyFreePos2 ), \
                        default: badDataTypeMyFreePos2), \
                char***: _Generic((SECOND(__VA_ARGS__)), \
                        int: _Generic((THIRD(__VA_ARGS__)), \
                                char*: threeArgsFree3, \
                                default: badDataTypeMyFreePos2 ), \
                        default: badDataTypeMyFreePos2), \
                char****: _Generic((SECOND(__VA_ARGS__)), \
                        int: _Generic((THIRD(__VA_ARGS__)), \
                                char*: threeArgsFree4, \
                                default: badDataTypeMyFreePos2), \
                        default: badDataTypeMyFreePos2), \
                default: badDataMyFree \
                )(__VA_ARGS__) \


#define myFree(...) \
        do { \
                int numArgs = COUNT_ARGS(__VA_ARGS__); \
                if (numArgs == 1) { oneArgsDefineMyFree(ARGSWITHPADDING(__VA_ARGS__));} \
                if (numArgs == 2) { twoArgsDefineMyFree(ARGSWITHPADDING(__VA_ARGS__));} \
                if (numArgs == 3) { threeArgsDefineMyFree(ARGSWITHPADDING(__VA_ARGS__));} \
        } while (0) \
*/
