#!/bin/bash

# This function generates c code to take a struct member name in as a string and return it's value
#Args: none
genCodeGetStructMemberVal(){
	# Vars
	i=0
	echo "void* getStructMemberVal_$dbObjNum(struct $structName* structIn, char* memberNameIn){"
	echo "	// Vars"
	echo "	char* FF = (char*) __func__;"
	echo "	void* returnVal;"
	echo ""
	echo "	// Data Validation"
	echo "	if (structIn == NULL){ myPerror($F_getStructVal, FF, \"Invalid 1st parameter; value is null. Returning null\"); return NULL;}"
	echo "	if (memberNameIn == NULL){ myPerror($F_getStructVal, FF, \"Invalid 2nd parameter; value is null. Returning null\"); return NULL;}"
	echo ""

	echo "	// Getting values for each member of the struct by matching member name"
	echo "	// C is a statically-typed language so I can't pass the member name stored in a variable"
	origIfs=$IFS
	IFS=' '
	for varName in $varNames; do
		if [ $i -eq 0 ]; then
			echo -n "	if (strcmp(memberNameIn, \"$varName\") == 0){ "
			i=1
		else
			echo -n "	else if (strcmp(memberNameIn, \"$varName\") == 0){ "
		fi

		echo -n "returnVal = (void*) &structIn->$varName;"
		echo "}"
	done
	IFS=$origIfs

	echo "	else {"
	echo "		myPerror($F_getStructVal, FF, \"Invalid 2nd parameter; member name is not found in struct. Returning null\");"
	echo "		return NULL;"
	echo "	}"
	echo ""
	echo "	return returnVal;"
	echo "}"

	return 0
}
