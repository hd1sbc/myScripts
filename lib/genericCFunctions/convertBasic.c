#include "symLink_100.sl"
#include "symLink_99.sl"
#include "symLink_49.sl"

// INPUT STRUCTURE---------------------------------------------------------------------------------
// <datatype> <convert to data type written as a string>
//-------------------------------------------------------------------------------------------------


// GLOBAL VARS-------------------------------------------------------------------------------------
char* globalReturnStrConvertBasic = NULL;
char globalReturnCharConvertBasic;
int globalReturnIntConvertBasic;
double globalReturnDoubleConvertBasic;
int globalTypeConvertBasic = 0;
char acceptedDataTypesConvertBasic[1024] = "string/char*, int, char, double";

// Error Msg Constant Vars
char* F_CONVERTB = (char*) __FILE__;

// Return Types (for reference only):
// 	1 :- string/char*
// 	2 :- int
// 	3 :- char
// 	4 :- double

//-------------------------------------------------------------------------------------------------


//=================================================================================================
// PRINT STATEMENT - ERRORS
//=================================================================================================
void badArgPos1ConvertBasic(...){ perror("===CONVERT BASIC===	Invalid data type at position 1. Accepted data types are: "); perror(acceptedDataTypesConvertBasic);}
void badArgPos2ConvertBasic(...){ perror("===CONVERT BASIC===	Invalid data type at position 2. Expected a char* or string that denotes the data type to convert parameter 1 to.");}
//=================================================================================================


//=================================================================================================
// CODE
//=================================================================================================

// This function converts a file to a string with the input as the file path
char* convertBasicFileToString(char* filePathIn){
	// Vars
	FILE* file;
	char ch;
	int fileLen = 0;
	char* returnStr = (char*) malloc(sizeof(char));
	char* FF = (char*) __func__;

	file = fopen(filePathIn, "r"); //opens input file
        if (file == NULL){ myPerror(F_CONVERTB, FF, "File not found"); fclose(file); return NULL;} // Data Validation

	// Adding each char to return string
	while((ch = fgetc(file)) != EOF){ //while not end-of-file char
		returnStr = realloc(returnStr, sizeof(char)*(fileLen+1));//resizing
		returnStr[fileLen] = ch; //adding current char to end of array
		fileLen++;
	}

	// Adding end-of-string char (\0) to returnStr. This is important for string manipulation
	returnStr = realloc(returnStr, sizeof(char)*(fileLen+1));//resizing
	returnStr[fileLen-1] = '\0'; //adding end-of-string char to end of array. Doing it at pos-1 cuz files usually end with '\n', and I usually don't want that

        fclose(file);
        return returnStr;
}

// This function converts from an int to a string
char* convertBasicFromInt(int intIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;
	
	if (strcmp(dataTypeToConvertTo, "char*") == 0 || strcmp(dataTypeToConvertTo, "string") == 0 ||
			strcmp(dataTypeToConvertTo, "s") == 0){
		globalReturnStrConvertBasic = malloc(sizeof(char)*100); //chose value of 100 arbitrarily to give enough room for int
		snprintf(globalReturnStrConvertBasic, 100, "%d", intIn);
		globalTypeConvertBasic = 1;
		return globalReturnStrConvertBasic;
	}
	else {
		myPerror(F_CONVERTB, FF, "Invalid data type to convert to. Can only convert int to string. Returning null");
		myPerror(F_CONVERTB, FF, "	Valid 2nd parameter strings: char*, string, s");
		return NULL;
	}
}

// This function converts from a double to a string
char* convertBasicFromDouble(double doubleIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;
	
	if (strcmp(dataTypeToConvertTo, "char*") == 0 || strcmp(dataTypeToConvertTo, "string") == 0 ||
			strcmp(dataTypeToConvertTo, "s") == 0){
		globalReturnStrConvertBasic = malloc(sizeof(char)*100); //chose value of 100 arbitrarily to give enough room for int
		snprintf(globalReturnStrConvertBasic, 100, "%f", doubleIn);
		globalTypeConvertBasic = 4;
		return globalReturnStrConvertBasic;
	}
	else {
		myPerror(F_CONVERTB, FF, "Invalid data type to convert to. Can only convert double to string. Returning null");
		myPerror(F_CONVERTB, FF, "	Valid 2nd parameter strings: char*, string, s");
		return NULL;
	}

}


// This function converts from a char to a string or int
void* convertBasicFromChar(char charIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;

	if (strcmp(dataTypeToConvertTo, "char*") == 0 || strcmp(dataTypeToConvertTo, "string") == 0 ||
			strcmp(dataTypeToConvertTo, "s") == 0){
		globalReturnStrConvertBasic = createStr(2);
		globalReturnStrConvertBasic[0] = charIn;
		globalReturnStrConvertBasic[1] = '\0';
		globalTypeConvertBasic = 1;
		return globalReturnStrConvertBasic;
	}
	else if (strcmp(dataTypeToConvertTo, "int") == 0 || strcmp(dataTypeToConvertTo, "d") == 0){
		globalReturnIntConvertBasic = charIn - '0'; //I'm not doing any data validation to check if charIn is actually an int cuz I'm too lazy
		globalTypeConvertBasic = 2;
		
		int* returnPtr = &globalReturnIntConvertBasic; //converting to pointer cuz void* doesn't accept non-pointer data types
		return returnPtr;
	}
	else {
		myPerror(F_CONVERTB, FF, "Invalid data type to convert to. Can only convert char to string and int. Returning null");
		myPerror(F_CONVERTB, FF, "	Valid 2nd parameter strings: char*, string, s, int, d");
		return NULL;
	}
	
}

