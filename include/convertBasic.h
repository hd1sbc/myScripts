#ifndef CONVERTBASIC
#define CONVERTBASIC

#include "symLink_100.sl"
#include "symLink_99.sl"
#include "symLink_49.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* globalReturnStrConvertBasic;
extern char globalReturnCharConvertBasic;
extern int globalReturnIntConvertBasic;
extern double globalReturnDoubleConvertBasic;
extern int globalTypeConvertBasic;
extern char acceptedDataTypesConvertBasic[1024];
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// DEFINES
//=================================================================================================


#define convertBasicHelper(_1, _2) \
                _Generic((_1), \
                int: _Generic((_2), \
                        char*: convertBasicFromInt, \
                        default: badArgPos2ConvertBasic), \
                double: _Generic((_2), \
                        char*: convertBasicFromDouble, \
                        default: badArgPos2ConvertBasic), \
                char: _Generic((_2), \
                        char*: convertBasicFromChar, \
                        default: badArgPos2ConvertBasic), \
                char*: _Generic((_2), \
                        char*: convertBasicFromStr, \
                        default: badArgPos2ConvertBasic), \
                default: badArgPos1ConvertBasic \
                )(_1,_2); \

#define convertBasic(_1, _2) ({ \
        void* returnData; \
        do { \
                convertBasicHelper(_1, _2); \
                if (globalTypeConvertBasic == 0){ returnData = NULL;} \
                else if (globalTypeConvertBasic == 1){ returnData = (char*) globalReturnStrConvertBasic;} \
                else if (globalTypeConvertBasic == 2){ \
                        int* returnPtr = &globalReturnIntConvertBasic; \
                        returnData = (int*) returnPtr; \
                } \
		else if (globalTypeConvertBasic == 4){ \
                        double* returnPtr = &globalReturnDoubleConvertBasic; \
                        returnData = (double*) returnPtr; \
                } \
        } while(0); \
        returnData; \
        }) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void badArgPos1ConvertBasic(...);
void badArgPos2ConvertBasic(...);
char* convertBasicFileToString(char* filePathIn);
char* convertBasicFromInt(int intIn, char* dataTypeToConvertTo);
char* convertBasicFromDouble(double doubleIn, char* dataTypeToConvertTo);
void* convertBasicFromChar(char charIn, char* dataTypeToConvertTo);
void* convertBasicFromStr(char* strIn, char* dataTypeToConvertTo);
//=================================================================================================

#endif
