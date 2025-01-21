#ifndef FILESINUSE
#define FILESINUSE

#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "symLink_77.sl"
#include "symLink_36.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern char** fileLocArr;
extern int errorBool;
extern char* myScriptsLoc;
extern char* backupsLoc;
extern char* filesAndFoldersLoc;
extern char* filesAndFoldersBackupLoc;
extern char* symLinksFolderLoc;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

struct filesAndFoldersDbObj** filesInUse(char* filesAndFoldersAsStrIn);
struct filesAndFoldersDbObj** missingOfficialFiles(char* filesAndFoldersAsStrIn);
//=================================================================================================

#endif
