#ifndef DBVALIDATION
#define DBVALIDATION

#include "symLink_92.sl"
#include "symLink_83.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern char* filesAndFoldersCsvLoc;
extern char* filesAndFoldersCsvBakLoc;
extern char* tableColTypeListLoc;
extern int numRowsInCsv;
extern int numColsInCsv;
extern int filePathsColNum;
extern int filePathsColTypeListColNum;
extern int dataTypesColTypeListColNum;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

char** getDataTypesArr(char* filePathIn);
int compareDbValues(const char* filePathIn, char* strIn);
int isValidDb(char* filePathIn, char* strIn);
//=================================================================================================

#endif
