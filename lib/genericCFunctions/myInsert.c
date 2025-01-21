#include "symLink_43.sl"
#include "symLink_76.sl"

//FUNCTIONS STRUCTURE------------------------------------------------------------------------------
// MY INSERT INPUT STRUCTURE
// <originalArr>, <arrToAdd>, <pos>

// MY APPEND INPUT STRUCTURE
// <originalArr>, <arrToAdd>
//-------------------------------------------------------------------------------------------------


// GLOBAL VARS-------------------------------------------------------------------------------------
int globalMyInsertType = -1;
char* returnStrMyInsert;
char** returnStrArrMyInsert;

// Error Msg Constant Vars
char* F_MYINSERT = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------


//=================================================================================================
// STRINGS
//=================================================================================================

// This function inserts a string to an existing string
char* insertStr(char* strIn, char* strToAdd, int pos){
	
	// Vars
	int currOriginalStrPos = 0;
	int strToAddLen = strlen(strToAdd);
	int strOriginalLen = strlen(strIn);
	int newLen = strOriginalLen+strToAddLen;
	char* FF = (char*) __func__;
	
	// Data validation-------------------------------------------------------------------------
	if (strOriginalLen == -1){
		myPerror(F_MYINSERT, FF, "Invalid original string. getArrSize() function returned -1");
		return NULL;
	}
	if (pos > strOriginalLen+1){
		myPerror(F_MYINSERT, FF, "Invalid position. Position is greater than length of string");
		return strIn;
	}
	if (strIn == NULL){
		myPerror(F_MYINSERT, FF, "Invalid input - original string is NULL");
		return strIn;
	}
	if (strToAdd == NULL){
		myPerror(F_MYINSERT, FF, "Invalid input - string-to-add is NULL");
		return strIn;
	}
	//-----------------------------------------------------------------------------------------
	
	returnStrMyInsert = createStr(newLen+1);

	for (int i = 0; i < newLen; i++){
		if (i < pos){ //if i<pos, then copy original string into return string
			returnStrMyInsert[i] = strIn[i];
			currOriginalStrPos++;
		}
		else if (i == pos){ //if i == pos, then add string-to-add to return string
			returnStrMyInsert[i] = '\0'; //add end-of-string char so that you can use returnStr with strcat
			strcat(returnStrMyInsert, strToAdd);
			i+=strToAddLen-1; //increment i by length of string-to-add. Subtract 1 cuz i will increment again at end of loop
		}
		else{ //add rest of original string after pos index to return string
			returnStrMyInsert[i] = strIn[currOriginalStrPos];
			currOriginalStrPos++;
		} 
	}

	returnStrMyInsert[newLen] = '\0'; //add end-of-string char
	
	globalMyInsertType = 0; //setting global return type to char*
	free(strIn);
	return returnStrMyInsert;
}

// This function inserts an int to a string
char* insertIntToStr(char* strIn, int intToAdd, int pos){
	char intInStrFormat[100]; //string var to hold int. Arbitrary size of 100
	snprintf(intInStrFormat, sizeof(intInStrFormat), "%d", intToAdd); //convert int to string
	returnStrMyInsert = insertStr(strIn, intInStrFormat, pos);
	return returnStrMyInsert;
}

// This function inserts an char to a string
char* insertChar(char* strIn, char charToAdd, int pos){
	char charInStrFormat[2]; //string var to hold char. Size of 2 so it can hold char value followed by end-of-string char
	charInStrFormat[0] = charToAdd;
	charInStrFormat[1] = '\0';
	returnStrMyInsert = insertStr(strIn, charInStrFormat, pos);
	return returnStrMyInsert;
}
//=================================================================================================


//=================================================================================================
// STRING ARRAYS
//=================================================================================================

// This function inserts a string to a string array
char** insertStrToStrArr(char** strArrIn, char* strToAdd, int pos){
	
	// Vars
	int currOriginalArrPos = 0;
	int strArrLen = getArrSize(strArrIn);
	int strToAddLen = strlen(strToAdd);
	int newLen = strArrLen + 1;
	char* FF = (char*) __func__;
	
	// Data validation-------------------------------------------------------------------------
	if (strArrLen == -1){
		myPerror(F_MYINSERT, FF, "Invalid original string array. getArrSize() function returned -1");
		return NULL;
	}
	if (pos > strArrLen+1){
		myPerror(F_MYINSERT, FF, "Invalid position. Position is greater than length of string array");
		return strArrIn;
	}
	if (strArrIn == NULL){
		myPerror(F_MYINSERT, FF, "Invalid input - original string array is NULL");
		return strArrIn;
	}
	if (strToAdd == NULL){
		myPerror(F_MYINSERT, FF, "Invalid input - string-to-add is NULL");
		return strArrIn;
	}
	//-----------------------------------------------------------------------------------------

	returnStrArrMyInsert = malloc(sizeof(char*)*(newLen+1));

	for (int i = 0; i < newLen; i++){
		if (i < pos){ //if i<pos, then copy original string to return string array at index i
			returnStrArrMyInsert[i] = createStr(strlen(strArrIn[i])+1);
			strcpy(returnStrArrMyInsert[i], strArrIn[i]);
			currOriginalArrPos++;
		}
		if (i == pos){ //if i==pos, then add string-to-add to return string array at index i
			returnStrArrMyInsert[i] = createStr(strToAddLen+1);
			strcpy(returnStrArrMyInsert[i], strToAdd);
		}
		else {
		       	//add rest of original array after pos index to return string array
			returnStrArrMyInsert[i] = createStr(strlen(strArrIn[currOriginalArrPos])+1);
			strcpy(returnStrArrMyInsert[i], strArrIn[currOriginalArrPos]);
			currOriginalArrPos++;
		}
	}

	returnStrArrMyInsert[newLen] = NULL; //add sentinel value for return string
	globalMyInsertType = 1; //setting global return type to char**
	myFree(strArrIn);
	return returnStrArrMyInsert;
}

