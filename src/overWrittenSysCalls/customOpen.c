#include "customOpen.h"
#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"
#include "dbValidationSpecial.h"
#include "fileInfoOwSysCalls.h"
#include "sourceIsInCsv.h"

// GLOBAL VARS-------------------------------------------------------------------------------------
extern int (*orig_open)(const char*, int, ...);
extern char* guiAlertScriptLoc;

// Error Msg Constant Vars
char* F_CUSTOMOPEN = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function executes the gui alert bash script that warns users of opening file
int openProtectedFileWarning(char* typeIn, char* filePathIn){
	// Vars
	char* FF = (char*) __func__;
	char* command;
	char* output;

	// Getting Values
	command = createStr(strlen(guiAlertScriptLoc)+128);
	strcat(command, "unset LD_PRELOAD; bash "); //unsetting LD_PRELOAD to avoid recurring loop of open() function
	strcat(command, guiAlertScriptLoc);
	strcat(command, " ");
	strcat(command, typeIn);
	strcat(command, " ");
	strcat(command, filePathIn);

	output = runCommand(command);
	free(command);
	if (output == NULL){ myPerror(F_CUSTOMOPEN, FF, "runCommand() returned a null value. Returning 0"); return 0;}

	if (strcmp(output, "yes\n") == 0 || strcmp(output, "yes") == 0){ free(output); return 1;}
	if (strcmp(output, "no\n") == 0 || strcmp(output, "no") == 0){ free(output); return 0;}
	else{ myPerror(F_CUSTOMOPEN, FF, "Invalid return value from gui alert. Returning 0 (don't open file value)"); free(output); return 0;}

}

// This function checks if a file should be opened
int customOpen(const char* pathIn, int flagsIn, ...) {
	// Vars
	char* FF = (char*) __func__;
	int isTargetPath;
	int perms;
	int openFileBool;
	int sourceIsInCsvVal;

	// Data Validation
	if (pathIn == NULL){ myPerror(F_CUSTOMOPEN, FF, "Invalid 1st parameter; value is null. Returning -1"); unsetPerror(FF); return -1;}

	// Getting Values
	isTargetPath = isTargetPathInCsv(pathIn);
	if (isTargetPath < 0){ myPerror(F_CUSTOMOPEN, FF, "isTargetPathInCsv returned an error value (-1) for ", (char*) pathIn, ". Returning -1"); unsetPerror(FF); return -1;}

	// Checking if path is in the csv and if the perms are not all set
	//not checking if only "read" (4) option is set cuz I wouldn't set value that way in csv
	if (isTargetPath > 0 && getPerms(pathIn, isTargetPath, 1) != 7){
		perms = getPerms(pathIn, isTargetPath, 1); //checking for errors in the getPerms() function
		if (perms < 0){ myPerror(F_CUSTOMOPEN, FF, "getPerms() function returned an error value (<0) for ", (char*) pathIn, ". Returning -1"); unsetPerror(FF); return -1;}

		// Getting extension
		char* ext = getFileExt((char*) pathIn, 2);
		if (ext == NULL){ myPerror(F_CUSTOMOPEN, FF, "getFileExt() function returned a null value for ", (char*) pathIn, ". Returning -1"); unsetPerror(FF); return -1;}

		// If the file pathIn extension is a csv (denoting a database)
		if (strcmp(ext, "csv") == 0){
			// Check if valid db
			int isValidDbNewCFuncsBool = isValidDbNewCFuncs((char*) pathIn, "");
			if (isValidDbNewCFuncsBool == 0){ //if not a valid DB, but not an error value
				myPerror(F_CUSTOMOPEN, FF, (char*) pathIn, " is not a valid database. It contains incorrect data types. Returning -1"); unsetPerror(FF); free(ext); return -1;}
			else if (isValidDbNewCFuncsBool < 0){ myPerror(F_CUSTOMOPEN, FF, "isValidDbNewCFuncs() function returned an error value (<0) for ", pathIn, ". Returning -1"); unsetPerror(FF); free(ext); return -1;
			}
		}
		free(ext);

		// Checking to see if source is in csv
		sourceIsInCsvVal = sourceIsInCsv(pathIn, flagsIn);
		if (sourceIsInCsvVal < -1){ //checking for error values from sourceIsInCsv()
			myPerror(F_CUSTOMOPEN, FF, "sourcesIsInCsv() function returned an error value (< -1) for ", pathIn, ". Returning -1");
			unsetPerror(FF);
			return -1;
		}
		if (sourceIsInCsvVal == -1){ //if not in csv
			openFileBool = openProtectedFileWarning("-o", (char*) pathIn); //show alert and get user input
			if (openFileBool == 1){	return orig_open(pathIn, flagsIn);}
			else if (openFileBool == 0){ close(sourceIsInCsvVal); return -1;}
		}
		else{ return sourceIsInCsvVal;}
	}

	return orig_open(pathIn, flagsIn); //if file path not in csv, open normally
}
