#include "customRmDir.h"
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

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_CUSTOMRMDIR = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function overwrites c standard library/syscall rename() function
//This just uses the unlink() function since it can determine if a path a dir or not (and act accordingly)
int rmdir(const char* pathIn){
	// Vars
	char* FF = (char*) __func__;

	int unlinkReturnVal = unlink(pathIn);
	if (unlinkReturnVal < 0){
		myPerror(F_CUSTOMRMDIR, FF, "unlink() function returned an error value (<0). Returning -1");
		return -1;		
	}

	return unlinkReturnVal;
}
