#ifndef REMROWONVAL
#define REMROWONVAL

#include "symLink_90.sl"
#include "symLink_83.sl"
#include "symLink_85.sl"
#include "symLink_86.sl"
#include "symLink_87.sl"
#include "symLink_88.sl"
#include "symLink_89.sl"


//=================================================================================================
// DEFINES
//=================================================================================================


#define remRowVal(_1, _2, _3) _Generic((_1), \
		struct acceptedDbDataTypes**: removeElementFromDbArrBasedOnVal_1, \
		struct envVars**: removeElementFromDbArrBasedOnVal_2, \
		struct listProgramsToInstall**: removeElementFromDbArrBasedOnVal_3, \
		default: badDataTypeRemRowVal \
		) (_1, _2, _3) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void* badDataTypeRemRowVal(...);
struct acceptedDbDataTypes** removeElementFromDbArrBasedOnVal_1(struct acceptedDbDataTypes** objIn, int colNum, void* val);
struct envVars** removeElementFromDbArrBasedOnVal_2(struct envVars** objIn, int colNum, void* val);
struct listProgramsToInstall** removeElementFromDbArrBasedOnVal_3(struct listProgramsToInstall** objIn, int colNum, void* val);
//=================================================================================================

#endif
