#ifndef CUSTOMOPENHELPERS
#define CUSTOMOPENHELPERS

#include "updateFAndF.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"
#include "dbValidationSpecial.h"
#include "fileInfoOwSysCalls.h"
#include "sourceIsInCsv.h"
#include "customOpen.h"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

int isValidFileName(char* pathIn);
int addFileToCsvNewCFuncs(char* pathIn);
int open(const char* pathname, int flags, ...);
//=================================================================================================

#endif
