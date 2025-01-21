#ifndef LOADINGBARC
#define LOADINGBARC

#include <pthread.h>
#include <unistd.h>
#include "symLink_48.sl"
#include "symLink_30.sl"


// GLOBAL VARS-------------------------------------------------------------------------------------
extern int progressLb;
//-------------------------------------------------------------------------------------------------

//=================================================================================================
// STRUCTS AND TYPEDEFS
//=================================================================================================

typedef struct{
	int expectedDur;
	int removeTextBool;
	char* loadingBarMsg;
} lbParams;

//=================================================================================================

//=================================================================================================
// FUNCTION DECLARATIONS
//=================================================================================================

void loadingBarCProcEndedEarly();
void closeLoadingBarC(lbParams* lbParamsIn);
void* loadingBarCThread(void* lbParamsIn);
lbParams* loadingBarC(int expectedDurIn, int removeTextBoolIn, char* loadingBarMsgIn);
//=================================================================================================

#endif
