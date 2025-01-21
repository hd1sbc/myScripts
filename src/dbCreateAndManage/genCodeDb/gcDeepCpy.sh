#!/bin/bash

# This function generates c code to copy all bytes of a struct (including the values held in pointer variables)
#Args: none
genCodeDeepCopyObj(){
	echo "struct $structName* deepCpyDbObj_${dbObjNum}(struct $structName* objToCopy){"
	echo "	// Vars"
	echo "	char* FF = (char*) __func__;"
	echo "	struct $structName* copy;"
	echo ""

	echo "	// Data Validation"
	echo "	if (objToCopy == NULL){ myPerror($F_deepCpy, FF, \"Invalid 1st parameter; value is null. Returning null\"); return NULL;}"
	echo ""

	echo "	// Getting Values"
	echo "	copy = malloc(sizeof(struct $structName));"
	echo ""

	# Converting datTypes string to array. Using "read -r -a ..." wasn't working, so did it manually
	dataTypesArr=()
	origIfs=' '
	IFS=' '
	for ele in $dataTypes; do
		dataTypesArr+=("$ele")
	done

	# Adding code based on data type
	i=0
	for varName in $varNames; do
		if [ "$(regexMatchSh "string\\|char\\*" "${dataTypesArr[$i]}")" == "1" ]; then
			echo "	copy->$varName = (char*) calloc(strlen(objToCopy->$varName)+1, sizeof(char));"
			echo "	strcpy(copy->$varName, objToCopy->$varName);"
		elif [ "$(regexMatchSh "^int\\|float\\|bool\\|char$" "${dataTypesArr[$i]}")" == "0" ]; then
			echo "	copy->$varName = (typeof(objToCopy->$varName)) malloc(sizeof(objToCopy->$varName)+1);"
			echo "	memcpy(copy->$varName, objToCopy->$varName, sizeof(objToCopy->$varName)+1);"
		else
			echo "	copy->$varName = objToCopy->$varName;"
		fi

		i=$(expr $i + 1)
	done
	
	echo ""
	echo "	return copy;"
	echo "}"
}

# This function generates c code to copy all bytes of a struct array using the function above
#Args: none
genCodeDeepCopyObjArr(){
	echo "struct $structName** deepCpyDbObjArr_${dbObjNum}(struct $structName** objToCopy){"
	echo "	// Vars"
	echo "	char* FF = (char*) __func__;"
	echo "	int size;"
	echo "	struct $structName** returnObjArr;"
	echo ""

	echo "	// Data Validation"
	echo "	if (objToCopy == NULL){ myPerror($F_deepCpy, FF, \"Invalid 1st parameter; value is null. Returning null\"); return NULL;}"
	echo ""

	echo "	// Getting Values"
	echo "	size = ${structName}GetArrSize(objToCopy);"
	echo "	returnObjArr = malloc(sizeof(struct $structName*)*(size+1));"
	echo ""

	echo "	// Deep copying each object in the array"
	echo "	for (int i = 0; i < size; i++){"
	echo "		returnObjArr[i] = deepCpyDbObj_${dbObjNum}(objToCopy[i]);"
	echo "	}"
	echo "	returnObjArr[size] = createStruct_${dbObjNum}(); //adding sentinal struct"
	echo ""

	echo "	// Freeing colTypeList string arrays"
	echo "	creatStruct(\"free\");"
	echo "	freeSetStructVal();"
	echo ""

	echo "	return returnObjArr;"
	echo "}"
}