//This function converts from a string to a: 1) string (where the input is the file path), 2) int, 3) double, 4) char, 5) fileDbObj**
void* convertBasicFromStr(char* strIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;
	
	// Convert file to string
	if (strcmp(dataTypeToConvertTo, "char*") == 0 || strcmp(dataTypeToConvertTo, "string") == 0 ||
			strcmp(dataTypeToConvertTo, "s") == 0){
		//if the string input contains a '/' or a '.', then I treat it as a file path or file name and will convert the file to a string
		if (regexMatch("\\/", strIn) == 1 || regexMatch("\\.", strIn) == 1){
			globalReturnStrConvertBasic = convertBasicFileToString(strIn);
			globalTypeConvertBasic = 1;
			return globalReturnStrConvertBasic;
		}
		else{
			myPerror(F_CONVERTB, FF, "Invalid file name and/or path. No forward slashes or periods were detected in the string. Returning null");
			return NULL;
		}
	}
	
	// Convert string to int
	else if (strcmp(dataTypeToConvertTo, "int") == 0 || strcmp(dataTypeToConvertTo, "d") == 0){
		if (isInt(strIn) == 1){
			globalReturnIntConvertBasic = atoi(strIn);
			globalTypeConvertBasic = 2;
			
			int* returnPtr = &globalReturnIntConvertBasic; //converting to pointer cuz void* doesn't accept non-pointer data types
			return returnPtr;

		}
		else {
			myPerror(F_CONVERTB, FF, "Invalid string to convert; the value is not an integer. Returning null");
			return NULL;
		}
	}
	
	// Convert string to double. Using "float" and "f" cuz printf formatting doesn't have an option for double, only float
	else if (strcmp(dataTypeToConvertTo, "float") == 0 || strcmp(dataTypeToConvertTo, "f") == 0){
		if (isDouble(strIn) == 1){
			globalReturnDoubleConvertBasic = atof(strIn);
			globalTypeConvertBasic = 4;
			
			double* returnPtr = &globalReturnDoubleConvertBasic; //converting to pointer cuz void* doesn't accept non-pointer data types
			return returnPtr;
		}
		else {
			myPerror(F_CONVERTB, FF, "Invalid string to convert; the value is not a double. Returning null");
			return NULL;
		}
	}
	
	// Convert to char
	else if (strcmp(dataTypeToConvertTo, "char") == 0 || strcmp(dataTypeToConvertTo, "c") == 0){
		globalReturnCharConvertBasic = strIn[0];
		globalTypeConvertBasic = 3;

		char* returnPtr = &globalReturnCharConvertBasic;
		return returnPtr; //converting to pointer cuz void* doesn't accept non-pointer data types
	}
		
	// Invalid data type
	else{
		myPerror(F_CONVERTB, FF, "Invalid data type \"", dataTypeToConvertTo, "\" to convert to. Returning null. Can only convert char to:");
		myPerror(F_CONVERTB, FF, "	int, double, char, and string (only when converting from a file where the input is the file path");
		myPerror(F_CONVERTB, FF, "	Valid 2nd parameter strings: char*, string, s, int, d, float, f, char, c");
		return NULL;
	}
}

//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define convertBasicHelper(_1, _2) \
                _Generic((_1), \
                int: _Generic((_2), \
                        char*: convertBasicFromInt, \
                        default: badArgPos2ConvertBasic), \
                double: _Generic((_2), \
                        char*: convertBasicFromDouble, \
                        default: badArgPos2ConvertBasic), \
                char: _Generic((_2), \
                        char*: convertBasicFromChar, \
                        default: badArgPos2ConvertBasic), \
                char*: _Generic((_2), \
                        char*: convertBasicFromStr, \
                        default: badArgPos2ConvertBasic), \
                default: badArgPos1ConvertBasic \
                )(_1,_2); \


#define convertBasic(_1, _2) ({ \
        void* returnData; \
        do { \
                convertBasicHelper(_1, _2); \
                if (globalTypeConvertBasic == 0){ returnData = NULL;} \
                else if (globalTypeConvertBasic == 1){ returnData = (char*) globalReturnStrConvertBasic;} \
                else if (globalTypeConvertBasic == 2){ \
                        int* returnPtr = &globalReturnIntConvertBasic; \
                        returnData = (int*) returnPtr; \
                } \
		else if (globalTypeConvertBasic == 4){ \
                        double* returnPtr = &globalReturnDoubleConvertBasic; \
                        returnData = (double*) returnPtr; \
                } \
        } while(0); \
        returnData; \
        }) \

*/
//=================================================================================================

// INPUT STRUCTURE---------------------------------------------------------------------------------
// <datatype> <convert to data type written as a string>
//-------------------------------------------------------------------------------------------------



// Return Types: (for reference)
// 	1 :- string/char*
// 	2 :- int
// 	3 :- char
// 	4 :- double
