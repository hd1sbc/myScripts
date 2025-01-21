#ifndef MISSINGFILES
#define MISSINGFILES

#include "symLink_78.sl"
#include "symLink_36.sl"
#include "symLink_77.sl"


//=================================================================================================
// STRUCTS AND TYPEDEFS
//=================================================================================================

typedef struct{
	int index;
	char* fileName;
} findStruct;

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void* findFileInWholeSysThread(void* findStructIn);
void missingOfficialFilesWholeSys(struct filesAndFoldersDbObj** missingFilesIn);
//=================================================================================================

#endif
