#!/bin/bash

# This function adds code to createStruct.c and createStruct.h. Can't use generic "addCodeToDefine" cuz this needs to add some extra code 
#Args: 1 - file path; 2 - function code string; 3 - function name
addCodeToCreatStruct(){
	# Var
	local FF="${FUNCNAME[0]}"
	returnVal1=0
	returnVal2=0

	# Data Validation
	if [ ! -f "$1" ]; then myPerrorSh $F_DBSH $FF "Invalid arg 1; file $1 doesn't exist. Returning -1"; return -1; fi
	if [ ! "$(grep "$3" "$1")" == "" ]; then
		myPerrorSh $F_DBSH $FF "Function $functionName already exists in $1. Nothing added to file. Returning -1"
		return -1
	fi

	# Adding code above "PRIMARY FUNCTION"
	lineNumPrimaryFunc="$(grep -n "// PRIMARY FUNCTION" "$1" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNumPrimaryFunc")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var lineNumPrimaryFunc (value: $lineNumPrimaryFunc) for file $1 is not a valid integer. Returning -1"
		return -1
	fi
	lineNumAdjusted=$(expr "$lineNumPrimaryFunc" - 5)

	insertTxtInFile "$2" -l "$lineNumAdjusted" "$1"
	returnVal1=$?
	myPerrorSh $returnVal1 $F_DBSH $FF "WARNING: insertTxtInFile for file $1 for adding above \"PRIMARY FUNCTION\" in .c file returned an error value (!=0). Code may not have been added to the file. Program will continue."
	#------------------------------------------------------------------------------------------

	# Inserting above conditional in "PRIMARY FUNCTION"----------------------------------------
	lineNumLastElse="$(grep -n "else{" "$1" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNumLastElse")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var lineNumLastElse (value: $lineNumLastElse) for file $1 is not a valid integer. Returning -1"
		return -1
	fi

	stringToAdd="	else if (strcmp(structNameIn, \"$structName\") == 0){ return createStruct_${dbObjNum}();}"
	insertTxtInFile "$stringToAdd" -l "$lineNumLastElse" "$1"
	returnVal2=$?
	myPerrorSh $returnVal2 $F_DBSH $FF "WARNING: insertTxtInFile for file $1 for adding above conditional in .c file returned an error value (!=0). Code may not have been added to the file. Program will continue."
	#------------------------------------------------------------------------------------------

	# If one of the return vals doesn't equal 0, return that
	if [ ! $returnVal1 -eq 0 ]; then
		return $returnVal1
	elif [ ! $returnVal2 -eq 0 ]; then
		return $returnVal2
	else
		return 0
	fi
}
