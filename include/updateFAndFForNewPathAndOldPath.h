#ifndef UPDATEFANDFFORNEWPATHANDOLDPATH
#define UPDATEFANDFFORNEWPATHANDOLDPATH

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


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

int updateDbArrNewCFuncs(const char* oldFilePathIn, const char* newFilePathIn);
//=================================================================================================

#endif
