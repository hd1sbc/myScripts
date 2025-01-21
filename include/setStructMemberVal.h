#ifndef SETSTRUCTMEMBERVAL
#define SETSTRUCTMEMBERVAL

#include "symLink_85.sl"
#include "symLink_83.sl"


//=================================================================================================
// DEFINES
//=================================================================================================


#define setStructVal(_1, _2) _Generic((_1), \
		struct acceptedDbDataTypes*: setStructMemberVal_1, \
		struct envVars*: setStructMemberVal_2, \
		struct listProgramsToInstall*: setStructMemberVal_3, \
		default: badDataTypeSetStructMemVal \
		) (_1, _2) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void* badDataTypeSetStructMemVal(...);
void freeSetStructVal();
int setStructMemberVal_1(struct acceptedDbDataTypes* structIn, char* memberNameIn, void* valueIn);
int setStructMemberVal_2(struct envVars* structIn, char* memberNameIn, void* valueIn);
int setStructMemberVal_3(struct listProgramsToInstall* structIn, char* memberNameIn, void* valueIn);
//=================================================================================================

#endif
