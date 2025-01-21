#ifndef PRINTTABLE
#define PRINTTABLE

#include "symLink_46.sl"
#include "symLink_30.sl"
#include "symLink_48.sl"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void centerTableElement(char* val, int colWidth);
void printHeaderRowFilesAndFolders();
void printFilesDbObj(struct filesAndFoldersDbObj* objIn);
void printTable(void* obj);
//=================================================================================================

#endif
