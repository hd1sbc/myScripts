#ifndef DEEPCPY
#define DEEPCPY

#include "symLink_88.sl"
#include "symLink_83.sl"
#include "symLink_85.sl"
#include "symLink_86.sl"
#include "symLink_87.sl"


//=================================================================================================
// DEFINES
//=================================================================================================


#define deepCpy(_1) _Generic((_1), \
		struct acceptedDbDataTypes*: deepCpyDbObj_1, \
		struct acceptedDbDataTypes**: deepCpyDbObjArr_1, \
		struct envVars*: deepCpyDbObj_2, \
		struct envVars**: deepCpyDbObjArr_2, \
		struct listProgramsToInstall*: deepCpyDbObj_3, \
		struct listProgramsToInstall**: deepCpyDbObjArr_3, \
		default: badDataTypeDeepCpy \
		) (_1) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void* badDataTypeDeepCpy(...);
struct acceptedDbDataTypes* deepCpyDbObj_1(struct acceptedDbDataTypes* objToCopy);
struct acceptedDbDataTypes** deepCpyDbObjArr_1(struct acceptedDbDataTypes** objToCopy);
struct envVars* deepCpyDbObj_2(struct envVars* objToCopy);
struct envVars** deepCpyDbObjArr_2(struct envVars** objToCopy);
struct listProgramsToInstall* deepCpyDbObj_3(struct listProgramsToInstall* objToCopy);
struct listProgramsToInstall** deepCpyDbObjArr_3(struct listProgramsToInstall** objToCopy);
//=================================================================================================

#endif
