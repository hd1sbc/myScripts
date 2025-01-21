#!/bin/bash

# This function generates c code to remove all struct element(s) from a struct array based on a value in a specified column
#Args: none
genCodeRemoveRowOnVal(){
	echo "struct $structName** removeElementFromDbArrBasedOnVal_${dbObjNum}(struct $structName** objIn, int colNum, void* val){"
	# Converting datTypes string to array. Using "read -r -a ..." wasn't working, so did it manually
	IFS=' '
	dataTypesArr=()
	for ele in $dataTypes; do
		dataTypesArr+=("$ele")
	done

	echo "	// Vars"
	echo "	char* FF = (char*) __func__;"
	echo "	int size;"
	echo "	int numCols = "${#dataTypesArr[@]}"; //this num is auto-updated"
	echo "	int iAdj = 0;"
	echo "	int newSize = 0;"
	echo ""

	echo "	// Data Validation"
	echo "	if (val == NULL){ myPerror($F_remRowVal, FF, \"Invalid 2nd parameter (value); value is null. Returning 3rd parameter (database)\"); return objIn;}"
	echo "	if (objIn == NULL){ myPerror($F_remRowVal, FF, \"Invalid 3rd parameter (database); value is null. Returning null\"); return NULL;}"
	echo ""

	echo "	// Getting Values"
	echo "	size = getArrSize(objIn);"
	echo "	if (size < 0){ myPerror($F_remRowVal, FF, \"getArrSize() function returned an error value (<0). Returning null\"); return NULL;}"
	echo "	if (size == 0){ myPerror($F_remRowVal, FF, \"WARNING: 1st parameter has a size of 0. Returning 1st parameter\"); return objIn;}"
	echo "	if (colNum > numCols - 1){"
	echo "		myPerror($F_remRowVal, FF, \"Invalid 1st parameter (id); value is larger than the number of columns: \", numCols, \". Returning 3rd parameter (database)\");"
	echo "		return objIn;"
	echo "	}"
	echo ""

	echo "	// Iterating over every row and comparing values based on column num"
	echo "	for (int i = 0; i < size; i++){"
	echo "		//since potentially removing rows, the db could be smaller than the original size"
	echo "		//this conditional statement prevents buffer overflows from that"
	echo "		if (objIn[iAdj]->id == -1){ return objIn;}"


	# Adding code based on data type
	i=0
	for varName in $varNames; do
		if [ "$(regexMatchSh "string\\|char\\*" "${dataTypesArr[$i]}")" == "1" ]; then
			echo "		else if(colNum == $i && strcmp(objIn[iAdj]->$varName, (char*) val) == 0){"
			echo "			objIn = removeElementFromDbArr_${dbObjNum}(objIn, objIn[iAdj]->id);"
			echo "		}"
		elif [ "$(regexMatchSh "^int\\|float\\|bool\\|char$" "${dataTypesArr[$i]}")" == "0" ]; then
			echo "		else if(colNum == $i && memcmp(objIn[iAdj]->$varName, (typeof(objIn[iAdj]->$varName)) val) == 0){"
			echo "			objIn = removeElementFromDbArr_${dbObjNum}(objIn, objIn[iAdj]->id);"
			echo "		}"
		else
			echo "		else if(colNum == $i && objIn[iAdj]->$varName == *(typeof(objIn[iAdj]->$varName)*) val){"
			echo "			objIn = removeElementFromDbArr_${dbObjNum}(objIn, objIn[iAdj]->id);"
			echo "		}"
		fi


		i=$(expr $i + 1)
	done
	echo "		else{ iAdj++;}"
	echo "	}"
	echo ""

	echo "	newSize = getArrSize(objIn);"
	echo "	if (newSize < 0){ myPerror($F_remRowVal, FF, \"getArrSize() function returned an error value (<0). Returning null\"); return NULL;}"
	echo "	if (newSize == 0){ myPerror($F_remRowVal, FF, \"WARNING: All elements removed; struct is empty. Returning object with only sentinel struct\");}"
	echo "	else if (newSize == size){ myPerror($F_remRowVal, FF, \"No elements removed. Returning parameter 3 (database)\");}"
	echo ""

	echo "	return objIn;"
	echo "}"

}
