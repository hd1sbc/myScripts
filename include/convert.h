#ifndef CONVERT
#define CONVERT

#include "symLink_31.sl"
#include "symLink_76.sl"
#include "symLink_33.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern struct filesAndFoldersDbObj** globalReturnFilesAndFoldersDbObjConvert;
extern struct acceptedDbDataTypes** globalReturnacceptedDbDataTypesConvert;
extern struct envVars** globalReturnenvVarsConvert;
extern struct listProgramsToInstall** globalReturnlistProgramsToInstallConvert;
extern char* globalReturnStrConvert;
extern char globalReturnCharConvert;
extern int globalReturnIntConvert;
extern double globalReturnDoubleConvert;
extern int globalTypeConvert;
extern char acceptedDataTypesConvert[177];
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// DEFINES
//=================================================================================================


#define convertHelper(_1, _2) \
                _Generic((_1), \
		int: _Generic((_2), \
			char*: convertFromInt, \
			default: badArgPos2), \
		double: _Generic((_2), \
			char*: convertFromDouble, \
			default: badArgPos2), \
		char: _Generic((_2), \
			char*: convertFromChar, \
			default: badArgPos2), \
		char*: _Generic((_2), \
			char*: convertFromStr, \
			default: badArgPos2), \
		struct filesAndFoldersDbObj**: _Generic((_2) , \
			char*: convertFromFilesAndFoldersDbObj, \
			default: badArgPos2), \
		struct acceptedDbDataTypes**: convertFromDbObjArr_1ToCsv, \
		struct envVars**: convertFromDbObjArr_2ToCsv, \
		struct listProgramsToInstall**: convertFromDbObjArr_3ToCsv, \
                default: badArgPos1 \
		)(_1,_2); \

#define convert(_1, _2) ({ \
	void* returnData; \
	do { \
		convertHelper(_1, _2); \
		if (globalTypeConvert == 0){ returnData = NULL;} \
		else if (globalTypeConvert == 1){ returnData = (char*) globalReturnStrConvert;} \
		else if (globalTypeConvert == 2){ \
			int* returnPtr = &globalReturnIntConvert; \
			returnData = (int*) returnPtr; \
		} \
		else if (globalTypeConvert == 4){ \
			double* returnPtr = &globalReturnDoubleConvert; \
			returnData = (double*) returnPtr; \
		} \
		else if (globalTypeConvert == 5){ returnData = (struct filesAndFoldersDbObj**) globalReturnFilesAndFoldersDbObjConvert;} \
		else if (globalTypeConvert == 6){ returnData = (struct acceptedDbDataTypes**) globalReturnacceptedDbDataTypesConvert;} \
		else if (globalTypeConvert == 7){ returnData = (struct envVars**) globalReturnenvVarsConvert;} \
		else if (globalTypeConvert == 8){ returnData = (struct listProgramsToInstall**) globalReturnlistProgramsToInstallConvert;} \
	} while(0); \
	returnData; \
	}) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void badArgPos1(...);
void badArgPos2(...);
char* convertFileToString(char* filePathIn);
char* convertFromInt(int intIn, char* dataTypeToConvertTo);
char* convertFromDouble(double doubleIn, char* dataTypeToConvertTo);
void* convertFromChar(char charIn, char* dataTypeToConvertTo);
void* convertFromStr(char* strIn, char* dataTypeToConvertTo);
char* convertFromFilesAndFoldersDbObj(struct filesAndFoldersDbObj** dbObjArrIn, char* dataTypeToConvertTo);
char* convertFromDbObj_1ToStr(struct acceptedDbDataTypes* dbIn, char* dataTypeToConvertTo);
char* makeHeaderRow_1();
char* convertFromDbObjArr_1ToCsv(struct acceptedDbDataTypes** dbIn, char* dataTypeToConvertTo);
char* convertFromDbObj_2ToStr(struct envVars* dbIn, char* dataTypeToConvertTo);
char* makeHeaderRow_2();
char* convertFromDbObjArr_2ToCsv(struct envVars** dbIn, char* dataTypeToConvertTo);
char* convertFromDbObj_3ToStr(struct listProgramsToInstall* dbIn, char* dataTypeToConvertTo);
char* makeHeaderRow_3();
char* convertFromDbObjArr_3ToCsv(struct listProgramsToInstall** dbIn, char* dataTypeToConvertTo);
struct acceptedDbDataTypes** convertFromCsvToDbObjArr_1(char* csvAsStrIn);
struct envVars** convertFromCsvToDbObjArr_2(char* csvAsStrIn);
struct listProgramsToInstall** convertFromCsvToDbObjArr_3(char* csvAsStrIn);
//=================================================================================================

#endif
