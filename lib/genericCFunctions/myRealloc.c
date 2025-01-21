#include "symLink_45.sl"
#include "symLink_37.sl"

// INPUT STRUCTURE---------------------------------------------------------------------------------
// [int increment], <datatype* data>, [int currentDataSize]
//-------------------------------------------------------------------------------------------------

// GLOBAL VARS--------------------------------------------------------------------------------------
//return vars
char* returnStrMyRealloc;
char** returnStrArrMyRealloc;
int globalTypeMyRealloc = -1; //-1 for error (don't return anything), 0 for str, 1 for strArr
//tracking vars
int defaultStrInc = 128;
int defaultStrArrInc = 1;
int currSizeProvided = 0;

// Error Msg Constant Vars
char* F_MYREALLOC = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------


//================================================================================================= 
// PRINT STATEMENTS - ERRORS
//=================================================================================================

//Generic versions of bad data type----------------------------------------------------------------
void invalidNumArgsMyRealloc(int i){
	fprintf(stderr, "%s%s%s", "===", F_MYREALLOC, "===	");
	fprintf(stderr, "%s%d%s", "Invalid number of args - ", i, " were provided. Between 1 and 3 arguments are permitted\n");
}
void printAcceptedDataTypes(){ fprintf(stderr, "%s", "		'char*', 'char**', 'filesAndFoldersDbObj**'");}
void badReallocDataType(int argPosition){
	fprintf(stderr, "%s%s%s", "===", F_MYREALLOC, "===	");
	fprintf(stderr, "%s%d%s\n", "Invalid data type for realloc at arg ", argPosition, " - accepted types for realloc are:");
	printAcceptedDataTypes();
	fprintf(stderr, "\n");
}
void badCurrSizeDataType(int argPosition){ 
	fprintf(stderr, "%s%s%s", "===", F_MYREALLOC, "===	");
	fprintf(stderr, "%s%d%s\n", "Invalid data type for arg ", argPosition, " - expected 'int' that represents the size of data");
}
void badIncrementDataType(int argPosition){ 
	fprintf(stderr, "%s%s%s", "===", F_MYREALLOC, "===	");
	printf("%s%d%s\n", "Invalid data type for arg ", argPosition, " - expected 'int' for increment value");
}
//-------------------------------------------------------------------------------------------------

// Bad Data Type By Number Of Args At Arg Positions------------------------------------------------
// One Args
void badReallocDataAtPos1(...){ badReallocDataType(1);}
// Two Args
void twoArgsBadDataAtPos1(...){ 
	fprintf(stderr, "%s%s%s", "===", F_MYREALLOC, "===	");
	fprintf(stderr, "%s\n", "Invalid data type at arg 1 - expected either 'int' or an accepted data type for realloc:");
	printAcceptedDataTypes();
	fprintf(stderr, "\n");
}
void twoArgsBadDataAtPos2Int(...){ badCurrSizeDataType(2);}
void badReallocDataAtPos2(...){ badReallocDataType(2);}
// Three Args
void threeArgsBadDataAtPos1(...){
	fprintf(stderr, "%s%s%s", "===", F_MYREALLOC, "===	");
	fprintf(stderr, "Invalid data type at arg 1 - expected 'int' for increment size");
}
void threeArgsBadDataAtPos3(...){ badCurrSizeDataType(3);}
//-------------------------------------------------------------------------------------------------

//=================================================================================================


//================================================================================================= 
// CODE
//=================================================================================================

// This functions free's the input string and returns a new empty string
char* resetStr(char* strIn){
	if (strIn != NULL){ free(strIn);}
	char* returnStr = createStr();
	return returnStr;
}

// This function realloc's a string
char* reallocStr(int incrementIn, char* strIn, int currSizeIn){
	// Vars
	char* FF = (char*) __func__;

	//checking if error with getArrSize() function
	if (currSizeIn == -1){ 
		fprintf(stderr, "%s%s%s%s%s", "===", F_MYREALLOC, " - ", FF, "===	");
		fprintf(stderr, "Invalid current string size\n");
		return NULL;
	}
	
	
	//reset string
	//-if the current size = 0 and the increment = 0, that would be a non-logical combo
	//-since this combo has no logical use, it can be used to represent another possible utility - reseting the string
	if (currSizeIn == 0 && incrementIn == 0){ strIn = resetStr(strIn);}

	//if the current size is 0, but the string needs to be realloc'd
	//-the currSizeProvided is necessary cuz if not, a string could be alloc'd, but not filled in. In that case, the alloc'd string isn't free'd
	//	-however, I can't include a free statement cuz the string might not have been alloc'd. The currSizeProvided solves this
	else if (currSizeIn == 0 && incrementIn > 0 && currSizeProvided == 1){
		returnStrMyRealloc = malloc(sizeof(char)*(incrementIn+1));
		returnStrMyRealloc[0] = '\0';
		currSizeProvided = 0;
	}

	//normal realloc
	else{
		returnStrMyRealloc = realloc(strIn, sizeof(char)*(incrementIn+currSizeIn+1));
	}

	globalTypeMyRealloc = 0; //sets the type to string/char*
	return returnStrMyRealloc;
}

