#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"
#include "dbValidationSpecial.h"
#include "fileInfoOwSysCalls.h"
#include "sourceIsInCsv.h"
#include "customOpen.h"
#include "customOpenHelpers.h"
#include "customWrite.h"
#include "updateFAndFForNewPathAndOldPath.h"
#include "customUnlink.h"
#include "customRmDir.h"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_NEWCFUNCS = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

/*
// DO NOT DELETE MAIN FUNCTION, EVEN IF COMMENTED OUT
// The main function is needed for compiling when using autoExec
int main(){
//	int openReadReturnVal = open(filesAndFoldersCsvLoc, O_RDONLY);
//	printf("open (read mode) return val: %d\n", openReadReturnVal);

//	int openCreateReturnVal = open("he ll", O_CREAT);
//	printf("open (create mode) return val: %d\n", openCreateReturnVal);

	// Open using higher-level c functions
	/ *FILE* file = fopen(filesAndFoldersCsvLoc, "r");
	char buffer[100];
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		printf("%s", buffer); // Print the contents of the file
	} * /

//	int closeReturnVal = close(y);
//	printf("close return val = %d\n", closeReturnVal);

//	int writeReturnVal = write(y, "hi", y);
//	printf("write return val: %d\n", writeReturnVal);

//	int renameReturnVal = rename(filesAndFoldersCsvLoc, "heee");	
//	printf("rename return val: %d\n", renameReturnVal);

//	int unlinkReturnVal = unlink(filesAndFoldersCsvLoc);
//	printf("unlink return val: %d\n", unlinkReturnVal);

//	int rmdirReturnVal = rmdir("MYSCRIPTSVAR/dep/ignore/");
//	printf("rmdir return val: %d\n", rmdirReturnVal);	

	return 0;
}
*/

// Useful commands for running
//unset LD_PRELOAD; gcc -shared -fPIC -o custom_open.o custom_open.c -ldl
//export LD_PRELOAD=...path.../custom_open.so
