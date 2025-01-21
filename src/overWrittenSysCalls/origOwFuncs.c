#include "origOwFuncs.h"
#include "genericC.h"
#include "globalVarsOwSysCalls.h"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_ORIGOWFUNCS = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function declares an empty function pointer. It will be populated by the next function definition of open() found in __attribute__ func below 
int (*orig_open)(const char*, int, ...) = NULL;

// This function declares an empty function pointer. It will be populated by the next function definition of write() found in __attribute__ func below 
ssize_t (*orig_write)(int, const void*, size_t) = NULL;

// This function declares an empty function pointer. It will be populated by the next function definition of rename() found in __attribute__ func below
int (*orig_rename)(const char*, const char*) = NULL;

// This function declares an empty function pointer. It will be populated by the next function definition of unlink() found in __attribute__ func below
int (*orig_unlink)(const char*) = NULL;

// This function declares an empty function pointer. It will be populated by the next function definition of unlink() found in __attribute__ func below
int (*orig_rmdir)(const char*) = NULL;

// This function initializes the custom open & custom write function by finding the original c functions
//the whole function definition here tells the compiler to execute this before main function. Sorta like macros/defines
void __attribute__((constructor)) init(void) {
	// Vars
	char* FF = (char*) __func__;
	
	//finds the next definition of open() function in library and sets it to the orig_open function pointer declaration
	orig_open = dlsym(RTLD_NEXT, "open"); 
	if (!orig_open) { myPerror(F_ORIGOWFUNCS, FF, "Unable to find standard c open() function. Exiting 1"); exit(1);}
	
	orig_write = dlsym(RTLD_NEXT, "write"); 
	if (!orig_write) { myPerror(F_ORIGOWFUNCS, FF, "Unable to find standard c write() function. Exiting 1"); exit(1);}
	
	orig_rename = dlsym(RTLD_NEXT, "rename"); 
	if (!orig_rename) { myPerror(F_ORIGOWFUNCS, FF, "Unable to find standard c rename() function. Exiting 1"); exit(1);}

	orig_unlink = dlsym(RTLD_NEXT, "unlink"); 
	if (!orig_unlink) { myPerror(F_ORIGOWFUNCS, FF, "Unable to find standard c unlink() function. Exiting 1"); exit(1);}

	orig_rmdir = dlsym(RTLD_NEXT, "rmdir"); 
	if (!orig_rmdir) { myPerror(F_ORIGOWFUNCS, FF, "Unable to find standard c rmdir() function. Exiting 1"); exit(1);}
}

