#!/bin/bash

# This function generates c code to convert from csv to database object array
#Args: none
genCodeConvertFromCsv(){
	echo "struct $structName** convertFromCsvToDbObjArr_$dbObjNum(char* csvAsStrIn){"
	echo "	// Vars"
	echo "	char* FF = (char*) __func__;"
	echo "	int numRows;"
	echo "	struct $structName** db_$dbObjNum;"
	echo ""

	echo "	// Data Validation"
	echo "	if (csvAsStrIn == NULL){ myPerror($F_convert, FF, \"Invalid 1st parameter; value is null. Returning null\"); return NULL;}"
	echo "	if (strlen(csvAsStrIn) == 0){ myPerror($F_convert, FF, \"Invalid 1st parameter; value is an empty string. Returning null\"); return NULL;}"
	echo ""

	echo "	// Getting Values"
       	echo "	numRows = getNumRows(csvAsStrIn);"
	echo "	if (numRows < 0){ myPerror($F_convert, FF, \"getNumRows() function returned an error value (<0). Returning null\"); return NULL;}"
	echo ""
	echo "	db_$dbObjNum = malloc(sizeof(struct $structName*)*(numRows+1));"
	echo "	db_$dbObjNum[0] = createStruct_$dbObjNum();"
	echo "	if (db_$dbObjNum[0] == NULL){"
	echo "		myPerror($F_convert, FF, \"createStruct_$dbObjNum() function returned a null value. Returning null\");"
	echo "		free(db_$dbObjNum);"
	echo "		return NULL;"
	echo "	}"
	echo""

	echo "	// Iterating over every row and adding each row to struct array"
	echo "	for (int i = 1; i < numRows; i++){"
	echo "		char* currRow = getRow(i, csvAsStrIn);"
	echo "		if (i+1 == numRows){ db_$dbObjNum = addElementToDbArr_$dbObjNum(db_$dbObjNum, currRow, 10);}"
	echo "		else{ db_$dbObjNum = addElementToDbArr_$dbObjNum(db_$dbObjNum, currRow, 0);}"
	echo "		if (db_$dbObjNum == NULL){"
	echo "			myPerror($F_convert, FF, \"addElementToDbArr_$dbObjNum() function returned a null value. Returning null\");"
	echo "			free(currRow);"
	echo "			return NULL;"
	echo "		}"
	echo "		free(currRow);"
	echo "	}"
	echo ""

	echo "	return db_$dbObjNum;"
	echo "}"

	return 0
}


