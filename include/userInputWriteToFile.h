#ifndef USERINPUTWRITETOFILE
#define USERINPUTWRITETOFILE

#include "updateFAndF.h"
#include "userInputMissingFiles.h"
#include "userInputUnmarkedFiles.h"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

int writeToFandFCsv(char* filesListIn, struct filesAndFoldersDbObj** dbIn, char* csvPathIn);
//=================================================================================================

#endif
