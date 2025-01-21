#!/bin/bash

# This function removes code related to a database from myFree.c and myFree.h
#Args: none
remCodeFromConvert(){
	# Vars
	local FF="${FUNCNAME[0]}"
	returnVal=0
	returnVal2=0
	returnVal3=0
	returnVal4=0
	returnVal5=0
	returnVal6=0
	returnVal7=0
	returnVal8=0
	returnVal9=0

	# Removing the struct name from the accepted data type string and resizing array-----------
	perl -i -pe "if (/char acceptedDataTypesConvert/) { s/, $structName\*\*//g; }" "$convertLoc"
	returnVal=$?
	myPerrorSh $returnVal $F_DBSH $FF "WARNING: Issue with running perl command for removing struct name from accepted data type. Program will continue"
	sizeAccDataTypesChar="$(sed -n '/char acceptedDataTypesConvert\[.*\] = /p' "$convertLoc" | sed 's/.*\[//g' | sed 's/\].*//g')"
	if [ ! "$(isIntSh "$sizeAccDataTypesChar")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var sizeAccDataTypesChar (value: $sizeAccDataTypesChar) for removing struct name from accepted data type string is not a valid integer. Returning -1"
		return -1
	fi
	sizeAccDataTypesAdjusted=$(expr $sizeAccDataTypesChar - "$(echo "$structName" | wc -c)" - 2)
	newAccDataTypes="$(sed -n '/char acceptedDataTypesConvert\[.*\] = /p' "$convertLoc" | sed "s/\[.*\]/\[$sizeAccDataTypesAdjusted\]/g")"
	newAccDataTypesAdjustedForSed="$(echo "$newAccDataTypes" | sed 's/\//\\\//g')"
	sed -i "s/char acceptedDataTypesConvert\[.*\] = .*/$newAccDataTypesAdjustedForSed/g" "$convertLoc"
	returnVal2=$?
	myPerrorSh $returnVal2 $F_DBSH $FF "sed command for removing $structName from accepted data types char array returned an error value (!=0). Code may not have been removed. Program will continue"
	#------------------------------------------------------------------------------------------

	#------------------------------------------------------------------------------------------
	# Removing line in conditional in convertFromStr that doesn't contain struct name and closing '}' of conditional
	strToLookFor="		globalReturn${structName}Convert = convertFromCsvToDbObjArr_${dbObjNum}(strIn);"
	lineNumStrToLookFor="$(grep -n "$strToLookFor" "$convertLoc" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNumStrToLookFor")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var lineNumStrToLookFor (value: $lineNumStrToLookFor) for conditional line in convertFromStr is not a valid integer. Returning -1"
		return -1
	fi

	# Removing line in conditional that doesn't contain struct name- - - - - - - - - - - - - -
	lineNumAdjusted=$(expr $lineNumStrToLookFor + 1)
	sed -i "${lineNumAdjusted}d" "$convertLoc"
	returnVal3=$?
	myPerrorSh $returnVal3 $F_DBSH $FF "sed command for removing line in conditional that doesn't contain $structName returned an error value (!=0). Code may not have been removed. Program will continue"
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	# Removing closing bracket- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	lineNumAdjusted=$(expr $lineNumStrToLookFor + 2) #subtracting 2 instead of 3 cuz previous line was removed
	sed -i "${lineNumAdjusted}d" "$convertLoc"
	returnVal4=$?
	myPerrorSh $returnVal4 $F_DBSH $FF "sed command for removing closing bracket in conditional that doesn't contain $structName returned an error value (!=0). Code may not have been removed. Program will continue"
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	# Removing extra space below conditional - - - - - - - - - - - - - - - - - - - - - - - - -
	lineNumAdjusted=$(expr $lineNumStrToLookFor + 2) #subtracting 2 instead of 3 cuz previous 2 lines were removed
	sed -i "${lineNumAdjusted}d" "$convertLoc"
	returnVal5=$?
	myPerrorSh $returnVal5 $F_DBSH $FF "sed command for removing extra line after closing bracket of conditional that doesn't contain $structName returned an error value (!=0). Code may not have been removed. Program will continue"
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	#------------------------------------------------------------------------------------------
	
	# Checking if there is an empty line above // BEGIN and removing it if so------------------
	lineNumBegin="$(grep -n "// BEGIN $structName" "$convertLoc" | head -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNumBegin")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var lineNumBegin (value: $lineNumBegin) for line beginning with \"// BEGIN\" is not a valid integer. Returning -1"
		return -1
	fi
	lineNumAboveBegin=-1
	lineNumAboveBeginContents="holder"
	if [ ! "$lineNumBegin" == "" ] && [ $lineNumBegin -gt 2 ]; then
		lineNumAboveBegin=$(expr $lineNumBegin - 1)
		lineNumAboveBeginContents="$(sed -n "$lineNumAboveBegin p" "$convertLoc")"
		if [[ "$lineNumAboveBeginContents" =~ ^[[:space:]]*$ ]]; then
			sed -i "${lineNumAboveBegin}d" "$convertLoc"
			returnVal6=$?
			myPerrorSh $returnVal6 $F_DBSH $FF "sed command for removing extra line above \"// BEGIN\" returned an error value (!=0). Code may not have been removed. Program will continue"
		fi
	else
		myPerrorSh $F_DBSH $FF "// BEGIN $structName not found in $convertLoc. Nothing removed. Program will continue"
	fi
	#------------------------------------------------------------------------------------------

	# Removing all lines between BEGIN and END
	sed -i "/\/\/ BEGIN $structName/,/\/\/ END $structName/d" "$convertLoc"
	returnVal7=$?
	myPerrorSh $returnVal7 $F_DBSH $FF "sed command for removing lines between \"// BEGIN\" and \"// END\" returned an error value (!=0). Code may not have been removed. Program will continue"
	
	# Removing all lines with struct name
	sed -i "/$structName/d" "$convertLoc"
	returnVal8=$?
	myPerrorSh $returnVal8 $F_DBSH $FF "sed command for removing lines containing $structName returned an error value (!=0). Code may not have been removed. Program will continue"

	# Removing all lines with struct name in header file---------------------------------------
	convertBaseName="$(getFileNameNoExtSh "$convertLoc")"
	headerLoc="${incLoc}${convertBaseName}.h"
	if [ ! -f "$headerLoc" ]; then
		myPerrorSh $F_DBSH $FF "$headerLoc not found for $convertLoc. Returning -1"
		return -1
	fi
	#------------------------------------------------------------------------------------------

	sed -i "/$structName/d" "$headerLoc"
	returnVal9=$?
	myPerrorSh $returnVal9 $F_DBSH $FF "sed command for removing lines containing $structName in $headerLoc returned an error value (!=0). Code may not have been removed. Program will continue"

	if [ ! $returnVal -eq 0 ]; then return $returnVal; fi
	if [ ! $returnVal2 -eq 0 ]; then return $returnVal2; fi
	if [ ! $returnVal3 -eq 0 ]; then return $returnVal3; fi
	if [ ! $returnVal4 -eq 0 ]; then return $returnVal4; fi
	if [ ! $returnVal5 -eq 0 ]; then return $returnVal5; fi
	if [ ! $returnVal6 -eq 0 ]; then return $returnVal6; fi
	if [ ! $returnVal7 -eq 0 ]; then return $returnVal7; fi
	if [ ! $returnVal8 -eq 0 ]; then return $returnVal8; fi
	if [ ! $returnVal9 -eq 0 ]; then return $returnVal9; fi
	return 0
}
