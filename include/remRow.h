#ifndef REMROW
#define REMROW

#include "symLink_89.sl"
#include "symLink_83.sl"
#include "symLink_85.sl"
#include "symLink_86.sl"
#include "symLink_87.sl"
#include "symLink_88.sl"


//=================================================================================================
// DEFINES
//=================================================================================================


#define remRow(_1, _2) _Generic((_1), \
		struct acceptedDbDataTypes**: removeElementFromDbArr_1, \
		struct envVars**: removeElementFromDbArr_2, \
		struct listProgramsToInstall**: removeElementFromDbArr_3, \
		default: badDataTypeRemRow \
		) (_1, _2) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void* badDataTypeRemRow(...);
struct acceptedDbDataTypes** removeElementFromDbArr_1(struct acceptedDbDataTypes** objIn, int idIn);
struct envVars** removeElementFromDbArr_2(struct envVars** objIn, int idIn);
struct listProgramsToInstall** removeElementFromDbArr_3(struct listProgramsToInstall** objIn, int idIn);
//=================================================================================================

#endif
