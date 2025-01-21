#ifndef FILEREGEXINC
#define FILEREGEXINC

#include <limits.h>
#include <sys/stat.h>
#include "symLink_96.sl"
#include "symLink_95.sl"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

char* getFileName(char* filePathIn);
char* getFilePathNoName(char* filePathIn);
char* getFileExt(char* filePathIn, int surpressErrors);
char systemObjType(char* filePathIn);
int isInDir(char* dirIn, char* filePathIn);
//=================================================================================================

#endif
