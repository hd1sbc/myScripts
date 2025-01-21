#include "symLink_31.sl"
#include "symLink_76.sl"
#include "symLink_33.sl"

// INPUT STRUCTURE---------------------------------------------------------------------------------
// <datatype> <convert to data type written as a string>
//-------------------------------------------------------------------------------------------------

// GLOBAL VARS-------------------------------------------------------------------------------------
char* globalReturnStrConvert = NULL;
char globalReturnCharConvert;
int globalReturnIntConvert;
double globalReturnDoubleConvert;
struct filesAndFoldersDbObj** globalReturnFilesAndFoldersDbObjConvert;
struct acceptedDbDataTypes** globalReturnacceptedDbDataTypesConvert = NULL;
struct envVars** globalReturnenvVarsConvert = NULL;
struct listProgramsToInstall** globalReturnlistProgramsToInstallConvert = NULL;
int globalTypeConvert = 0;
char acceptedDataTypesConvert[177] = "string/char*, int, char, double, filesAndFoldersDbObj**, acceptedDbDataTypes**, envVars**, listProgramsToInstall**";
static char** dataTypes = NULL;
static char** memberNames = NULL;

// Error Msg Constant Vars
char* F_CONVERT = (char*) __FILE__;

// Return Types (for reference only):
// 	1 :- string/char*
// 	2 :- int
// 	3 :- char
// 	4 :- double
//	5 :- filesAndFoldersDbObj**
//	6 :- acceptedDbDataTypes**
//	7 :- envVars**
//	8 :- listProgramsToInstall**
//-------------------------------------------------------------------------------------------------


//=================================================================================================
// PRINT STATEMENT - ERRORS
//=================================================================================================
void badArgPos1(...){ perror("===CONVERT===	Invalid data type at position 1. Accepted data types are: "); perror(acceptedDataTypesConvert);}
void badArgPos2(...){ perror("===CONVERT===	Invalid data type at position 2. Expected a char* or string that denotes the data type to convert parameter 1 to.");}
//=================================================================================================


//=================================================================================================
// CODE
//=================================================================================================


