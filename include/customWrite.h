#ifndef CUSTOMWRITE
#define CUSTOMWRITE

#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"
#include "dbValidationSpecial.h"
#include "fileInfoOwSysCalls.h"
#include "sourceIsInCsv.h"
#include "customOpen.h"
#include "customOpenHelpers.h"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

char* getFilePathFromFd(int fdIn);
ssize_t write(int fd, const void* buf, size_t bufLen);
//=================================================================================================

#endif
