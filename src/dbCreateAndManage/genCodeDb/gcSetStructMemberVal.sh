#!/bin/bash

# This function generates c code to set a struct member value where the struct member name is stored as a variable
#Args: none
genCodeSetStructMemberVal(){
	echo "int setStructMemberVal_$dbObjNum(struct $structName* structIn, char* memberNameIn, void* valueIn){"
	echo "	// Vars"
	echo "	char* FF = (char*) __func__;"
	echo ""
	echo "	// Data Validation-----------------------------------------------------------------"
	echo "	if (structIn == NULL){ myPerror($F_setStructVal, FF, \"Invalid 1st parameter; value is null. Returning -1\"); return -1;}"
	echo "	if (memberNameIn == NULL){ myPerror($F_setStructVal, FF, \"Invalid 2nd parameter; value is null. Returning -1\"); return -1;}"
	echo "	if (strlen(memberNameIn) == 0){ myPerror($F_setStructVal, FF, \"Invalid 2nd parameter; value is an empty string. Returning -1\"); return -1;}"
	echo "	if (valueIn == NULL){ myPerror($F_setStructVal, FF, \"Invalid 3rd parameter; value is null. Returning -1\"); return -1;}"
	echo "	//---------------------------------------------------------------------------------"
	echo ""

	echo "	// Getting Values"
	echo "	if (memberNames == NULL){ memberNames = getTableInfo(\"$csvLoc\", $colNameColTypeListColNum);}"
	echo "	if (memberNames == NULL){ myPerror($F_setStructVal, FF, \"getTableInfo() function returned a null value. Returning -1\"); return -1;}"
	echo ""
	echo "	if (dataTypes == NULL){ dataTypes = getTableInfo(\"$csvLoc\", $dataTypesColTypeListColNum);}"
	echo "	if (dataTypes == NULL){ myPerror($F_setStructVal, FF, \"getTableInfo() function returned a null value. Returning -1\"); myFree(memberNames); return -1;}"
	echo ""

	echo "	// Getting values for each member of the struct by matching member name"
	echo "	// C is a statically-typed language so I can't pass the member name stored in a variable"
	i=0
	origIfs=$IFS
	IFS=' '
	for varName in $varNames; do
		if [ $i -eq 0 ]; then
			echo "	if (strcmp(memberNameIn, \"$varName\") == 0){ structIn->$varName = *(typeof(structIn->$varName)*) valueIn;}"
			i=1
		else
			echo "	else if (strcmp(memberNameIn, \"$varName\") == 0){ structIn->$varName = *(typeof(structIn->$varName)*) valueIn;}"
		fi
	done
	IFS=$origIfs

	echo "	else {"
	echo "		myPerror($F_setStructVal, FF, \"Invalid 2nd parameter; member name \", memberNameIn, \" is not found in struct. Returning -1\");"
	echo "		myFree(memberNames); myFree(dataTypes);"
	echo "		return -1;"
	echo "	}"
	echo ""

	echo "	return 0;"
	echo "}"

	return 0
}
