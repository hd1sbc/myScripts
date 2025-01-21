#ifndef UPDATESTRUCT
#define UPDATESTRUCT

#include "symLink_80.sl"
#include "symLink_36.sl"
#include "symLink_77.sl"
#include "symLink_78.sl"
#include "symLink_79.sl"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

struct filesAndFoldersDbObj** addFileToFilesDbArr(struct filesAndFoldersDbObj** dbIn, char* filePathIn, char* csvPathIn);
struct filesAndFoldersDbObj** updateDbWithNewPathsHelper(char* filePathIn, struct filesAndFoldersDbObj** dbIn);
struct filesAndFoldersDbObj** updateDbWithNewPaths(struct filesAndFoldersDbObj** dbIn);
struct filesAndFoldersDbObj** removeFoundFilesFromMissingFiles(struct filesAndFoldersDbObj** missingFilesIn);
//=================================================================================================

#endif
