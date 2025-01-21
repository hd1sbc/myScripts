#ifndef FINDSYSOBJBASICS
#define FINDSYSOBJBASICS

#include "symLink_93.sl"
#include "symLink_76.sl"
#include "symLink_31.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* sysFilesAndFoldersListLoc;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

char* getFilePathOrNameById(int idIn, int returnFilePathOrNameBool);
char* getFileById(int idIn);
char* getFilePathById(int idIn);
char* getFileNameById(int idIn);
int getIdFromFile(char* filePathOrNameIn);
char* getFilePathFromFileName(char* fileNameIn);
//=================================================================================================

#endif
