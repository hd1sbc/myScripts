#ifndef GETSTRUCTMEMBERVAL
#define GETSTRUCTMEMBERVAL

#include "symLink_84.sl"
#include "symLink_83.sl"


//=================================================================================================
// DEFINES
//=================================================================================================


#define getStructVal(_1, _2) _Generic((_1), \
		struct acceptedDbDataTypes*: getStructMemberVal_1, \
		struct envVars*: getStructMemberVal_2, \
		struct listProgramsToInstall*: getStructMemberVal_3, \
		default: badDataTypeGetStructMemVal \
		) (_1, _2) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void* badDataTypeGetStructMemVal(...);
void* getStructMemberVal_1(struct acceptedDbDataTypes* structIn, char* memberNameIn);
void* getStructMemberVal_2(struct envVars* structIn, char* memberNameIn);
void* getStructMemberVal_3(struct listProgramsToInstall* structIn, char* memberNameIn);
//=================================================================================================

#endif
