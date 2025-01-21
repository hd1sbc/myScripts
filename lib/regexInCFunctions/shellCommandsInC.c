#include "symLink_47.sl"
#include "symLink_38.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_SHELLCOMS = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------


// This function runs a command within a shell and returns the result as a string
char* runCommand(char* commandIn){
	// Vars
	char* FF = (char*) __func__;
	FILE* shellOutput;
	char ch;
	int arrInc = 128;
	int returnStrLen = 0;
	char* returnStr = createStr();

	// Data Validation
	if (commandIn == NULL){
		myPerror(F_SHELLCOMS, FF, "Invalid input - parameter is null. Returning null");
		return NULL;
	}
	if (strlen(commandIn) <=1){
		myPerror(F_SHELLCOMS, FF, "Invalid input - parameter is empty. Returning null");
		return NULL;
	}
	if (returnStr == NULL){
		myPerror(F_SHELLCOMS, FF, "Failed to create a return string. Returning null");
		return NULL;
	}

	// Running command and getting output as file descriptor
	shellOutput = popen(commandIn, "r");
	if (shellOutput == NULL){
		myPerror(F_SHELLCOMS, FF, "popen() c standard function returned a null value. Returning null");
		return NULL;
	}

	// Adding every char from output to return array
	while ((ch=fgetc(shellOutput)) != EOF){
		if ((returnStrLen+1)%arrInc == 0 && returnStrLen != 0){ returnStr = myRealloc(returnStr);}
		if (returnStr != NULL){ 	
			returnStr[returnStrLen] = ch;
			returnStr[returnStrLen+1] = '\0';
			returnStrLen++;
		}
		else{
			myPerror(F_SHELLCOMS, FF, "Error with myRealloc function. Returning null");
			free(returnStr);
			pclose(shellOutput);
			return NULL;
		}
	}
	pclose(shellOutput);
	
	if (strlen(returnStr) > 0 && returnStr[strlen(returnStr)-1] == '\n'){ //if ends in new line char, remove it and mark new end-of-string position
		returnStr[strlen(returnStr)-1] = '\0';
	}

	return returnStr;
}

// This function takes in a sed command (including "sed") and a string and return the output
// This is used for regex
// This function is essentially a wrapper of the runCommand() function, but makes it easier by only having to provide the command and string
char* sedC(char* sedCommand, char* strIn){
	// Vars
	char* FF = (char*) __func__;
	int commandLen;
	char* command;
	char* returnStr;
	
	// Data Validation-------------------------------------------------------------------------
	if (sedCommand == NULL){ 
		myPerror(F_SHELLCOMS, FF, "Invalid parameter 1 (sedCommand); the value is null. Returning null");
		return NULL;
	}
	if (strlen(sedCommand) == 0){
		myPerror(F_SHELLCOMS, FF, "Invalid parameter 1 (sedCommand); the string is empty. Returning null");
		return NULL;
	}
	if (strIn == NULL){
		myPerror(F_SHELLCOMS, FF, "Invalid parameter 2 (string input); the value is null. Returning null");
		return NULL;
	}
	if (strlen(strIn) == 0){ myPerror(F_SHELLCOMS, FF, "WARNING: parameter 2 (string input) is empty");}
	//-----------------------------------------------------------------------------------------

	// Vars
	commandLen = strlen(sedCommand) + strlen(strIn) + 40; //adding 40 as an arbitrary num to make room for other characters like "echo" and "|"
	
	//command. the command template is: echo strIn | sed sedCommand
	command = createStr(commandLen);
	if (command == NULL){ 
		myPerror(F_SHELLCOMS, FF, "myRealloc() function returned a null value. Returning null");
		return NULL;
	}
	strcat(command, "echo \""); strcat(command, strIn); strcat(command, "\" | "); strcat(command, sedCommand);
	
	returnStr = runCommand(command);
	free(command); 

	if (returnStr == NULL){ 
		myPerror(F_SHELLCOMS, FF, "return string returned a null value. Returning null");
		return NULL;
	}
	returnStr[strlen(returnStr)] = '\0';
	return returnStr;
}
