#!/bin/bash

# This function generates c code to find all structs in a struct array that have a value at a specified col matching the given equality sign
#Args: none
genCodeFindAllRows(){
	echo "struct $structName** findAllElementsFromDbArrBasedOnVal_${dbObjNum}(struct $structName** objIn, int colNum, void* val, char* equality){"
	
	# Converting datTypes string to array. Using "read -r -a ..." wasn't working, so did it manually
	IFS=' '
	dataTypesArr=()
	for ele in $dataTypes; do
		dataTypesArr+=("$ele")
	done

	echo "	// Vars"
	echo "	char* FF = (char*) __func__;"
	echo "	int size;"
	echo "	int numCols = "${#dataTypesArr[@]}";"
	echo "	int returnArrSize = 0;"
	echo "	int equ;"
	echo "	struct $structName** returnArr;"
	echo ""

	echo "	// Data Validation-------------------------------------------------------------------------"
	echo "	if (val == NULL){ myPerror($F_getRowsWhere, FF, \"Invalid 2nd parameter (value); value is null. Returning 3rd parameter (database)\"); return objIn;}"
	echo "	if (objIn == NULL){ myPerror($F_getRowsWhere, FF, \"Invalid 3rd parameter (database); value is null. Returning null\"); return NULL;}"
	echo "	if (equality == NULL){ myPerror(F_GETROWSWHERE, FF, \"Invalid 4th parameter (equality value); value is null. Returning null\"); return NULL;}"
	echo "	//-------------------------------------------------------------------------------------------"
	echo ""

	echo "	// Getting Values--------------------------------------------------------------------------"
	echo "	size = getArrSize(objIn);"
	echo "	if (size < 0){ myPerror($F_getRowsWhere, FF, \"getArrSize() function returned an error value (<0). Returning null\"); return NULL;}"
	echo "	if (colNum > numCols){"
	echo "		myPerror($F_getRowsWhere, FF, \"Invalid 1st parameter (id); value is larger than the number of columns: \", numCols, \". Returning 3rd parameter (database)\");"
	echo "		return objIn;"
	echo "	}"
	echo ""
	echo "	if (strcmp(equality, \"==\") == 0){ equ = 0;}"
	echo "	else if (strcmp(equality, \"!=\") == 0){ equ = 1;}"
	echo "	else if (strcmp(equality, \">\") == 0){ equ = 2;}"
	echo "	else if (strcmp(equality, \"<\") == 0){ equ = 3;}"
	echo "	else if (strcmp(equality, \">=\") == 0){ equ = 4;}"
	echo "	else if (strcmp(equality, \"<=\") == 0){ equ = 5;}"
	echo "	else{"
	echo "		myPerror(F_GETROWSWHERE, FF, \"Invalid 4th parameter (equality value); value is not valid. Can either be ==, !=, >, <, >=, or <=. Returning null\");"
	echo "		return NULL;"
	echo "	}"

	echo "	returnArr = malloc(sizeof(struct $structName*)*(size+1));"
	echo "	//-------------------------------------------------------------------------------------------"
	echo ""

	echo "	// Iterating over every row and comparing values based on column num"
	echo "	for (int i = 0; i < size; i++){"

	# Adding code based on data type
	i=0
	for varName in $varNames; do
		if [ $i -eq 0 ]; then
			echo -n "		if "
		else
			echo -n "		else if "
		fi
		
		# If data type is a string (also marked as char*)
		if [ "$(regexMatchSh "string\\|char\\*" "${dataTypesArr[$i]}")" == "1" ]; then
			echo "(colNum == $i){"
			echo -n "			" #3 tabs
			echo "if (equ == 0 && strcmp(objIn[i]->$varName, (char*) val) == 0){"
			echo -n "				" #4 tabs
			echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 1 && strcmp(objIn[i]->$varName, (char*) val) != 0){"
			echo -n "				" #4 tabs
		       	echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 2 && strcmp(objIn[i]->$varName, (char*) val) > 0){"
			echo -n "				" #4 tabs
			echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 3 && strcmp(objIn[i]->$varName, (char*) val) < 0){"
			echo -n "				" #4 tabs
		       	echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 4 && strcmp(objIn[i]->$varName, (char*) val) >= 0){"
			echo -n "				" #4 tabs
			echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 5 && strcmp(objIn[i]->$varName, (char*) val) <= 0){"
			echo -n "				" #4 tabs
			echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo "		}"
		# If data type is NOT a simple data type (int, float, bool, char), then do memcmp
		elif [ "$(regexMatchSh "^int\\|float\\|bool\\|char$" "${dataTypesArr[$i]}")" == "0" ]; then
			echo "(colNum == $i){"
			echo -n "			" #3 tabs
			echo "if (equ == 0 && memcmp(objIn[i]->$varName, (typeof(objIn[i]->$varName)) val) == 0){"
			echo -n "				" #4 tabs
		       	echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 1 && memcmp(objIn[i]->$varName, (typeof(objIn[i]->$varName)) val) != 0){"
			echo -n "				" #4 tabs
			echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 2 && memcmp(objIn[i]->$varName, (typeof(objIn[i]->$varName)) val) > 0){"
			echo -n "				" #4 tabs
			echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 3 && memcmp(objIn[i]->$varName, (typeof(objIn[i]->$varName)) val) < 0){"
			echo -n "				" #4 tabs
			echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 4 && memcmp(objIn[i]->$varName, (typeof(objIn[i]->$varName)) val) >= 0){"
			echo -n "				" #4 tabs
		       	echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 5 && memcmp(objIn[i]->$varName, (typeof(objIn[i]->$varName)) val) <= 0){"
			echo -n "				" #4 tabs
			echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo "		}"

		# If data type is a simple data type (int, float, bool, char)
		else
			echo "(colNum == $i){"
			echo -n "			" #3 tabs
			echo "if (equ == 0 && objIn[i]->$varName == *(typeof(objIn[i]->$varName)*) val){"
			echo -n "				" #4 tabs
		       	echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 1 && objIn[i]->$varName != *(typeof(objIn[i]->$varName)*) val){"
			echo -n "				" #4 tabs
		       	echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 2 && objIn[i]->$varName > *(typeof(objIn[i]->$varName)*) val){"
			echo -n "				" #4 tabs
			echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 3 && objIn[i]->$varName < *(typeof(objIn[i]->$varName)*) val){"
			echo -n "				" #4 tabs
			echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 4 && objIn[i]->$varName >= *(typeof(objIn[i]->$varName)*) val){"
			echo -n "				" #4 tabs
			echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo -n "			" #3 tabs
			echo "else if (equ == 5 && objIn[i]->$varName <= *(typeof(objIn[i]->$varName)*) val){"
			echo -n "				" #4 tabs
			echo "returnArr[returnArrSize] = deepCpyDbObj_${dbObjNum}(objIn[i]); returnArrSize++;"
			echo -n "			" #3 tabs
			echo "}"

			echo "		}"
		fi
		i=$(expr $i + 1)
	done
	echo "	}"
	echo ""

	echo "	if (returnArrSize == 0){ myPerror($F_getRowsWhere, FF, \"WARNING: No matches found. Returning sentinel struct\");}"
	echo "	returnArr[returnArrSize] = createStruct_${dbObjNum}();"
	echo ""

	echo "	creatStruct(\"free\");"
	echo "	freeSetStructVal();"
	echo ""

	echo "	return returnArr;"
	echo "}"
}
