#ifndef GETARRSIZE
#define GETARRSIZE

#include "symLink_39.sl"
#include "symLink_30.sl"
#include "symLink_48.sl"


//=================================================================================================
// DEFINES
//=================================================================================================


#define getArrSize(_1) _Generic((_1), \
		char*: strGetArrSize, \
		char**: strArrGetArrSize, \
		struct filesAndFoldersDbObj**: filesAndFoldersDbObjArrGetArrSize, \
		struct acceptedDbDataTypes**: acceptedDbDataTypesGetArrSize, \
		struct envVars**: envVarsGetArrSize, \
		struct listProgramsToInstall**: listProgramsToInstallGetArrSize, \
		default: wrongDataTypeGetArrSize \
		)(_1) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

int wrongDataTypeGetArrSize(...);
int nullInput();
int strGetArrSize(char* strIn);
int strArrGetArrSize(char** strArrIn);
int filesAndFoldersDbObjArrGetArrSize(struct filesAndFoldersDbObj** objIn);
int acceptedDbDataTypesGetArrSize(struct acceptedDbDataTypes** objIn);
int envVarsGetArrSize(struct envVars** objIn);
int listProgramsToInstallGetArrSize(struct listProgramsToInstall** objIn);
//=================================================================================================

#endif