// This function inserts a string array to an existing string array
char** insertStrArrToStrArr(char** strArrIn, char** strArrToAdd, int pos){
	
	// Vars
	int currOriginalArrPos = 0;
	int strArrLen = getArrSize(strArrIn);
	int strArrToAddLen = getArrSize(strArrToAdd);
	int newLen = strArrToAddLen + strArrLen;
	char* FF = (char*) __func__;

	// Data Validation-------------------------------------------------------------------------
	if (strArrLen == -1){
		myPerror(F_MYINSERT, FF, "Invalid original string array. getArrSize() function returned -1");
		return NULL;
	}
	if (pos > strArrLen+1){
		myPerror(F_MYINSERT, FF, "Invalid position. Position is greater than length of string array");
		return strArrIn;
	}
	if (strArrIn == NULL){
		myPerror(F_MYINSERT, FF, "Invalid input - original string array is NULL");
		return strArrIn;
	}
	if (strArrToAdd == NULL){
		myPerror(F_MYINSERT, FF, "Invalid input - string-to-add is NULL");
		return strArrIn;
	}
	//-----------------------------------------------------------------------------------------

	returnStrArrMyInsert = malloc(sizeof(char*)*(newLen+1));

	for (int i = 0; i < newLen; i++){
		if (i < pos){ //if i<pos, then copy original string to return string array at index i
			returnStrArrMyInsert[i] = createStr(strlen(strArrIn[i])+1);
			//Copying instead of setting pointer since I'm returning the return array, not modifying strArrIn
			strcpy(returnStrArrMyInsert[i], strArrIn[i]);
			free(strArrIn[i]); //free the strArrToAdd since I copied it
			currOriginalArrPos++;
		}
		else if (i == pos){ //if i==pos, then add string-to-add to return string array at index i
			for (int j = 0; j < strArrToAddLen; j++){
				//Copying instead of setting pointer in case string-array-to-add was free'd somewhere else
				returnStrArrMyInsert[i+j] = createStr(strlen(strArrToAdd[j])+1);
				strcpy(returnStrArrMyInsert[i+j], strArrToAdd[j]);
			}
			i+=strArrToAddLen-1;
		}
		else { //add rest of original array after pos index to return string array
			returnStrArrMyInsert[i] = createStr(strlen(strArrIn[currOriginalArrPos])+1);
			//Copying instead of setting pointer since I'm returning the return array, not modifying strArrIn
			strcpy(returnStrArrMyInsert[i], strArrIn[currOriginalArrPos]);
			free(strArrIn[currOriginalArrPos]); //free the strArrToAdd since I copied it
			currOriginalArrPos++;
		}
	}

	returnStrArrMyInsert[newLen] = NULL; //add sentinel value for return string
	globalMyInsertType = 1; //setting global return type to char**
	free(strArrIn);
	return returnStrArrMyInsert;
}
//=================================================================================================


//=================================================================================================
// PRINT STATEMENTS - ERRORS
//=================================================================================================
void badDataTypeStrPos2(...){ puts("myInsert/myAppend - Bad data type at position 2");}
void badDataTypeStrArrPos2(...){ puts("myInsert/myAppend - Bad data type at position 2");}
void badDataTypePos1(...){ puts("myInsert/myAppend - Bad data type at position 1");}
//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define insertDefine(_1, _2, _3) _Generic((_1), \
                char*: _Generic((_2), \
                        char: insertChar, \
                        int: insertIntToStr, \
                        char*: insertStr , \
                        default: badDataTypeStrPos2), \
                char**: _Generic((_2), \
                        char*: insertStrToStrArr, \
                        char**: insertStrArrToStrArr, \
                        default: badDataTypeStrArrPos2), \
                default: badDataTypePos1 \
                )(_1, _2, _3);

#define myInsert(_1, _2, _3) ({ \
        void* returnArr; \
        do { \
                insertDefine(_1, _2, _3); \
                if (globalMyInsertType == 0){ returnArr = (char*) returnStrMyInsert;} \
                else if (globalMyInsertType == 1){ returnArr = (char**) returnStrArrMyInsert;}\
        } while(0); \
        returnArr; \

#define myAppend(_1, _2) insertDefine(_1, _2, getArrSize(_1)) \

*/
//=================================================================================================

//FUNCTIONS STRUCTURE------------------------------------------------------------------------------
// MY INSERT INPUT STRUCTURE
// <originalArr>, <arrToAdd>, <pos>

// MY APPEND INPUT STRUCTURE
// <originalArr>, <arrToAdd>
//-------------------------------------------------------------------------------------------------
