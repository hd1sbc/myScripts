#ifndef FILEINFOOWSYSCALLS
#define FILEINFOOWSYSCALLS

#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"
#include "dbValidationSpecial.h"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

int isTargetPathInCsv(const char* pathIn);
int getPerms(const char* pathIn, int rowNum, int rweBool);
//=================================================================================================

#endif
