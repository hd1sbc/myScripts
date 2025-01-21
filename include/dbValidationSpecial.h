#ifndef DBVALIDATIONSPECIAL
#define DBVALIDATIONSPECIAL

#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

char** getDataTypesArrNewCFuncs(char* filePathIn);
int compareDbValuesNewCFuncs(const char* filePathIn, char* strIn);
int isValidDbNewCFuncs(const char* filePathIn, char* strIn);
//=================================================================================================

#endif
