#ifndef MYFREE
#define MYFREE

#include "symLink_42.sl"
#include "symLink_30.sl"
#include "symLink_29.sl"
#include "symLink_48.sl"
#include "symLink_46.sl"


//=================================================================================================
// DEFINES
//=================================================================================================


#define oneArgsDefineMyFree(...) _Generic((FIRST(__VA_ARGS__)), \
                char*: oneArgsFreeStr, \
                char**: oneArgsFreeStr2, \
                char***: oneArgsFreeStr3, \
                char****: oneArgsFreeStr4, \
                struct filesAndFoldersDbObj*: oneArgsFreeFilesDbObj1, \
                struct filesAndFoldersDbObj**: oneArgsFreeFilesDbObj2, \
		struct acceptedDbDataTypes*: oneArgsFreeacceptedDbDataTypesOne, \
		struct acceptedDbDataTypes**: oneArgsFreeacceptedDbDataTypesTwo, \
		struct envVars*: oneArgsFreeenvVarsOne, \
		struct envVars**: oneArgsFreeenvVarsTwo, \
		struct listProgramsToInstall*: oneArgsFreelistProgramsToInstallOne, \
		struct listProgramsToInstall**: oneArgsFreelistProgramsToInstallTwo, \
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
		struct acceptedDbDataTypes*: twoArgsFreeacceptedDbDataTypesOne, \
		struct acceptedDbDataTypes**: twoArgsFreeacceptedDbDataTypesTwo, \
		struct envVars*: twoArgsFreeenvVarsOne, \
		struct envVars**: twoArgsFreeenvVarsTwo, \
		struct listProgramsToInstall*: twoArgsFreelistProgramsToInstallOne, \
		struct listProgramsToInstall**: twoArgsFreelistProgramsToInstallTwo, \
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
		struct acceptedDbDataTypes*: threeArgsFreeacceptedDbDataTypesOne, \
		struct acceptedDbDataTypes**: threeArgsFreeacceptedDbDataTypesTwo, \
		struct envVars*: threeArgsFreeenvVarsOne, \
		struct envVars**: threeArgsFreeenvVarsTwo, \
		struct listProgramsToInstall*: threeArgsFreelistProgramsToInstallOne, \
		struct listProgramsToInstall**: threeArgsFreelistProgramsToInstallTwo, \
                default: badDataMyFree \
                )(__VA_ARGS__) \


#define myFree(...) \
        do { \
                int numArgs = COUNT_ARGS(__VA_ARGS__); \
                if (numArgs == 1) { oneArgsDefineMyFree(ARGSWITHPADDING(__VA_ARGS__));} \
                if (numArgs == 2) { twoArgsDefineMyFree(ARGSWITHPADDING(__VA_ARGS__));} \
                if (numArgs == 3) { threeArgsDefineMyFree(ARGSWITHPADDING(__VA_ARGS__));} \
        } while (0) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void myFreeChar(int numPtrsIn, void* arrIn, int printBoolIn);
void myFreeFilesAndFoldersDbObj(int numPtrsIn, void* arrIn, int printBoolIn);
int myFreeDbObjArr_1(int numPtrsIn, void* arrIn, int printBoolIn);
int myFreeDbObjArr_2(int numPtrsIn, void* arrIn, int printBoolIn);
int myFreeDbObjArr_3(int numPtrsIn, void* arrIn, int printBoolIn);
void myFreeHelper(int numPtrsIn, void* arrIn, int printBoolIn, char* charTypeIn);
void oneArgsFreeStr(char* arrIn, ...);
void oneArgsFreeStr2(char** arrIn, ...);
void oneArgsFreeStr3(char*** arrIn, ...);
void oneArgsFreeStr4(char**** arrIn, ...);
void twoArgsFreeStrPrint(char* arrIn, int printBoolIn, ...);
void twoArgsFreeOther(char* arrIn, char* typeIn, ...);
void twoArgsFreeStrPrint2(char** arrIn, int printBoolIn, ...);
void twoArgsFreeOther2(char** arrIn, char* typeIn, ...);
void twoArgsFreeStrPrint3(char*** arrIn, int printBoolIn, ...);
void twoArgsFreeOther3(char*** arrIn, char* typeIn, ...);
void twoArgsFreeStrPrint4(char**** arrIn, int printBoolIn, ...);
void twoArgsFreeOther4(char**** arrIn, char* typeIn, ...);
void threeArgsFree1(char* arrIn, int printBoolIn, char* typeIn, ...);
void threeArgsFree2(char** arrIn, int printBoolIn, char* typeIn, ...);
void threeArgsFree3(char*** arrIn, int printBoolIn, char* typeIn, ...);
void threeArgsFree4(char**** arrIn, int printBoolIn, char* typeIn, ...);
void badDataMyFree(...);
void badDataTypeMyFreePos2(...);
void badDataTypeMyFreePos3(...);
void oneArgsFreeFilesDbObj1(struct filesAndFoldersDbObj* objIn,...);
void oneArgsFreeFilesDbObj2(struct filesAndFoldersDbObj** objIn,...);
void twoArgsFreeFilesDbObjPrint1(struct filesAndFoldersDbObj* objIn, int printBoolIn,...);
void twoArgsFreeFilesDbObjPrint2(struct filesAndFoldersDbObj** objIn, int printBoolIn,...);
void oneArgsFreelistProgramsToInstallOne(struct listProgramsToInstall* objIn, ...);
void oneArgsFreelistProgramsToInstallTwo(struct listProgramsToInstall** objIn, ...);
void twoArgsFreelistProgramsToInstallOne(struct listProgramsToInstall* objIn, int printBoolIn, ...);
void twoArgsFreelistProgramsToInstallTwo(struct listProgramsToInstall** objIn, int printBoolIn, ...);
void threeArgsFreelistProgramsToInstallOne(struct listProgramsToInstall* objIn, int printBoolIn, ...);
void threeArgsFreelistProgramsToInstallTwo(struct listProgramsToInstall** objIn, int printBoolIn, ...);
void oneArgsFreeenvVarsOne(struct envVars* objIn, ...);
void oneArgsFreeenvVarsTwo(struct envVars** objIn, ...);
void twoArgsFreeenvVarsOne(struct envVars* objIn, int printBoolIn, ...);
void twoArgsFreeenvVarsTwo(struct envVars** objIn, int printBoolIn, ...);
void threeArgsFreeenvVarsOne(struct envVars* objIn, int printBoolIn, ...);
void threeArgsFreeenvVarsTwo(struct envVars** objIn, int printBoolIn, ...);
void oneArgsFreeacceptedDbDataTypesOne(struct acceptedDbDataTypes* objIn, ...);
void oneArgsFreeacceptedDbDataTypesTwo(struct acceptedDbDataTypes** objIn, ...);
void twoArgsFreeacceptedDbDataTypesOne(struct acceptedDbDataTypes* objIn, int printBoolIn, ...);
void twoArgsFreeacceptedDbDataTypesTwo(struct acceptedDbDataTypes** objIn, int printBoolIn, ...);
void threeArgsFreeacceptedDbDataTypesOne(struct acceptedDbDataTypes* objIn, int printBoolIn, ...);
void threeArgsFreeacceptedDbDataTypesTwo(struct acceptedDbDataTypes** objIn, int printBoolIn, ...);
//=================================================================================================

#endif
