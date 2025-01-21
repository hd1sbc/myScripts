#ifndef CUSTOMOPEN
#define CUSTOMOPEN

#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"
#include "dbValidationSpecial.h"
#include "fileInfoOwSysCalls.h"
#include "sourceIsInCsv.h"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

int openProtectedFileWarning(char* typeIn, char* filePathIn);
int customOpen(const char* pathIn, int flagsIn, ...);
//=================================================================================================

#endif
