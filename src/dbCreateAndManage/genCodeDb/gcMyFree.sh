#!/bin/bash

# This function generates c code to add to myFree.c
#Args: none
genCodeMyFree(){
	echo "int myFreeDbObjArr_$dbObjNum(int numPtrsIn, void* arrIn, int printBoolIn){"
	echo "	// Recursion base case"
	echo "	if (numPtrsIn == 1){"
	echo "		struct $structName* objPtr = (struct $structName*) arrIn;"
	echo "		if (printBoolIn > 0 && objPtr->"$(echo $varNames | cut -d ' ' -f 1)" != -1){ printTable(objPtr);}"
	echo "		if (printBoolIn != 2){ //freeing each dynamically alloc'd element in the struct and the pointer"

	i=1
	IFS=' '
	for dataType in $dataTypes; do
		if [ "$dataType" == "string" ] || [ ! "$(echo "$dataType" | sed -n '/\*/p')" == "" ]; then
			currVarName="$(echo $varNames | cut -d ' ' -f $i)"
			echo "			if (objPtr->"$currVarName" != NULL){free(objPtr->"$currVarName");}"
		fi
		i=$(expr $i + 1)
	done
	echo ""
	echo "			free(objPtr); //free the pointer"
	echo "		}"
	echo "	}"
	echo ""

	echo "	// Recursion"
	echo "	else {"
	echo "		if (numPtrsIn == 2){"
	echo "			struct $structName** ptr = (struct $structName**) arrIn;"
	echo "			if (printBoolIn != 0){ printHeaderRowFilesAndFolders();}"
	echo "			int i = 0;"
	echo "			for (; ptr[i]->"$(echo $varNames | cut -d ' ' -f 1)" != -1; i++){ myFreeDbObjArr_$dbObjNum(numPtrsIn-1, (void*) ptr[i], printBoolIn);}"
	echo ""
	echo "			//need to free the sentinel struct since it isn't part of the for loop"
	echo "			//the values of the default sentinel struct don't need to freed cuz they weren't dynamically allocated, unlike the other structs in the array"
	echo "			if (printBoolIn != 2){ free(ptr[i]); free(ptr);}"
	echo "		}"
	echo "		if (numPtrsIn == 3){"
	echo "			struct $structName*** ptr = (struct $structName***) arrIn;"
	echo "			int i = 0;"
	echo "			for (; ptr[i][0]->id != -1; i++){ myFreeDbObjArr_$dbObjNum(numPtrsIn-1, (void*) ptr[i], printBoolIn);}"
	echo "			//need to free the sentinel struct since it isn't part of the for loop"
	echo "			//the values of the default sentinel struct don't need to freed cuz they weren't dynamically allocated, unlike the other structs in the array"
	echo "			if (printBoolIn != 2){ free(ptr[i]); free(ptr);}"
	echo "		}"
	echo "	}"
	echo ""
	echo "	return 0;"
	echo "}"
}