// This function realloc's a string array
char** reallocStrArr(int incrementIn, char** strArrIn, int currSizeIn){
	// Vars
	char* FF = (char*) __func__;
	
	//checking if error with getArrSize() function
	if (currSizeIn == -1){ 
		fprintf(stderr, "%s%s%s%s%s", "===", F_MYREALLOC, " - ", FF, "===	");
		fprintf(stderr, "Invalid current string arr size\n");
		return NULL;
	}


	//reset string array
	//-if the current size = 0 and the increment = 0, that would be a non-logical combo
	//-since this combo has no logical use, it can be used to represent another possible utility - reseting the string array
	if (currSizeIn == 0 && incrementIn == 0){ 
		myFree(strArrIn);
		char** returnStr = malloc(sizeof(char*)*(defaultStrArrInc+1));
		returnStr[0] = createStr();
		returnStr[1] = NULL;
	}
	
	//if the current size is 0, but the string array needs to be realloc'd
	//unlike with the reallocStr() function, I just assume the array hasn't been alloc'd
	else if (currSizeIn == 0 && incrementIn > 0){
		returnStrArrMyRealloc = malloc(sizeof(char*)*(incrementIn+2));
		returnStrArrMyRealloc[0] = malloc(sizeof(char*));
		returnStrArrMyRealloc[0][0] = '\0';
		returnStrArrMyRealloc[1] = NULL;

	}

	//normal realloc
	else {
		strArrIn = (char**) realloc(strArrIn, sizeof(char*)*(incrementIn+currSizeIn+2));
		strArrIn[currSizeIn] = malloc(sizeof(char*));
		strArrIn[currSizeIn][0] = '\0';
		strArrIn[currSizeIn+1] = NULL;
		returnStrArrMyRealloc = strArrIn;	
	}

	globalTypeMyRealloc = 1; //setting the type to string/char*
	return returnStrArrMyRealloc;
}
//=================================================================================================


//=================================================================================================
// GENERIC TYPE TO FUNCTION
//=================================================================================================

// ONE ARGS
char* oneArgsStr(char* strIn, ...){ strIn = reallocStr(defaultStrInc, strIn, getArrSize(strIn)); return strIn;}
char** oneArgsStrArr(char** strArrIn, ...){ return reallocStrArr(defaultStrArrInc, strArrIn, getArrSize(strArrIn));}

// TWO ARGS
char* twoArgsStrCurrSize(char* strIn, int currSizeIn, ...){ currSizeProvided = 1; return reallocStr(defaultStrInc, strIn, currSizeIn);}
char** twoArgsStrArrCurrSize(char** strArrIn, int currSizeIn, ...){ return reallocStrArr(defaultStrArrInc, strArrIn, currSizeIn);}
char* twoArgsStrInc(int incIn, char* strIn, ...){ return reallocStr(incIn, strIn, getArrSize(strIn));}
char** twoArgsStrArrInc(int incIn, char** strArrIn, ...){ return reallocStrArr(incIn, strArrIn, getArrSize(strArrIn));}

// THREE ARG
char* threeArgsStr(int incIn, char* strIn, int currSizeIn, ...){ currSizeProvided = 1; return reallocStr(incIn, strIn, currSizeIn);}
char** threeArgsStrArr(int incIn, char** strArrIn, int currSizeIn, ...){ return reallocStrArr(incIn, strArrIn, currSizeIn);}
//=================================================================================================

// INPUT STRUCTURE---------------------------------------------------------------------------------
// [int increment], <datatype* data>, [int currentDataSize]
//-------------------------------------------------------------------------------------------------


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define oneArgsDefine(...)  \
                _Generic((FIRST(__VA_ARGS__)), \
                char*: oneArgsStr , \
                char**: oneArgsStrArr, \
                default: badReallocDataAtPos1 \
                )(__VA_ARGS__); \

#define twoArgsDefine(...) _Generic((FIRST(__VA_ARGS__)), \
                char*: _Generic((SECOND(__VA_ARGS__)), \
                        int: twoArgsStrCurrSize, \
                        default: twoArgsBadDataAtPos2Int), \
                char**: _Generic((SECOND(__VA_ARGS__)), \
                        int: twoArgsStrArrCurrSize, \
                        default: twoArgsBadDataAtPos2Int), \
                int: _Generic((SECOND(__VA_ARGS__)), \
                             char*: twoArgsStrInc, \
                             char**: twoArgsStrArrInc, \
                             default: badReallocDataAtPos2), \
                default: twoArgsBadDataAtPos1 \
                )(__VA_ARGS__); \


#define threeArgsDefine(...) \
                _Generic((FIRST(__VA_ARGS__)), \
                int: _Generic((SECOND(__VA_ARGS__)), \
                        char*: _Generic((THIRD(__VA_ARGS__)), \
                                int: threeArgsStr, \
                                default: threeArgsBadDataAtPos3), \
                        char**: _Generic((THIRD(__VA_ARGS__)), \
                                int: threeArgsStrArr ,\
                                default: threeArgsBadDataAtPos3), \
                        default: badReallocDataAtPos2), \
                default: threeArgsBadDataAtPos1 \
                )(__VA_ARGS__); \

#define myRealloc(...) ({ \
        void* returnArr; \
        do { \
                int numArgs = COUNT_ARGS(__VA_ARGS__); \
                if (numArgs == 1) { oneArgsDefine(ARGSWITHPADDING(__VA_ARGS__));} \
                else if (numArgs == 2) { twoArgsDefine(ARGSWITHPADDING(__VA_ARGS__));} \
                else if (numArgs == 3) { threeArgsDefine(ARGSWITHPADDING(__VA_ARGS__));} \
                else { invalidNumArgsMyRealloc(numArgs);} \
                \
                if (globalTypeMyRealloc == 0) { \
                        returnArr = (char*) returnStrMyRealloc; \
                        globalTypeMyRealloc = 0; \
                } \
                else if (globalTypeMyRealloc == 1) { \
                        returnArr = (char**) returnStrArrMyRealloc; \
                        globalTypeMyRealloc = 0; \
                } \
        } while (0); \
        returnArr; \
        }) \

*/
//=================================================================================================
