#ifndef TABLEMETHODS
#define TABLEMETHODS

#include "symLink_49.sl"
#include "symLink_99.sl"


//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

int getNumRows(char* fileInStrFormatIn);
int getNumCols(char* fileInStrFormatIn);
char* getRow(int rowNumIn, char* fileInStrFormatIn);
char* getCol(int colNumIn, char* rowIn);
//=================================================================================================

#endif