# This function generates c code to convert from database object array to string (in csv format, including header row)
#Args: none
genCodeConvertToCsv(){
	# Converting from single struct (ptr) to string (csv-format)-------------------------------
	echo "char* convertFromDbObj_${dbObjNum}ToStr(struct $structName* dbIn, char* dataTypeToConvertTo){"
	echo "	// Vars"
	echo "	char* FF = (char*) __func__;"
	echo "	int currColNum = 0;"
	echo "	char* returnStr;"
	echo ""

	echo "	// Data Validation-------------------------------------------------------------------------"
	echo "	if (dbIn == NULL){ myPerror($F_convert, FF, \"Invalid 1st parameter; value is null. Returning null\"); return NULL;}"
	echo "	if (dataTypeToConvertTo == NULL){ myPerror($F_convert, FF, \"Invalid 2nd parameter; value is null. Returning null\"); return NULL;}"
	echo "	if (strcmp(dataTypeToConvertTo, \"string\") != 0 && strcmp(dataTypeToConvertTo, \"char*\") != 0 && strcmp(dataTypeToConvertTo, \"s\") != 0){"
	echo "		myPerror($F_convert, FF, \"Invalid data type to convert to. Can only convert to string. Returning null\");"
	echo "		myPerror($F_convert, FF, \"	Valid 2nd parameter strings: char*, string, s\");"
	echo "		return NULL;"
	echo "	}"
	echo "	//-----------------------------------------------------------------------------------------"
	echo ""

	echo "	// Getting Values--------------------------------------------------------------------------"
	echo "	returnStr = createStr(1);"
	echo ""
	echo "	if (dataTypes == NULL){ dataTypes = getTableInfo(\"$csvLoc\", $dataTypesColTypeListColNum);}"
	echo "	if (dataTypes == NULL){ myPerror($F_convert, FF, \"getTableInfo() function returned a null value. Returning null\"); return NULL;}"
	echo "	//-----------------------------------------------------------------------------------------"
	echo ""

	# Iterating over every column name and adding code to convert it to string based on it's data type
	origIfs=$IFS
	IFS=' '
	for varName in $varNames; do
		echo "	char* ${varName}AsStr;"
		echo "	if (strcmp(dataTypes[currColNum], \"string\") != 0){ //if not a string data type, convert to string"
		echo "		${varName}AsStr = (char*) convert(dbIn->$varName, \"string\");"
		echo "		if (${varName}AsStr == NULL){"
		echo "			myPerror($F_convert, FF, \"convert() function for ${varName} returned a null value. Returning null\");"
		echo "			free(returnStr); return NULL;"
		echo "		}"
		echo "	}"

		echo "	else{"
		echo "		//using pragma to prevent conversion warnings. The warning can be ignored cuz conditional code above prevents data type mismatch"
		echo "		#pragma GCC diagnostic push"
		echo "		#pragma GCC diagnostic ignored \"-Wint-conversion\""
		echo "		${varName}AsStr = createStr(strlen(dbIn->$varName)); strcat(${varName}AsStr, dbIn->$varName);"
		echo "		#pragma GCC diagnostic pop"
		echo "	}"
		echo "	returnStr = myRealloc((int) strlen(${varName}AsStr)+2, returnStr);"
		echo "	if (returnStr == NULL){"
		echo "		myPerror($F_convert, FF, \"myRealloc() function returned a null value. Returning null\");"
		echo "		free(returnStr); free(${varName}AsStr); return NULL;"
		echo "	}"
		echo "	if (strlen(${varName}AsStr) > 0){ strcat(returnStr, ${varName}AsStr);}"
		echo "	strcat(returnStr, \",\");"
		echo "	free(${varName}AsStr);"
		echo "	currColNum++;"
		echo ""
	done
	IFS=$origIfs

	echo "	returnStr[strlen(returnStr)-1] = '\\\\0'; //replacing trailing comma with end-of-string char"
	echo "	return returnStr;"
	echo "}"
	#------------------------------------------------------------------------------------------

	echo ""
	echo ""
	
	# Making header row------------------------------------------------------------------------
	echo "char* makeHeaderRow_$dbObjNum(){"
	echo "	// Vars"
	echo "	char* FF = (char*) __func__;"
	echo "	char* headerRow;"
	echo ""
	
	echo "	// Getting Values"
	echo "	if (memberNames == NULL){ memberNames = getTableInfo(\"$csvLoc\", $colNameColTypeListColNum);}"
	echo "	if (memberNames == NULL){ myPerror($F_convert, FF, \"getTableInfo() function returned a null value. Returning null\"); return NULL;}"
	echo "	headerRow = createStr(1);"
	echo ""
	
	echo "	// Converting the char array with member names into a string where each element is separated by commas"
	echo "	for (int i = 0; i < getArrSize(memberNames); i++){"
	echo "		headerRow = myRealloc((int) strlen(memberNames[i])+2, headerRow);"
	echo "		strcat(headerRow, memberNames[i]);"
	echo "		strcat(headerRow, \",\");"
	echo "	}"
	echo ""
	echo "	headerRow[strlen(headerRow)-1] = '\\\\0'; //replacing trailing comma with end-of-string char"
	echo "	return headerRow;"
	echo "}"
	#------------------------------------------------------------------------------------------
	
	echo ""
	echo ""

	# Converting from object array to string (csv-format) by combining functions above---------
	echo "char* convertFromDbObjArr_${dbObjNum}ToCsv(struct $structName** dbIn, char* dataTypeToConvertTo){"
	echo "	// Vars"
	echo "	char* FF = (char*) __func__;"
	echo "	char* returnStr;"
	echo "	char* headerRow;"
	echo ""
	echo "	// Data Validation"
	echo "	if (dbIn == NULL){ myPerror($F_convert, FF, \"Invalid 1st parameter; value is null. Returning null\"); return NULL;}"
	echo "	if (dataTypeToConvertTo == NULL){ myPerror($F_convert, FF, \"Invalid 2nd parameter; value is null. Returning null\"); return NULL;}"
	echo "	if (strcmp(dataTypeToConvertTo, \"string\") != 0 && strcmp(dataTypeToConvertTo, \"char*\") != 0 && strcmp(dataTypeToConvertTo, \"s\") != 0){"
	echo "		myPerror($F_convert, FF, \"Invalid data type to convert to. Can only convert to string. Returning null\");"
	echo "		myPerror($F_convert, FF, \"	Valid 2nd parameter strings: char*, string, s\");"
	echo "		return NULL;"
	echo "	}"
	echo "	//-----------------------------------------------------------------------------------------"
	echo ""

	echo "	// Getting Values"
	echo "	if (dataTypes == NULL){ dataTypes = getTableInfo(\"$csvLoc\", $dataTypesColTypeListColNum);}"
	echo "	if (memberNames == NULL){ memberNames = getTableInfo(\"$csvLoc\", $colNameColTypeListColNum);}"
	echo "	headerRow = makeHeaderRow_$dbObjNum();"
       	echo "	returnStr = createStr(1);"
	echo ""

	echo "	returnStr = myRealloc((int) strlen(headerRow)+2, returnStr);"
	echo "	if (returnStr == NULL){ myPerror($F_convert, FF, \"myRealloc() function returned a null value. Returning null\"); free(headerRow); return NULL;}"
	echo "	strcat(returnStr, headerRow);"
	echo "	strcat(returnStr, \"\\\\n\");"
	echo "	free(headerRow);"
	echo ""

	echo "	// Converting each struct ptr in dbIn to a string and adding it to returnStr"
	echo "	for (int i = 0; i < getArrSize(dbIn); i++){"
	echo "		char* structAsStr = convertFromDbObj_${dbObjNum}ToStr(dbIn[i], dataTypeToConvertTo);"
	echo "		if (structAsStr == NULL){ myPerror($F_convert, FF, \"convertFromDbObj_${dbObjNum}() function returned a null value. Returning null\"); free(returnStr); return NULL;}"
	echo ""
	echo "		returnStr = myRealloc((int) strlen(structAsStr)+2, returnStr);"
	echo "		if (returnStr == NULL){ myPerror($F_convert, FF, \"myRealloc() function returned a null value. Returning null\"); free(structAsStr); return NULL;}"
	echo ""
	echo "		strcat(returnStr, structAsStr);"
	echo "		strcat(returnStr, \"\\\\n\");"
	echo "		free(structAsStr);"
	echo "	}"
	echo ""
	echo "	if (returnStr[strlen(returnStr)] == '\\\\n'){ returnStr[strlen(returnStr)] = '\\\\0';}"
	echo ""

	echo "	globalReturnStrConvert = returnStr;"
	echo "	globalTypeConvert = 1;"
	echo ""
	
	echo "	myFree(dataTypes); myFree(memberNames);"
	echo "	dataTypes = NULL; memberNames = NULL;"
	echo ""
	echo "	return returnStr;"
	echo "}"
	#------------------------------------------------------------------------------------------
}
