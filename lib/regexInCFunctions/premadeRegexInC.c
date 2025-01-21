#include "symLink_82.sl"
#include "symLink_38.sl"
#include "symLink_47.sl"
#include "symLink_81.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_PREMADERX = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function determines if the input matches my standard option for a help menu (-h or --help)
int isHelpMenu(char* argIn){
	// Vars
	char firstArg[7];

	//the input might be empty. If the arg doesn't exist, that arg wasn't provided to the c program. No need to print an error message
	if (argIn == NULL){ return 0;}
	
	for (int i = 0; argIn[i] != '\0'; i++){
		if (i == 2){
			firstArg[2] = '\0';
			if (strcmp(firstArg, "-h") == 0){ return 1;}
			if (strcmp(firstArg, "--") != 0){ return 0;}
		}
		if (i == 6){
			firstArg[6] = '\0';
			if (strcmp(firstArg, "--help") == 0){ return 1;}
			else {return 0;}
		}
		
		firstArg[i] = argIn[i];
	}

	return 0; //if i < 6 chars long but doesn't contain -h or --help
	//return regexMatch("^\\(-h\\|--help\\)$", argIn);
}

// This function checks if a string is an integer. Returns 1 if true and 0 if false
// This function accepts negative numbers
int isInt(char* strIn){
	// Vars
	char* FF = (char*) __func__;
	
	// Data Validation
	if (strIn == NULL){
		myPerror(F_PREMADERX, FF, "Invalid parameter (string input); the value is null. Returning -1");
		return -1;
	}
	if (strlen(strIn) == 0){ return 0;}

	// Testing if each char is a string and/or if first char is '-'
	for (int i = 0; strIn[i] != '\0'; i++){
		if (!isdigit(strIn[i]) && !(i == 0 && strIn[i] == '-')){ return 0;}
	}

	return 1;
}

// This function checks if a string is a decimal. Returns 1 if true and 0 if false
// This function requires input to have a decimal point. Leading 0 is not required. Accepts negative numbers
int isDouble(char* strIn){
	// Vars
	char* FF = (char*) __func__;
	char* isDoubleBool;

	// Data Validation
	if (strIn == NULL){
		myPerror(F_PREMADERX, FF, "Invalid parameter (string input); the value is null. Returning -1");
		return -1;
	}

	//regex: from beginning of line, optional negative sign followed by any number 0-9 zero or more times followed by a decimal point followed by any number 0-9 one or more times
	//two backslashes are required. 1st is to let the c program know the next backslash is a literal
	isDoubleBool = sedC("sed -n '/^-\\?[0-9]*\\.[0-9]\\+$/p'", strIn);
	if (isDoubleBool == NULL){
		myPerror(F_PREMADERX, FF, "sedC() returned a null value. Returning -1");
		return -1;
	}
	else if (strlen(isDoubleBool) == 0){ free(isDoubleBool); return 0;}
	else { free(isDoubleBool); return 1;}
	//not bothering to check for size of double cuz I'm lazy
}

// This function checks if the string is either an int or decimal. Returns 1 if true and 0 if false
int isNum(char* strIn){
	// Vars
	char* FF = (char*) __func__;
	
	// Data Validation
	if (strIn == NULL){ 
		myPerror(F_PREMADERX, FF, "Invalid parameter (string input); the value is null. Returning -1");
		return -1;
	}
	
	if (isInt(strIn) == 1 || isDouble(strIn) == 1){ return 1;}
	else{ return 0;}
}

// This function checks if the string is either a positive int or double. Returns 1 if true and 0 if false
int isPositiveNum(char* strIn){
	// Vars
	char* FF = (char*) __func__;
	
	// Data Validation
	if (strIn == NULL){ 
		myPerror(F_PREMADERX, FF, "Invalid parameter (string input); the value is null. Returning -1");
		return -1;
	}
	
	if (isInt(strIn) == 1){ 
		int x = atoi(strIn);
		if (x >= 0){ return 1;}
		else{ return 0;}
	}
	else if (isDouble(strIn) == 1){
		double x = atof(strIn);
		if (x >= 0){ return 1;}
		else {return 0;}
	}
	else { return 0;}
}

// This function check is the string is a 0 or 1 (representing false and true)
int isBool(char* strIn){
	// Vars
	char* FF = (char*) __func__;

	// Data Validation
	if (strIn == NULL){ 
		myPerror(F_PREMADERX, FF, "Invalid parameter (string input); the value is null. Returning -1");
		return -1;
	}

	if (regexMatch("^\\(0\\|1\\)$", strIn)){ return 1;}
	else{ return 0;}
}
