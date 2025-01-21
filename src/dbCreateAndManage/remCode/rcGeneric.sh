#!/bin/bash

# This function combines the function above to add the auto-generated code and define statements to the file
#Args: 1 - file path
remCodeFromFiles(){
	# Vars
	local FF="${FUNCNAME[0]}"
	lineNumBegin=""
	lineNumAboveBegin=-1
	lineNumAboveBeginContents="holder"
	returnVal=0
	returnVal2=0
	returnVal3=0
	returnVal4=0

	# Data Validation
	if [ ! -f "$1" ]; then myPerrorSh $F_DBSH $FF "File $1 was not found. Nothing removed. Returning -1"; return -1; fi

	# Checking if there is an empty line above // BEGIN and removing it if so------------------
	lineNumBegin="$(grep -n "// BEGIN $structName" "$1" | head -n 1 | cut -d ':' -f 1)"
	if [ ! "$lineNumBegin" == "" ] && [ $lineNumBegin -gt 2 ]; then
		lineNumAboveBegin=$(expr $lineNumBegin - 1)
		lineNumAboveBeginContents="$(sed -n "$lineNumAboveBegin p" "$1")"
		if [[ "$lineNumAboveBeginContents" =~ ^[[:space:]]*$ ]]; then
			sed -i "${lineNumAboveBegin}d" "$1"
			returnVal=$?
			myPerrorSh $returnVal $F_DBSH $FF "sed command for removing empty line above \"// BEGIN\" returned an error value (!=0). Code may not have been removed. Program will continue"
		fi
	else
		myPerrorSh $F_DBSH $FF "// BEGIN $structName not found in $1. Nothing removed. Program will continue"
		returnVal=-1
	fi
	#------------------------------------------------------------------------------------------

	# Removing all lines between BEGIN and END
	sed -i "/\/\/ BEGIN $structName/,/\/\/ END $structName/d" "$1"
	returnVal2=$?
	myPerrorSh $returnVal2 $F_DBSH $FF "sed command for removing lines between \"// BEGIN\" and \"// END\" returned an error value (!=0). Code may not have been removed. Program will continue"

	# Removing all lines with struct name
	sed -i "/$structName/d" "$1"
	returnVal3=$?
	myPerrorSh $returnVal3 $F_DBSH $FF "sed command for removing lines between \"// BEGIN\" and \"// END\" returned an error value (!=0). Code may not have been removed. Program will continue"

	# Removing all lines with struct name in header file
	arg1BaseName="$(getFileNameNoExtSh "$1")"
	headerLoc="${incLoc}${arg1BaseName}.h"
	if [ ! -f "$headerLoc" ]; then
		myPerrorSh $F_DBSH $FF "$headerLoc not found. Program will continue"
		return -1
	fi
	
	sed -i "/$structName/d" "$headerLoc"
	returnVal4=$?
	myPerrorSh $returnVal4 $F_DBSH $FF "sed command for removing lines containing $structName in header file for $1 returned an error value (!=0). Code may not have been removed. Program will continue"

	if [ ! $returnVal -eq 0 ]; then return $returnVal; fi
	if [ ! $returnVal2 -eq 0 ]; then return $returnVal2; fi
	if [ ! $returnVal3 -eq 0 ]; then return $returnVal3; fi
	if [ ! $returnVal4 -eq 0 ]; then return $returnVal4; fi
	return 0
}
