#ifndef FILESANDFOLDERSBASICFUNCTIONS
#define FILESANDFOLDERSBASICFUNCTIONS

#include "symLink_76.sl" //database.h
#include "symLink_49.sl" //tableMethods.h


// GLOBAL VARS-------------------------------------------------------------------------------------
extern int arrIncFilesAndFoldersDbObj;
extern int numElementsInFandFDbObj;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

int numColsFilesAndFolderObj();
void addElementToStruct(struct filesAndFoldersDbObj* objIn, int pos, char* colVal);
char* dbObjToStr(struct filesAndFoldersDbObj* objIn);
char* dbObjArrToCsv(struct filesAndFoldersDbObj** objArrIn);
struct filesAndFoldersDbObj* sentinelFilesDbStruct();
struct filesAndFoldersDbObj** remRowFromDbArr(int colNum, void* colVal, struct filesAndFoldersDbObj** dbIn);
struct filesAndFoldersDbObj** csvToDbObjArr(char* fileInStrFormat);
//=================================================================================================

#endif
