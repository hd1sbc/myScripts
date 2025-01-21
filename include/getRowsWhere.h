#ifndef GETROWSWHERE
#define GETROWSWHERE

#include "symLink_91.sl"
#include "symLink_83.sl"
#include "symLink_85.sl"
#include "symLink_86.sl"
#include "symLink_87.sl"
#include "symLink_88.sl"
#include "symLink_89.sl"
#include "symLink_90.sl"


//=================================================================================================
// DEFINES
//=================================================================================================


#define getRowsWhere(_1, _2, _3, _4) _Generic((_1), \
		struct acceptedDbDataTypes**: findAllElementsFromDbArrBasedOnVal_1, \
		struct envVars**: findAllElementsFromDbArrBasedOnVal_2, \
		struct listProgramsToInstall**: findAllElementsFromDbArrBasedOnVal_3, \
		default: badDataTypeGetRowsWhere \
		) (_1, _2, _3, _4) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void* badDataTypeGetRowsWhere(...);
struct acceptedDbDataTypes** findAllElementsFromDbArrBasedOnVal_1(struct acceptedDbDataTypes** objIn, int colNum, void* val, char* equality);
struct envVars** findAllElementsFromDbArrBasedOnVal_2(struct envVars** objIn, int colNum, void* val, char* equality);
struct listProgramsToInstall** findAllElementsFromDbArrBasedOnVal_3(struct listProgramsToInstall** objIn, int colNum, void* val, char* equality);
//=================================================================================================

#endif
