#include "globalVarsOwSysCalls.h"
#define _GNU_SOURCE
#include <dlfcn.h>
#include <fcntl.h>
#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <limits.h>

#include "genericC.h"

// GLOBAL VARS-------------------------------------------------------------------------------------
char* filesAndFoldersCsvLoc;
char* filesAndFoldersCsvBakLoc;
char* myScriptsDirLoc;
char* tableColTypeListLoc;
char* guiAlertScriptLoc;
char* execCodeDirLoc;
char* symLinkBasePath;

char* csvAsStr;
int numRowsInCsv;
int numColsInCsv;
int filePathsColNum = 3; //HARDCODED_COL_NUM
int filePathsColTypeListColNum = 2; //HARDCODED_COL_NUM
int dataTypesColTypeListColNum = 4; //HARDCODED_COL_NUM
int rweColNum = 10; //HARDCODED_COL_NUM
int rmdColNum = 11; //HARDCODED_COL_NUM

// Error Message Constant Values
char* F_NEWCFUNCS = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------


// INSTANTIATING GLOBAL VARS-----------------------------------------------------------------------
__attribute__((constructor)) void init_NEWCFUNCS() {
	filesAndFoldersCsvLoc = myFindPathC(79); //filesAndFolders.csv
	filesAndFoldersCsvBakLoc = myFindPathC(207); // filesAndFolders.csv.bak
	myScriptsDirLoc = myFindPathC(1); strcat(myScriptsDirLoc, "/"); //linuxUtils
	tableColTypeListLoc = myFindPathC(357); //tableColTypeList.csv
	guiAlertScriptLoc = myFindPathC(374); //sysCallsGuiWarnings.sh
	execCodeDirLoc = myFindPathC(109); strcat(execCodeDirLoc, "/"); //executeCode

	symLinkBasePath = myFindPathC(146);
	symLinkBasePath = myRealloc(10, symLinkBasePath);
	strcat(symLinkBasePath, "/symLink_"); //symLinks/symLink_
}
__attribute__((destructor)) void deinit_NEWCFUNCS() {
	free(filesAndFoldersCsvLoc);
	free(filesAndFoldersCsvBakLoc);
	free(myScriptsDirLoc);
	free(tableColTypeListLoc);
	free(guiAlertScriptLoc);
	free(execCodeDirLoc);
	free(symLinkBasePath);
}
//-------------------------------------------------------------------------------------------------
