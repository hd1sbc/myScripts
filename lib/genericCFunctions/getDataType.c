#include "symLink_40.sl"
#include "symLink_30.sl"
#include "symLink_48.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
int globalDataTypeGetDataType = -1;
int dataTypeArr[5];
char acceptedDataTypeGetDataType[1024] = "int, double, float, char, char*, char**, struct filesAndFoldersDbObj*, struct filesAndFoldersDbObj**";

// Error Msg Constant Vars
char* F_GETDATATYPE = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------


//=================================================================================================
// CONVERT DATA TYPE TO AN INT REPRESENTATION
//=================================================================================================
void dataTypeInt(...){ globalDataTypeGetDataType = 0;}
void dataTypeDouble(...){ globalDataTypeGetDataType = 1;}
void dataTypeFloat(...){ globalDataTypeGetDataType = 2;}
void dataTypeChar(...){ globalDataTypeGetDataType = 3;}
void dataTypeStr(...){ globalDataTypeGetDataType = 4;}
void dataTypeStrArr(...){ globalDataTypeGetDataType = 5;}
void dataTypeFilesAndFolders(...){ globalDataTypeGetDataType = 6;}
void dataTypeFilesAndFoldersArr(...){ globalDataTypeGetDataType = 7;}
//=================================================================================================


//=================================================================================================
// PRINT STATEMETNS - ERRORS
//=================================================================================================
void badDataTypeGetDataType(...){ 
	fprintf(stderr, "%s%s%s", "===", F_GETDATATYPE, "===   Invalid data type. Accepted data types are: \n");
	perror(acceptedDataTypeGetDataType);
}
//=================================================================================================


//=================================================================================================
// CODE
//=================================================================================================

// This function take a variable number of integer inputs and adds them to an array
// The inputs represent the data type
int* getDataTypeArr(int numArgsIn, ...){
        va_list args;
        va_start(args, numArgsIn);

        for(int i = 0; i < numArgsIn; i++){
                dataTypeArr[i] = va_arg(args, int);
        }

	return dataTypeArr;
}
//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
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
*/
//=================================================================================================
