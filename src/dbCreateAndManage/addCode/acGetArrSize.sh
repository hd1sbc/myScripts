#!/bin/bash

# This function adds code to getArrSize.c and getArrSize.h
#Args: none
addCodeToGetArrSize(){
	# Vars
	local FF="${FUNCNAME[0]}"
	lineNum=""
	generatedCode=""
	headOfFile=""
	tailOfFile=""

	# Data Validation
	if [ ! "$(cat "$getArrSizeLoc" | grep "// BEGIN $structName")" == "" ]; then
		myPerrorSh $F_DBSH $FF "File $getArrSizeLoc already contains code for $structName; // BEGIN $structName was found in file. Nothing added to $getArrSizeLoc. Returning -1"
		return -1
	fi
	if [ ! -f "$getArrSizeLoc" ]; then myPerrorSh $F_DBSH $FF "File $getArrSizeLoc wasn't found. Returning -1"; return -1; fi

	# Getting Values---------------------------------------------------------------------------
	lineNum="$(grep -n "// END " "$getArrSizeLoc" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNum")" == "1" ]; then
		lineNum="$(grep -n "}" "$getArrSizeLoc" | tail -n 1 | cut -d ':' -f 1)"
		if [ ! "$(isIntSh "$lineNum")" == "1" ]; then
			myPerrorSh $F_DBSH $FF "Line num $lineNum for .c file is not a valid integer. Returning -1"
			return -1
		fi
	fi
	lineNumAdj="$(expr "$lineNum" + 1)"
	stringToAdd="// BEGIN $structName\n$(genCodeGetArrSize)\n// END $structName" #generated code
	if [ "$stringToAdd" == "" ]; then myPerrorSh $F_DBSH $FF "WARNING: genCodeGetArrSize() function returned an empty string. Program will continue"; fi
	insertTxtInFile "$stringToAdd" -l "$lineNumAdj" "$getArrSizeLoc"
	myPerrorSh $? $F_DBSH $FF "WARNING: insertTxtInFile for file $getArrSizeLoc returned an error value (!=0). Program will continue"
	#------------------------------------------------------------------------------------------

	# Adding define code to header (.h) file---------------------------------------------------
	getArrSizeBaseName="$(getFileNameNoExtSh "$getArrSizeLoc")"
	headerLoc="${incLoc}${getArrSizeBaseName}.h"

	if [ ! -f "$headerLoc" ]; then
		myPerrorSh $F_DBSH $FF "Header file $headerLoc not found for $getArrSizeLoc. Returning -1"
		return -1
	fi


	if [ "$(grep "struct $structName\*\*:" "$headerLoc")" == "" ]; then
		lineNumDefault="$(grep -n "default:" "$headerLoc" | tail -n 1 | cut -d ':' -f 1)"
		#using perl cuz sed doesn't print leading and trailing whitespace
		perl -i -pe "if ($. == $lineNumDefault) { print \"		struct $structName**: ${structName}GetArrSize, \\\\\\n\"; }" "$headerLoc"
		myPerrorSh $? $F_DBSH $FF "WARNING: Issue with running perl command for .h file $headerLoc. Program will continue"
	else
		myPerrorSh $F_DBSH $FF "Code for define statement in .h file $headerLoc already found. Nothing added. Program will continue"
	fi
	#------------------------------------------------------------------------------------------

	return 0
}
