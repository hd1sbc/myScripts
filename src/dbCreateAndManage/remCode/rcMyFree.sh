#!/bin/bash

# This function removes code related to a database from myFree.c and myFree.h
#Args: none
remCodeFromMyFree(){
	# Vars
	local FF="${FUNCNAME[0]}"
	lineNumBegin=""
	lineNumAboveBegin=-1
	lineNumAboveBeginContents="holder"
	returnVal=0
	returnVal2=0
	returnVal3=0
	returnVal4=0

	# Checking if there is an empty line above // BEGIN and removing it if so------------------
	lineNumBegin="$(grep -n "// BEGIN $structName" "$myFreeLoc" | head -n 1 | cut -d ':' -f 1)"
	if [ ! "$lineNumBegin" == "" ] && [ $lineNumBegin -gt 2 ]; then
		lineNumAboveBegin=$(expr $lineNumBegin - 1)
		lineNumAboveBeginContents="$(sed -n "$lineNumAboveBegin p" "$myFreeLoc")"
		if [[ "$lineNumAboveBeginContents" =~ ^[[:space:]]*$ ]]; then
			sed -i "${lineNumAboveBegin}d" "$myFreeLoc"
			returnVal=$?
			myPerrorSh $returnVal $F_DBSH $FF "sed command for removing empty line above \"// BEGIN\" returned an error value (!=0). Code may not have been removed. Program will continue"
		fi
	else
		myPerrorSh $F_DBSH $FF "// BEGIN $structName not found in $myFreeLoc. Nothing removed. Program will continue"
		returnVal=-1
	fi
	#------------------------------------------------------------------------------------------

	# Removing all lines between BEGIN and END
	sed -i "/\/\/ BEGIN $structName/,/\/\/ END $structName/d" "$myFreeLoc"
	returnVal2=$?
	myPerrorSh $returnVal2 $F_DBSH $FF "sed command for removing lines between \"// BEGIN\" and \"// END\" returned an error value (!=0). Code may not have been removed. Program will continue"

	# Removing all lines with struct name
	sed -i "/$structName/d" "$myFreeLoc"
	returnVal3=$?
	myPerrorSh $returnVal3 $F_DBSH $FF "sed command for removing lines between \"// BEGIN\" and \"// END\" returned an error value (!=0). Code may not have been removed. Program will continue"

	# Removing all lines with struct name in header file---------------------------------------
	myFreeBaseName="$(getFileNameNoExtSh "$myFreeLoc")"
	headerLoc="${incLoc}${myFreeBaseName}.h"
	if [ ! -f "$headerLoc" ]; then
		myPerrorSh $F_DBSH $FF "$headerLoc not found. Program will continue"
		returnVal=-1
	else
		sed -i "/$structName/d" "$headerLoc"
		returnVal4=$?
		myPerrorSh $returnVal4 $F_DBSH $FF "sed command for removing lines containing $structName in header file for $getArrSize returned an error value (!=0). Code may not have been removed. Program will continue"
	fi
	#------------------------------------------------------------------------------------------

	if [ ! $returnVal -eq 0 ]; then return $returnVal; fi
	if [ ! $returnVal2 -eq 0 ]; then return $returnVal2; fi
	if [ ! $returnVal3 -eq 0 ]; then return $returnVal3; fi
	if [ ! $returnVal4 -eq 0 ]; then return $returnVal4; fi
	return 0

}
