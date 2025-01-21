#ifndef CUSTOMRMDIR
#define CUSTOMRMDIR

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
#include "customUnlink.h"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

int rmdir(const char* pathIn);
//=================================================================================================

#endif