// This function converts a file to a string with the input as the file path
char* convertFileToString(char* filePathIn){
	// Vars
	FILE* file;
	char ch;
	int fileLen = 0;
	char* returnStr = (char*) malloc(sizeof(char));
	char* FF = (char*) __func__;

	file = fopen(filePathIn, "r"); //opens input file
        if (file == NULL){ myPerror(F_CONVERT, FF, "File not found"); fclose(file); return NULL;} // Data Validation

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
char* convertFromInt(int intIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;
	
	if (strcmp(dataTypeToConvertTo, "char*") == 0 || strcmp(dataTypeToConvertTo, "string") == 0 ||
			strcmp(dataTypeToConvertTo, "s") == 0){
		globalReturnStrConvert = malloc(sizeof(char)*100); //chose value of 100 arbitrarily to give enough room for int
		snprintf(globalReturnStrConvert, 100, "%d", intIn);
		globalTypeConvert = 1;
		return globalReturnStrConvert;
	}
	else {
		myPerror(F_CONVERT, FF, "Invalid data type to convert to. Can only convert int to string. Returning null");
		myPerror(F_CONVERT, FF, "	Valid 2nd parameter strings: char*, string, s");
		return NULL;
	}
}

// This function converts from a double to a string
char* convertFromDouble(double doubleIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;
	
	if (strcmp(dataTypeToConvertTo, "char*") == 0 || strcmp(dataTypeToConvertTo, "string") == 0 ||
			strcmp(dataTypeToConvertTo, "s") == 0){
		globalReturnStrConvert = malloc(sizeof(char)*100); //chose value of 100 arbitrarily to give enough room for int
		snprintf(globalReturnStrConvert, 100, "%f", doubleIn);
		globalTypeConvert = 4;
		return globalReturnStrConvert;
	}
	else {
		myPerror(F_CONVERT, FF, "Invalid data type to convert to. Can only convert double to string. Returning null");
		myPerror(F_CONVERT, FF, "	Valid 2nd parameter strings: char*, string, s");
		return NULL;
	}

}


// This function converts from a char to a string or int
void* convertFromChar(char charIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;

	if (strcmp(dataTypeToConvertTo, "char*") == 0 || strcmp(dataTypeToConvertTo, "string") == 0 ||
			strcmp(dataTypeToConvertTo, "s") == 0){
		globalReturnStrConvert = createStr(2);
		globalReturnStrConvert[0] = charIn;
		globalReturnStrConvert[1] = '\0';
		globalTypeConvert = 1;
		return globalReturnStrConvert;
	}
	else if (strcmp(dataTypeToConvertTo, "int") == 0 || strcmp(dataTypeToConvertTo, "d") == 0){
		globalReturnIntConvert = charIn - '0'; //I'm not doing any data validation to check if charIn is actually an int cuz I'm too lazy
		globalTypeConvert = 2;
		
		int* returnPtr = &globalReturnIntConvert; //converting to pointer cuz void* doesn't accept non-pointer data types
		return returnPtr;
	}
	else {
		myPerror(F_CONVERT, FF, "Invalid data type to convert to. Can only convert char to string and int. Returning null");
		myPerror(F_CONVERT, FF, "	Valid 2nd parameter strings: char*, string, s, int, d");
		return NULL;
	}
	
}

//This function converts from a string to a: 1) string (where the input is the file path), 2) int, 3) double, 4) char, 5) fileDbObj**
void* convertFromStr(char* strIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;
	
	// Convert file to string
	if (strcmp(dataTypeToConvertTo, "char*") == 0 || strcmp(dataTypeToConvertTo, "string") == 0 ||
			strcmp(dataTypeToConvertTo, "s") == 0){
		//if the string input contains a '/' or a '.', then I treat it as a file path or file name and will convert the file to a string
		if (regexMatch("\\/", strIn) == 1 || regexMatch("\\.", strIn) == 1){
			globalReturnStrConvert = convertFileToString(strIn);
			globalTypeConvert = 1;
			return globalReturnStrConvert;
		}
		else{
			myPerror(F_CONVERT, FF, "Invalid file name and/or path. No forward slashes or periods were detected in the string. Returning null");
			return NULL;
		}
	}
	
	// Convert string to int
	else if (strcmp(dataTypeToConvertTo, "int") == 0 || strcmp(dataTypeToConvertTo, "d") == 0){
		if (isInt(strIn) == 1){
			globalReturnIntConvert = atoi(strIn);
			globalTypeConvert = 2;
			
			int* returnPtr = &globalReturnIntConvert; //converting to pointer cuz void* doesn't accept non-pointer data types
			return returnPtr;

		}
		else {
			myPerror(F_CONVERT, FF, "Invalid string to convert; the value is not an integer. Returning null");
			return NULL;
		}
	}
	
	// Convert string to double. Using "float" and "f" cuz printf formatting doesn't have an option for double, only float
	else if (strcmp(dataTypeToConvertTo, "float") == 0 || strcmp(dataTypeToConvertTo, "f") == 0){
		if (isDouble(strIn) == 1){
			globalReturnDoubleConvert = atof(strIn);
			globalTypeConvert = 4;
			
			double* returnPtr = &globalReturnDoubleConvert; //converting to pointer cuz void* doesn't accept non-pointer data types
			return returnPtr;
		}
		else {
			myPerror(F_CONVERT, FF, "Invalid string to convert; the value is not a double. Returning null");
			return NULL;
		}
	}
	
	// Convert to char
	else if (strcmp(dataTypeToConvertTo, "char") == 0 || strcmp(dataTypeToConvertTo, "c") == 0){
		globalReturnCharConvert = strIn[0];
		globalTypeConvert = 3;

		char* returnPtr = &globalReturnCharConvert;
		return returnPtr; //converting to pointer cuz void* doesn't accept non-pointer data types
	}
	
	// Convert to filesAndFoldersDbObj
	else if (strcmp(dataTypeToConvertTo, "filesAndFoldersDbObj") == 0 || strcmp(dataTypeToConvertTo, "filesDb") == 0){
		globalReturnFilesAndFoldersDbObjConvert = csvToDbObjArr(strIn);
		globalTypeConvert = 5;
		return globalReturnFilesAndFoldersDbObjConvert;
	}

	else if (strcmp(dataTypeToConvertTo, "acceptedDbDataTypes") == 0){
		globalReturnacceptedDbDataTypesConvert = convertFromCsvToDbObjArr_1(strIn);
		globalTypeConvert = 6;
		return globalReturnacceptedDbDataTypesConvert;
	}

	else if (strcmp(dataTypeToConvertTo, "envVars") == 0){
		globalReturnenvVarsConvert = convertFromCsvToDbObjArr_2(strIn);
		globalTypeConvert = 7;
		return globalReturnenvVarsConvert;
	}

	else if (strcmp(dataTypeToConvertTo, "listProgramsToInstall") == 0){
		globalReturnlistProgramsToInstallConvert = convertFromCsvToDbObjArr_3(strIn);
		globalTypeConvert = 8;
		return globalReturnlistProgramsToInstallConvert;
	}

	// Invalid data type
	else{
		myPerror(F_CONVERT, FF, "	int, double, char, filesAndFoldersDbObj, and string (only when converting from a file where the input is the file path");
		myPerror(F_CONVERT, FF, "	Valid 2nd parameter strings: char*, string, s, int, d, float, f, char, c, filesAndFolderDbObj, filesDb");
		return NULL;
	}
}

// This function converts from filesAndFoldersDbObj to a csv-formatted string
char* convertFromFilesAndFoldersDbObj(struct filesAndFoldersDbObj** dbObjArrIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;

	if (strcmp(dataTypeToConvertTo, "char*") == 0 || strcmp(dataTypeToConvertTo, "string") == 0 ||
			strcmp(dataTypeToConvertTo, "s") == 0){
		globalReturnStrConvert = dbObjArrToCsv(dbObjArrIn);
		globalTypeConvert = 1;
		return globalReturnStrConvert;
	}
	else{
		myPerror(F_CONVERT, FF, "Invalid data type to convert to. Can only convert to string. Returning null");
		myPerror(F_CONVERT, FF, "	Valid 2nd parameter strings: char*, string, s");
		return NULL;
	}
}

// BEGIN acceptedDbDataTypes
struct acceptedDbDataTypes** convertFromCsvToDbObjArr_1(char* csvAsStrIn){
	// Vars
	char* FF = (char*) __func__;
	int numRows;
	struct acceptedDbDataTypes** db_1;

	// Data Validation
	if (csvAsStrIn == NULL){ myPerror(F_CONVERT, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (strlen(csvAsStrIn) == 0){ myPerror(F_CONVERT, FF, "Invalid 1st parameter; value is an empty string. Returning null"); return NULL;}

	// Getting Values
	numRows = getNumRows(csvAsStrIn);
	if (numRows < 0){ myPerror(F_CONVERT, FF, "getNumRows() function returned an error value (<0). Returning null"); return NULL;}

	db_1 = malloc(sizeof(struct acceptedDbDataTypes*)*(numRows+1));
	db_1[0] = createStruct_1();
	if (db_1[0] == NULL){
		myPerror(F_CONVERT, FF, "createStruct_1() function returned a null value. Returning null");
		free(db_1);
		return NULL;
	}

	// Iterating over every row and adding each row to struct array
	for (int i = 1; i < numRows; i++){
		char* currRow = getRow(i, csvAsStrIn);
		if (i+1 == numRows){ db_1 = addElementToDbArr_1(db_1, currRow, 10);}
		else{ db_1 = addElementToDbArr_1(db_1, currRow, 0);}
		if (db_1 == NULL){
			myPerror(F_CONVERT, FF, "addElementToDbArr_1() function returned a null value. Returning null");
			free(currRow);
			return NULL;
		}
		free(currRow);
	}

	return db_1;
}
char* convertFromDbObj_1ToStr(struct acceptedDbDataTypes* dbIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;
	int currColNum = 0;
	char* returnStr;

	// Data Validation-------------------------------------------------------------------------
	if (dbIn == NULL){ myPerror(F_CONVERT, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (dataTypeToConvertTo == NULL){ myPerror(F_CONVERT, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}
	if (strcmp(dataTypeToConvertTo, "string") != 0 && strcmp(dataTypeToConvertTo, "char*") != 0 && strcmp(dataTypeToConvertTo, "s") != 0){
		myPerror(F_CONVERT, FF, "Invalid data type to convert to. Can only convert to string. Returning null");
		myPerror(F_CONVERT, FF, "	Valid 2nd parameter strings: char*, string, s");
		return NULL;
	}
	//-----------------------------------------------------------------------------------------

	// Getting Values--------------------------------------------------------------------------
	returnStr = createStr(1);

	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/acceptedDbDataTypes.csv", 4);}
	if (dataTypes == NULL){ myPerror(F_CONVERT, FF, "getTableInfo() function returned a null value. Returning null"); return NULL;}
	//-----------------------------------------------------------------------------------------

	char* idAsStr;
	if (strcmp(dataTypes[currColNum], "string") != 0){ //if not a string data type, convert to string
		idAsStr = (char*) convert(dbIn->id, "string");
		if (idAsStr == NULL){
			myPerror(F_CONVERT, FF, "convert() function for id returned a null value. Returning null");
			free(returnStr); return NULL;
		}
	}
	else{
		//using pragma to prevent conversion warnings. The warning can be ignored cuz conditional code above prevents data type mismatch
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wint-conversion"
		idAsStr = createStr(strlen(dbIn->id)); strcat(idAsStr, dbIn->id);
		#pragma GCC diagnostic pop
	}
	returnStr = myRealloc((int) strlen(idAsStr)+2, returnStr);
	if (returnStr == NULL){
		myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null");
		free(returnStr); free(idAsStr); return NULL;
	}
	if (strlen(idAsStr) > 0){ strcat(returnStr, idAsStr);}
	strcat(returnStr, ",");
	free(idAsStr);
	currColNum++;

	char* valueAsStr;
	if (strcmp(dataTypes[currColNum], "string") != 0){ //if not a string data type, convert to string
		valueAsStr = (char*) convert(dbIn->value, "string");
		if (valueAsStr == NULL){
			myPerror(F_CONVERT, FF, "convert() function for value returned a null value. Returning null");
			free(returnStr); return NULL;
		}
	}
	else{
		//using pragma to prevent conversion warnings. The warning can be ignored cuz conditional code above prevents data type mismatch
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wint-conversion"
		valueAsStr = createStr(strlen(dbIn->value)); strcat(valueAsStr, dbIn->value);
		#pragma GCC diagnostic pop
	}
	returnStr = myRealloc((int) strlen(valueAsStr)+2, returnStr);
	if (returnStr == NULL){
		myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null");
		free(returnStr); free(valueAsStr); return NULL;
	}
	if (strlen(valueAsStr) > 0){ strcat(returnStr, valueAsStr);}
	strcat(returnStr, ",");
	free(valueAsStr);
	currColNum++;

	returnStr[strlen(returnStr)-1] = '\0'; //replacing trailing comma with end-of-string char
	return returnStr;
}


char* makeHeaderRow_1(){
	// Vars
	char* FF = (char*) __func__;
	char* headerRow;

	// Getting Values
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/acceptedDbDataTypes.csv", 3);}
	if (memberNames == NULL){ myPerror(F_CONVERT, FF, "getTableInfo() function returned a null value. Returning null"); return NULL;}
	headerRow = createStr(1);

	// Converting the char array with member names into a string where each element is separated by commas
	for (int i = 0; i < getArrSize(memberNames); i++){
		headerRow = myRealloc((int) strlen(memberNames[i])+2, headerRow);
		strcat(headerRow, memberNames[i]);
		strcat(headerRow, ",");
	}

	headerRow[strlen(headerRow)-1] = '\0'; //replacing trailing comma with end-of-string char
	return headerRow;
}


char* convertFromDbObjArr_1ToCsv(struct acceptedDbDataTypes** dbIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;
	char* returnStr;
	char* headerRow;

	// Data Validation
	if (dbIn == NULL){ myPerror(F_CONVERT, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (dataTypeToConvertTo == NULL){ myPerror(F_CONVERT, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}
	if (strcmp(dataTypeToConvertTo, "string") != 0 && strcmp(dataTypeToConvertTo, "char*") != 0 && strcmp(dataTypeToConvertTo, "s") != 0){
		myPerror(F_CONVERT, FF, "Invalid data type to convert to. Can only convert to string. Returning null");
		myPerror(F_CONVERT, FF, "	Valid 2nd parameter strings: char*, string, s");
		return NULL;
	}
	//-----------------------------------------------------------------------------------------

	// Getting Values
	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/acceptedDbDataTypes.csv", 4);}
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/acceptedDbDataTypes.csv", 3);}
	headerRow = makeHeaderRow_1();
	returnStr = createStr(1);

	returnStr = myRealloc((int) strlen(headerRow)+2, returnStr);
	if (returnStr == NULL){ myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null"); free(headerRow); return NULL;}
	strcat(returnStr, headerRow);
	strcat(returnStr, "\n");
	free(headerRow);

	// Converting each struct ptr in dbIn to a string and adding it to returnStr
	for (int i = 0; i < getArrSize(dbIn); i++){
		char* structAsStr = convertFromDbObj_1ToStr(dbIn[i], dataTypeToConvertTo);
		if (structAsStr == NULL){ myPerror(F_CONVERT, FF, "convertFromDbObj_1() function returned a null value. Returning null"); free(returnStr); return NULL;}

		returnStr = myRealloc((int) strlen(structAsStr)+2, returnStr);
		if (returnStr == NULL){ myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null"); free(structAsStr); return NULL;}

		strcat(returnStr, structAsStr);
		strcat(returnStr, "\n");
		free(structAsStr);
	}

	if (returnStr[strlen(returnStr)] == '\n'){ returnStr[strlen(returnStr)] = '\0';}

	globalReturnStrConvert = returnStr;
	globalTypeConvert = 1;

	myFree(dataTypes); myFree(memberNames);
	dataTypes = NULL; memberNames = NULL;

	return returnStr;
}
// END acceptedDbDataTypes

// BEGIN envVars
struct envVars** convertFromCsvToDbObjArr_2(char* csvAsStrIn){
	// Vars
	char* FF = (char*) __func__;
	int numRows;
	struct envVars** db_2;

	// Data Validation
	if (csvAsStrIn == NULL){ myPerror(F_CONVERT, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (strlen(csvAsStrIn) == 0){ myPerror(F_CONVERT, FF, "Invalid 1st parameter; value is an empty string. Returning null"); return NULL;}

	// Getting Values
	numRows = getNumRows(csvAsStrIn);
	if (numRows < 0){ myPerror(F_CONVERT, FF, "getNumRows() function returned an error value (<0). Returning null"); return NULL;}

	db_2 = malloc(sizeof(struct envVars*)*(numRows+1));
	db_2[0] = createStruct_2();
	if (db_2[0] == NULL){
		myPerror(F_CONVERT, FF, "createStruct_2() function returned a null value. Returning null");
		free(db_2);
		return NULL;
	}

	// Iterating over every row and adding each row to struct array
	for (int i = 1; i < numRows; i++){
		char* currRow = getRow(i, csvAsStrIn);
		if (i+1 == numRows){ db_2 = addElementToDbArr_2(db_2, currRow, 10);}
		else{ db_2 = addElementToDbArr_2(db_2, currRow, 0);}
		if (db_2 == NULL){
			myPerror(F_CONVERT, FF, "addElementToDbArr_2() function returned a null value. Returning null");
			free(currRow);
			return NULL;
		}
		free(currRow);
	}

	return db_2;
}
char* convertFromDbObj_2ToStr(struct envVars* dbIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;
	int currColNum = 0;
	char* returnStr;

	// Data Validation-------------------------------------------------------------------------
	if (dbIn == NULL){ myPerror(F_CONVERT, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (dataTypeToConvertTo == NULL){ myPerror(F_CONVERT, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}
	if (strcmp(dataTypeToConvertTo, "string") != 0 && strcmp(dataTypeToConvertTo, "char*") != 0 && strcmp(dataTypeToConvertTo, "s") != 0){
		myPerror(F_CONVERT, FF, "Invalid data type to convert to. Can only convert to string. Returning null");
		myPerror(F_CONVERT, FF, "	Valid 2nd parameter strings: char*, string, s");
		return NULL;
	}
	//-----------------------------------------------------------------------------------------

	// Getting Values--------------------------------------------------------------------------
	returnStr = createStr(1);

	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/envVars.csv", 4);}
	if (dataTypes == NULL){ myPerror(F_CONVERT, FF, "getTableInfo() function returned a null value. Returning null"); return NULL;}
	//-----------------------------------------------------------------------------------------

	char* idAsStr;
	if (strcmp(dataTypes[currColNum], "string") != 0){ //if not a string data type, convert to string
		idAsStr = (char*) convert(dbIn->id, "string");
		if (idAsStr == NULL){
			myPerror(F_CONVERT, FF, "convert() function for id returned a null value. Returning null");
			free(returnStr); return NULL;
		}
	}
	else{
		//using pragma to prevent conversion warnings. The warning can be ignored cuz conditional code above prevents data type mismatch
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wint-conversion"
		idAsStr = createStr(strlen(dbIn->id)); strcat(idAsStr, dbIn->id);
		#pragma GCC diagnostic pop
	}
	returnStr = myRealloc((int) strlen(idAsStr)+2, returnStr);
	if (returnStr == NULL){
		myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null");
		free(returnStr); free(idAsStr); return NULL;
	}
	if (strlen(idAsStr) > 0){ strcat(returnStr, idAsStr);}
	strcat(returnStr, ",");
	free(idAsStr);
	currColNum++;

	char* fieldNameAsStr;
	if (strcmp(dataTypes[currColNum], "string") != 0){ //if not a string data type, convert to string
		fieldNameAsStr = (char*) convert(dbIn->fieldName, "string");
		if (fieldNameAsStr == NULL){
			myPerror(F_CONVERT, FF, "convert() function for fieldName returned a null value. Returning null");
			free(returnStr); return NULL;
		}
	}
	else{
		//using pragma to prevent conversion warnings. The warning can be ignored cuz conditional code above prevents data type mismatch
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wint-conversion"
		fieldNameAsStr = createStr(strlen(dbIn->fieldName)); strcat(fieldNameAsStr, dbIn->fieldName);
		#pragma GCC diagnostic pop
	}
	returnStr = myRealloc((int) strlen(fieldNameAsStr)+2, returnStr);
	if (returnStr == NULL){
		myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null");
		free(returnStr); free(fieldNameAsStr); return NULL;
	}
	if (strlen(fieldNameAsStr) > 0){ strcat(returnStr, fieldNameAsStr);}
	strcat(returnStr, ",");
	free(fieldNameAsStr);
	currColNum++;

	char* fieldValAsStr;
	if (strcmp(dataTypes[currColNum], "string") != 0){ //if not a string data type, convert to string
		fieldValAsStr = (char*) convert(dbIn->fieldVal, "string");
		if (fieldValAsStr == NULL){
			myPerror(F_CONVERT, FF, "convert() function for fieldVal returned a null value. Returning null");
			free(returnStr); return NULL;
		}
	}
	else{
		//using pragma to prevent conversion warnings. The warning can be ignored cuz conditional code above prevents data type mismatch
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wint-conversion"
		fieldValAsStr = createStr(strlen(dbIn->fieldVal)); strcat(fieldValAsStr, dbIn->fieldVal);
		#pragma GCC diagnostic pop
	}
	returnStr = myRealloc((int) strlen(fieldValAsStr)+2, returnStr);
	if (returnStr == NULL){
		myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null");
		free(returnStr); free(fieldValAsStr); return NULL;
	}
	if (strlen(fieldValAsStr) > 0){ strcat(returnStr, fieldValAsStr);}
	strcat(returnStr, ",");
	free(fieldValAsStr);
	currColNum++;

	returnStr[strlen(returnStr)-1] = '\0'; //replacing trailing comma with end-of-string char
	return returnStr;
}


char* makeHeaderRow_2(){
	// Vars
	char* FF = (char*) __func__;
	char* headerRow;

	// Getting Values
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/envVars.csv", 3);}
	if (memberNames == NULL){ myPerror(F_CONVERT, FF, "getTableInfo() function returned a null value. Returning null"); return NULL;}
	headerRow = createStr(1);

	// Converting the char array with member names into a string where each element is separated by commas
	for (int i = 0; i < getArrSize(memberNames); i++){
		headerRow = myRealloc((int) strlen(memberNames[i])+2, headerRow);
		strcat(headerRow, memberNames[i]);
		strcat(headerRow, ",");
	}

	headerRow[strlen(headerRow)-1] = '\0'; //replacing trailing comma with end-of-string char
	return headerRow;
}


char* convertFromDbObjArr_2ToCsv(struct envVars** dbIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;
	char* returnStr;
	char* headerRow;

	// Data Validation
	if (dbIn == NULL){ myPerror(F_CONVERT, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (dataTypeToConvertTo == NULL){ myPerror(F_CONVERT, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}
	if (strcmp(dataTypeToConvertTo, "string") != 0 && strcmp(dataTypeToConvertTo, "char*") != 0 && strcmp(dataTypeToConvertTo, "s") != 0){
		myPerror(F_CONVERT, FF, "Invalid data type to convert to. Can only convert to string. Returning null");
		myPerror(F_CONVERT, FF, "	Valid 2nd parameter strings: char*, string, s");
		return NULL;
	}
	//-----------------------------------------------------------------------------------------

	// Getting Values
	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/envVars.csv", 4);}
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/envVars.csv", 3);}
	headerRow = makeHeaderRow_2();
	returnStr = createStr(1);

	returnStr = myRealloc((int) strlen(headerRow)+2, returnStr);
	if (returnStr == NULL){ myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null"); free(headerRow); return NULL;}
	strcat(returnStr, headerRow);
	strcat(returnStr, "\n");
	free(headerRow);

	// Converting each struct ptr in dbIn to a string and adding it to returnStr
	for (int i = 0; i < getArrSize(dbIn); i++){
		char* structAsStr = convertFromDbObj_2ToStr(dbIn[i], dataTypeToConvertTo);
		if (structAsStr == NULL){ myPerror(F_CONVERT, FF, "convertFromDbObj_2() function returned a null value. Returning null"); free(returnStr); return NULL;}

		returnStr = myRealloc((int) strlen(structAsStr)+2, returnStr);
		if (returnStr == NULL){ myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null"); free(structAsStr); return NULL;}

		strcat(returnStr, structAsStr);
		strcat(returnStr, "\n");
		free(structAsStr);
	}

	if (returnStr[strlen(returnStr)] == '\n'){ returnStr[strlen(returnStr)] = '\0';}

	globalReturnStrConvert = returnStr;
	globalTypeConvert = 1;

	myFree(dataTypes); myFree(memberNames);
	dataTypes = NULL; memberNames = NULL;

	return returnStr;
}
// END envVars

// BEGIN listProgramsToInstall
struct listProgramsToInstall** convertFromCsvToDbObjArr_3(char* csvAsStrIn){
	// Vars
	char* FF = (char*) __func__;
	int numRows;
	struct listProgramsToInstall** db_3;

	// Data Validation
	if (csvAsStrIn == NULL){ myPerror(F_CONVERT, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (strlen(csvAsStrIn) == 0){ myPerror(F_CONVERT, FF, "Invalid 1st parameter; value is an empty string. Returning null"); return NULL;}

	// Getting Values
	numRows = getNumRows(csvAsStrIn);
	if (numRows < 0){ myPerror(F_CONVERT, FF, "getNumRows() function returned an error value (<0). Returning null"); return NULL;}

	db_3 = malloc(sizeof(struct listProgramsToInstall*)*(numRows+1));
	db_3[0] = createStruct_3();
	if (db_3[0] == NULL){
		myPerror(F_CONVERT, FF, "createStruct_3() function returned a null value. Returning null");
		free(db_3);
		return NULL;
	}

	// Iterating over every row and adding each row to struct array
	for (int i = 1; i < numRows; i++){
		char* currRow = getRow(i, csvAsStrIn);
		if (i+1 == numRows){ db_3 = addElementToDbArr_3(db_3, currRow, 10);}
		else{ db_3 = addElementToDbArr_3(db_3, currRow, 0);}
		if (db_3 == NULL){
			myPerror(F_CONVERT, FF, "addElementToDbArr_3() function returned a null value. Returning null");
			free(currRow);
			return NULL;
		}
		free(currRow);
	}

	return db_3;
}
char* convertFromDbObj_3ToStr(struct listProgramsToInstall* dbIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;
	int currColNum = 0;
	char* returnStr;

	// Data Validation-------------------------------------------------------------------------
	if (dbIn == NULL){ myPerror(F_CONVERT, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (dataTypeToConvertTo == NULL){ myPerror(F_CONVERT, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}
	if (strcmp(dataTypeToConvertTo, "string") != 0 && strcmp(dataTypeToConvertTo, "char*") != 0 && strcmp(dataTypeToConvertTo, "s") != 0){
		myPerror(F_CONVERT, FF, "Invalid data type to convert to. Can only convert to string. Returning null");
		myPerror(F_CONVERT, FF, "	Valid 2nd parameter strings: char*, string, s");
		return NULL;
	}
	//-----------------------------------------------------------------------------------------

	// Getting Values--------------------------------------------------------------------------
	returnStr = createStr(1);

	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/listProgramsToInstall.csv", 4);}
	if (dataTypes == NULL){ myPerror(F_CONVERT, FF, "getTableInfo() function returned a null value. Returning null"); return NULL;}
	//-----------------------------------------------------------------------------------------

	char* idAsStr;
	if (strcmp(dataTypes[currColNum], "string") != 0){ //if not a string data type, convert to string
		idAsStr = (char*) convert(dbIn->id, "string");
		if (idAsStr == NULL){
			myPerror(F_CONVERT, FF, "convert() function for id returned a null value. Returning null");
			free(returnStr); return NULL;
		}
	}
	else{
		//using pragma to prevent conversion warnings. The warning can be ignored cuz conditional code above prevents data type mismatch
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wint-conversion"
		idAsStr = createStr(strlen(dbIn->id)); strcat(idAsStr, dbIn->id);
		#pragma GCC diagnostic pop
	}
	returnStr = myRealloc((int) strlen(idAsStr)+2, returnStr);
	if (returnStr == NULL){
		myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null");
		free(returnStr); free(idAsStr); return NULL;
	}
	if (strlen(idAsStr) > 0){ strcat(returnStr, idAsStr);}
	strcat(returnStr, ",");
	free(idAsStr);
	currColNum++;

	char* packageNameAsStr;
	if (strcmp(dataTypes[currColNum], "string") != 0){ //if not a string data type, convert to string
		packageNameAsStr = (char*) convert(dbIn->packageName, "string");
		if (packageNameAsStr == NULL){
			myPerror(F_CONVERT, FF, "convert() function for packageName returned a null value. Returning null");
			free(returnStr); return NULL;
		}
	}
	else{
		//using pragma to prevent conversion warnings. The warning can be ignored cuz conditional code above prevents data type mismatch
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wint-conversion"
		packageNameAsStr = createStr(strlen(dbIn->packageName)); strcat(packageNameAsStr, dbIn->packageName);
		#pragma GCC diagnostic pop
	}
	returnStr = myRealloc((int) strlen(packageNameAsStr)+2, returnStr);
	if (returnStr == NULL){
		myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null");
		free(returnStr); free(packageNameAsStr); return NULL;
	}
	if (strlen(packageNameAsStr) > 0){ strcat(returnStr, packageNameAsStr);}
	strcat(returnStr, ",");
	free(packageNameAsStr);
	currColNum++;

	char* systemsToInstallOnAsStr;
	if (strcmp(dataTypes[currColNum], "string") != 0){ //if not a string data type, convert to string
		systemsToInstallOnAsStr = (char*) convert(dbIn->systemsToInstallOn, "string");
		if (systemsToInstallOnAsStr == NULL){
			myPerror(F_CONVERT, FF, "convert() function for systemsToInstallOn returned a null value. Returning null");
			free(returnStr); return NULL;
		}
	}
	else{
		//using pragma to prevent conversion warnings. The warning can be ignored cuz conditional code above prevents data type mismatch
		#pragma GCC diagnostic push
		#pragma GCC diagnostic ignored "-Wint-conversion"
		systemsToInstallOnAsStr = createStr(strlen(dbIn->systemsToInstallOn)); strcat(systemsToInstallOnAsStr, dbIn->systemsToInstallOn);
		#pragma GCC diagnostic pop
	}
	returnStr = myRealloc((int) strlen(systemsToInstallOnAsStr)+2, returnStr);
	if (returnStr == NULL){
		myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null");
		free(returnStr); free(systemsToInstallOnAsStr); return NULL;
	}
	if (strlen(systemsToInstallOnAsStr) > 0){ strcat(returnStr, systemsToInstallOnAsStr);}
	strcat(returnStr, ",");
	free(systemsToInstallOnAsStr);
	currColNum++;

	returnStr[strlen(returnStr)-1] = '\0'; //replacing trailing comma with end-of-string char
	return returnStr;
}


char* makeHeaderRow_3(){
	// Vars
	char* FF = (char*) __func__;
	char* headerRow;

	// Getting Values
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/listProgramsToInstall.csv", 3);}
	if (memberNames == NULL){ myPerror(F_CONVERT, FF, "getTableInfo() function returned a null value. Returning null"); return NULL;}
	headerRow = createStr(1);

	// Converting the char array with member names into a string where each element is separated by commas
	for (int i = 0; i < getArrSize(memberNames); i++){
		headerRow = myRealloc((int) strlen(memberNames[i])+2, headerRow);
		strcat(headerRow, memberNames[i]);
		strcat(headerRow, ",");
	}

	headerRow[strlen(headerRow)-1] = '\0'; //replacing trailing comma with end-of-string char
	return headerRow;
}


char* convertFromDbObjArr_3ToCsv(struct listProgramsToInstall** dbIn, char* dataTypeToConvertTo){
	// Vars
	char* FF = (char*) __func__;
	char* returnStr;
	char* headerRow;

	// Data Validation
	if (dbIn == NULL){ myPerror(F_CONVERT, FF, "Invalid 1st parameter; value is null. Returning null"); return NULL;}
	if (dataTypeToConvertTo == NULL){ myPerror(F_CONVERT, FF, "Invalid 2nd parameter; value is null. Returning null"); return NULL;}
	if (strcmp(dataTypeToConvertTo, "string") != 0 && strcmp(dataTypeToConvertTo, "char*") != 0 && strcmp(dataTypeToConvertTo, "s") != 0){
		myPerror(F_CONVERT, FF, "Invalid data type to convert to. Can only convert to string. Returning null");
		myPerror(F_CONVERT, FF, "	Valid 2nd parameter strings: char*, string, s");
		return NULL;
	}
	//-----------------------------------------------------------------------------------------

	// Getting Values
	if (dataTypes == NULL){ dataTypes = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/listProgramsToInstall.csv", 4);}
	if (memberNames == NULL){ memberNames = getTableInfo("MYSCRIPTSVAR/dep/fileAndFolderInfo/listProgramsToInstall.csv", 3);}
	headerRow = makeHeaderRow_3();
	returnStr = createStr(1);

	returnStr = myRealloc((int) strlen(headerRow)+2, returnStr);
	if (returnStr == NULL){ myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null"); free(headerRow); return NULL;}
	strcat(returnStr, headerRow);
	strcat(returnStr, "\n");
	free(headerRow);

	// Converting each struct ptr in dbIn to a string and adding it to returnStr
	for (int i = 0; i < getArrSize(dbIn); i++){
		char* structAsStr = convertFromDbObj_3ToStr(dbIn[i], dataTypeToConvertTo);
		if (structAsStr == NULL){ myPerror(F_CONVERT, FF, "convertFromDbObj_3() function returned a null value. Returning null"); free(returnStr); return NULL;}

		returnStr = myRealloc((int) strlen(structAsStr)+2, returnStr);
		if (returnStr == NULL){ myPerror(F_CONVERT, FF, "myRealloc() function returned a null value. Returning null"); free(structAsStr); return NULL;}

		strcat(returnStr, structAsStr);
		strcat(returnStr, "\n");
		free(structAsStr);
	}

	if (returnStr[strlen(returnStr)] == '\n'){ returnStr[strlen(returnStr)] = '\0';}

	globalReturnStrConvert = returnStr;
	globalTypeConvert = 1;

	myFree(dataTypes); myFree(memberNames);
	dataTypes = NULL; memberNames = NULL;

	return returnStr;
}
// END listProgramsToInstall

//=================================================================================================


//=================================================================================================
// BACKUP OF DEFINES
//=================================================================================================
/*
#define convertHelper(_1, _2) \
                _Generic((_1), \
                int: _Generic((_2), \
                        char*: convertFromInt, \
                        default: badArgPos2), \
                double: _Generic((_2), \
                        char*: convertFromDouble, \
                        default: badArgPos2), \
                char: _Generic((_2), \
                        char*: convertFromChar, \
                        default: badArgPos2), \
                char*: _Generic((_2), \
                        char*: convertFromStr, \
                        default: badArgPos2), \
                struct filesAndFoldersDbObj**: _Generic((_2) , \
                        char*: convertFromFilesAndFoldersDbObj, \
                        default: badArgPos2), \
		struct acceptedDbDataTypes**: convertFromDbObjArr_1ToCsv, \
		struct envVars**: convertFromDbObjArr_2ToCsv, \
		struct listProgramsToInstall**: convertFromDbObjArr_3ToCsv, \
                default: badArgPos1 \
                )(_1,_2); \


#define convert(_1, _2) ({ \
        void* returnData; \
        do { \
                convertHelper(_1, _2); \
                if (globalTypeConvert == 0){ returnData = NULL;} \
                else if (globalTypeConvert == 1){ returnData = (char*) globalReturnStrConvert;} \
                else if (globalTypeConvert == 2){ \
                        int* returnPtr = &globalReturnIntConvert; \
                        returnData = (int*) returnPtr; \
                } \
		else if (globalTypeConvert == 4){ \
                        double* returnPtr = &globalReturnDoubleConvert; \
                        returnData = (double*) returnPtr; \
                } \
                else if (globalTypeConvert == 5){ returnData = (struct filesAndFoldersDbObj**) globalReturnFilesAndFoldersDbObjConvert;} \
		else if (globalTypeConvert == 6){ returnData = (struct acceptedDbDataTypes**) globalReturnacceptedDbDataTypesConvert;} \
		else if (globalTypeConvert == 7){ returnData = (struct envVars**) globalReturnenvVarsConvert;} \
		else if (globalTypeConvert == 8){ returnData = (struct listProgramsToInstall**) globalReturnlistProgramsToInstallConvert;} \
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
//	5 :- filesAndFoldersDbObj**
//	6 :- acceptedDbDataTypes**
//	7 :- envVars**
//	8 :- listProgramsToInstall**
