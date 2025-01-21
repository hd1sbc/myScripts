#include "symLink_97.sl"
#include "symLink_95.sl"
#include "symLink_96.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_FILEEXISTS = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function finds if the file or folder (or any system object) exists anywhere in the system by running the find command in a shell
// Returns 0 if a match isn't found, return 1 if match is found
int fileExistsInSystem(char* fileNameIn){
	// Vars
	char* FF = (char*) __func__;
	char command[1024] = "find / -name ";
	char* output;

	// Data Validation
	if (fileNameIn == NULL){ 
		myPerror(F_FILEEXISTS, FF, "Invalid parameter 1 (file name); the value is null. Returning null");
		return -1;
	}
	if (strlen(fileNameIn) == 0){
		myPerror(F_FILEEXISTS, FF, "Invalid parameter 1 (file name) is empty. Returning null");
		return -1;
	}

	strcat(command, fileNameIn);
	strcat(command, " 2</dev/null");
	
	output = runCommand(command);
	if (output == NULL){ 
		myPerror(F_FILEEXISTS, FF, "runCommand() returned a null value. Returning -1");
		return -1;
	}
	
	if (output[0] == '\0'){ free(output); return 0;} //if the output is empty, that means the file doesn't exist
	else { free(output); return 1;}
}


// This function determines if a file or folder exists. Returns 1 if exists at given path (from filePathIn var), 2 if exists, but at a different file path, and 0 if does not exist at all
// 	chechWholeSystemBooleanIn: 0 - don't check whole system. 1 - check whole system. 2 - check home dir
int fileExists(char* filePathIn, int checkWholeSystemBooleanIn){
	// Vars
	char* FF = (char*) __func__;
	struct stat buffer; //necessary struct for stat() function
	int exists;
	int returnVal;

	// Data Validation
	if (filePathIn == NULL){ 
		myPerror(F_FILEEXISTS, FF, "Invalid parameter 1 (file path); the value is null. Returning -1");
		return -1;
	}
	if (strlen(filePathIn) == 0){ 
		myPerror(F_FILEEXISTS, FF, "Invalid parameter 1 (file path) is empty. Returning -1");
		return -1;
	}

	// Checking if current file path exists
	exists = stat(filePathIn, &buffer);
	if (exists == 0){ returnVal = 1;} //stat returns 0 if a match
	else { returnVal = 0;}
	
	// Checking whole system for file name
	if (returnVal == 0 && checkWholeSystemBooleanIn == 1){
		char* fileName = getFileName(filePathIn); //removing file path to check system just for name
		printf("%s%s%s\n", "Searching through filesystem to see if", fileName, " exists");
		returnVal = fileExistsInSystem(fileName);
		free(fileName);
	}

	if (returnVal == 0 && checkWholeSystemBooleanIn == 2){
		char* fileName = getFileName(filePathIn); //removing file path to check system just for name
		printf("%s%s%s\n", "Searching through filesystem to see if", fileName, " exists");
		
		char* command = createStr();
		strcat(command, "find ~/ -name ");
		strcat(command, fileName);
		strcat(command, " 2>/dev/null");

		char* output = runCommand(command);
		if (output[0] == '\0'){ free(output); returnVal = 0;} //if the output is empty, that means the file doesn't exist
		else { free(output); returnVal = 1;}
		
		free(fileName);
		free(command);
		free(output);
	}
	
	return returnVal;
}
