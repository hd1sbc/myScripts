#ifndef FINDSYSOBJSYMLINK
#define FINDSYSOBJSYMLINK

#include "symLink_94.sl"
#include "symLink_76.sl"
#include "symLink_31.sl"
#include "symLink_93.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* symLinkFilePrefix;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

int isSymLinkAtId(int idIn);
char* getSymLinkById(int idIn);
int getSymLinkIdFromFileName(char* fileNameIn);
//=================================================================================================

#endif
