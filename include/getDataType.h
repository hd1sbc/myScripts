#ifndef GETDATATYPE
#define GETDATATYPE

#include "symLink_40.sl"
#include "symLink_30.sl"
#include "symLink_48.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern int globalDataTypeGetDataType;
extern int dataTypeArr[5];
extern char acceptedDataTypeGetDataType[1024];
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// DEFINES
//=================================================================================================


#define getDataTypeHelper(_1) _Generic((_1), \
                        int: dataTypeInt, \
                        char: dataTypeChar, \
                        double: dataTypeDouble, \
                        float: dataTypeFloat,\
                        char*: dataTypeStr, \
                        char**: dataTypeStrArr, \
                        struct filesAndFoldersDbObj*: dataTypeFilesAndFolders, \
                        struct filesAndFoldersDbObj**: dataTypeFilesAndFoldersArr, \
                        default: badDataTypeGetDataType \
                        )(_1); \


#define getDataType(_1) ({ \
        int dataTypeReturn; \
        do { \
                getDataTypeHelper(_1); \
                dataTypeReturn = globalDataTypeGetDataType; \
        } while(0); \
        dataTypeReturn; \
        }) \

#define makeArrayOfArgDataTypes(numArgsIn, ...) ({ \
	int* returnArr; \
        do { \
                if (numArgsIn < 3){ returnArr = getDataTypeArr(2, getDataType(FIRST(__VA_ARGS__)), getDataType(SECOND(__VA_ARGS__)) ); }\
                else if (numArgsIn < 4){ returnArr = getDataTypeArr(3, getDataType(FIRST(__VA_ARGS__)), getDataType(SECOND(__VA_ARGS__)), getDataType(THIRD(__VA_ARGS__)) ); }\
                else if (numArgsIn < 5){ returnArr = getDataTypeArr(4, getDataType(FIRST(__VA_ARGS__)), getDataType(SECOND(__VA_ARGS__)), getDataType(THIRD(__VA_ARGS__)), getDataType(FOURTH(__VA_ARGS__)) ); }\
                else if (numArgsIn < 6){ returnArr = getDataTypeArr(5, getDataType(FIRST(__VA_ARGS__)), getDataType(SECOND(__VA_ARGS__)), getDataType(THIRD(__VA_ARGS__)), getDataType(FOURTH(__VA_ARGS__)), getDataType(FIFTH(__VA_ARGS__)) ); }\
        } while (0); \
	returnArr; \
	}) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void dataTypeInt(...);
void dataTypeDouble(...);
void dataTypeFloat(...);
void dataTypeChar(...);
void dataTypeStr(...);
void dataTypeStrArr(...);
void dataTypeFilesAndFolders(...);
void dataTypeFilesAndFoldersArr(...);
void badDataTypeGetDataType(...);
int* getDataTypeArr(int numArgsIn, ...);
//=================================================================================================

#endif
