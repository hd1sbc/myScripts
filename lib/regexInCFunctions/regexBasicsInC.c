#include "symLink_81.sl"
#include "symLink_38.sl"
#include "symLink_47.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_RXBASICS = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function is used to match regex patterns (using sed command). If a match, returns 1, else returns 0
int regexMatch(char* regexIn, char* strIn){
	// Vars
	char* FF = (char*) __func__;
	int commandLen;
	char* command;
	char* isMatchStr;
	
	// Data Validation-------------------------------------------------------------------------
	if (regexIn == NULL){ 
		myPerror(F_RXBASICS, FF, "Invalid parameter 1 (regex); the value is null. Returning -1");
		return -1;
	}
	if (strlen(regexIn) == 0){
		myPerror(F_RXBASICS, FF, "Invalid parameter 1 (regex); the string is empty. Returning -1");
		return -1;
	}
	if (strIn == NULL){
		myPerror(F_RXBASICS, FF, "Invalid parameter 2 (string input); the value is null. Returning -1");
		return -1;
	}
	if (strlen(strIn) == 0){ myPerror(F_RXBASICS, FF, "WARNING: parameter 2 (string input) is empty");}
	//-----------------------------------------------------------------------------------------

	// Getting Values
	commandLen = strlen(regexIn) + 40; //adding 40 as an arbitrary num to make room for other characters like "sed -n"
	command = createStr(commandLen+1); //command. Command template is: sed -n '/regexIn/p'
	strcat(command, "sed -n '/"); strcat(command, regexIn); strcat(command, "/p'");
	
	// Checking if match
	isMatchStr = sedC(command, strIn);
	free(command);
	if (isMatchStr == NULL){
		myPerror(F_RXBASICS, FF, "sedC() function returned a null value. Returning -1");
		return -1;
	}
	
	if (strlen(isMatchStr) > 0){ free(isMatchStr); return 1;}
	else{ free(isMatchStr); return 0;}
}

// This function counts the number of times a pattern is found in a string
// Used in time sensitive code instead of regex cuz regex requires opening a shell
int stringContains(char* patternIn, char* strIn){
	// Vars
	char* FF = (char*) __func__;
	int lenStr;
	int lenPat;
	char* currStrAtPatternLen;
	int numMatches = 0;

	// Data Validation
	if (patternIn == NULL){
		myPerror(F_RXBASICS, FF, "Invalid 1st parameter (pattern); value is null. Returning 0");
		return 0;
	}
	if (strIn == NULL){
		myPerror(F_RXBASICS, FF, "Invalid 2nd parameter (string); value is null. Returning 0");
		return 0;
	}
	if (strlen(patternIn) == 0){
		myPerror(F_RXBASICS, FF, "Invalid 1st parameter (pattern); value is empty string. Returning 0");
		return 0;
	}
	//-----------------------------------------------------------------------------------------
	
	// Getting Values
	lenStr = strlen(strIn);
	lenPat = strlen(patternIn);
	currStrAtPatternLen = createStr(lenPat+1);

	// Iterating over every element in the list
	for (int i = 0; i < lenStr; i++){
		// If i < length of pattern, add the curr char at i to curr pattern string
		if (i < lenPat){
			currStrAtPatternLen[i] = strIn[i];
			currStrAtPatternLen[i+1] = '\0';
		}
		// If i >= length of pattern, replace the last element in curr pattern string with curr char
		else{
			currStrAtPatternLen[lenPat-1] = strIn[i];
			currStrAtPatternLen[lenPat] = '\0';
		}
		
		// If i >= length of pattern, compare the curr pattern string with the pattern & remove 1st char from curr pattern string
		// subtracting 1 cuz lenPat is string length (beginning effectively at 1) while this function works with indices (beginning with 0)
		if (i >= lenPat-1){
			if (strcmp(currStrAtPatternLen, patternIn) == 0){ numMatches++;}
			
			// Remove 1st element from curr pattern string and move all other elements 1 index lower
			for (int j = 1; j < lenPat; j++){
				currStrAtPatternLen[j-1] = currStrAtPatternLen[j];
			}
			currStrAtPatternLen[lenPat-1] = '\0';
		}
		
	}

	free(currStrAtPatternLen);
	return numMatches;
}
