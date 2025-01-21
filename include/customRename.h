#ifndef CUSTOMRENAME
#define CUSTOMRENAME

#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"
#include "dbValidationSpecial.h"
#include "fileInfoOwSysCalls.h"
#include "sourceIsInCsv.h"
#include "customOpen.h"
#include "customOpenHelpers.h"
#include "customWrite.h"
#include "updateFAndFForNewPathAndOldPath.h"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

int rename(const char* oldFilePathIn, const char* newFilePathIn);
//=================================================================================================

#endif
