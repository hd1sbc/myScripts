#ifndef TABLEMETHODSADVANCED
#define TABLEMETHODSADVANCED

#include "symLink_50.sl"
#include "symLink_99.sl"
#include "symLink_49.sl"
#include "symLink_100.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* tableColTypeLoc;
extern int filePathsColTypeListColNum;
extern int dataTypesColTypeListColNum;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

char* getColValFromId(int idIn, int colNumIn, char* dbInStrFormatIn);
char* vLookUp(char* lookUpValIn, int colNumToFindValIn, int colIndexToReturnIn, char* filePathOrFileInStrFormatIn);
char* vlookup(char* lookUpValIn, int colNumToFindValIn, int colIndexToReturnIn, char* filePathOrFileInStrFormatIn);
char* vLookup(char* lookUpValIn, int colNumToFindValIn, int colIndexToReturnIn, char* filePathOrFileInStrFormatIn);
char** getTableInfo(char* filePathIn, int colNumIn);
//=================================================================================================

#endif
