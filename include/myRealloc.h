#ifndef MYREALLOC
#define MYREALLOC

#include "symLink_45.sl"
#include "symLink_37.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* returnStrMyRealloc;
extern char** returnStrArrMyRealloc;
extern int globalTypeMyRealloc;
extern int defaultStrInc;
extern int defaultStrArrInc;
extern int currSizeProvided;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// DEFINES
//=================================================================================================


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

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void invalidNumArgsMyRealloc(int i);
void printAcceptedDataTypes();
void badReallocDataType(int argPosition);
void badCurrSizeDataType(int argPosition);
void badIncrementDataType(int argPosition);
void badReallocDataAtPos1(...);
void twoArgsBadDataAtPos1(...);
void twoArgsBadDataAtPos2Int(...);
void badReallocDataAtPos2(...);
void threeArgsBadDataAtPos1(...);
void threeArgsBadDataAtPos3(...);
char* resetStr(char* strIn);
char* reallocStr(int incrementIn, char* strIn, int currSizeIn);
char** reallocStrArr(int incrementIn, char** strArrIn, int currSizeIn);
char* oneArgsStr(char* strIn, ...);
char** oneArgsStrArr(char** strArrIn, ...);
char* twoArgsStrCurrSize(char* strIn, int currSizeIn, ...);
char** twoArgsStrArrCurrSize(char** strArrIn, int currSizeIn, ...);
char* twoArgsStrInc(int incIn, char* strIn, ...);
char** twoArgsStrArrInc(int incIn, char** strArrIn, ...);
char* threeArgsStr(int incIn, char* strIn, int currSizeIn, ...);
char** threeArgsStrArr(int incIn, char** strArrIn, int currSizeIn, ...);
//=================================================================================================

#endif
