#ifndef UNMARKEDFILES
#define UNMARKEDFILES

#include "symLink_79.sl"
#include "symLink_36.sl"
#include "symLink_77.sl"
#include "symLink_78.sl"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

char* runFindCommandFandFMethods(int homeDirOrRootDir);
char* appendToMissingFilesStr(char* findCommandOutput, char* missingFilesList, struct filesAndFoldersDbObj** officialObjArr);
void* unMarkedFiles(void* officialListOfFilesIn);
struct filesAndFoldersDbObj** markFilesAsNotInUse(struct filesAndFoldersDbObj** dbToUpdate, struct filesAndFoldersDbObj** listFilesNotInUse);
//=================================================================================================

#endif
