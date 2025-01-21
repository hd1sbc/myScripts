#ifndef ADDROW
#define ADDROW

#include "symLink_87.sl"
#include "symLink_83.sl"
#include "symLink_85.sl"
#include "symLink_86.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern int numCols;
extern int currDbSize;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// DEFINES
//=================================================================================================


#define addRow(_1, _2, _3) _Generic((_1), \
		struct acceptedDbDataTypes**: addElementToDbArr_1, \
		struct envVars**: addElementToDbArr_2, \
		struct listProgramsToInstall**: addElementToDbArr_3, \
		default: badDataTypeAddRow \
		) (_1, _2, _3) \

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void* badDataTypeAddRow(...);
struct acceptedDbDataTypes** addElementToDbArr_1(struct acceptedDbDataTypes** dbIn, char* rowIn, int flags);
struct envVars** addElementToDbArr_2(struct envVars** dbIn, char* rowIn, int flags);
struct listProgramsToInstall** addElementToDbArr_3(struct listProgramsToInstall** dbIn, char* rowIn, int flags);
//=================================================================================================

#endif
