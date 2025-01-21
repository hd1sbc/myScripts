#!/bin/bash

# This function adds code to convert.c and convert.h
#Args: none
addCodeToConvert(){
	# Vars
	local FF="${FUNCNAME[0]}"
	lastReturnType=""

	# Data Validation
	if [ ! "$(grep "// BEGIN $structName" "$convertLoc")" == "" ]; then
		myPerrorSh $F_DBSH $FF "$structName code already exists in file $convertLoc. Nothing added to file. Returning -1"
		return -1
	fi

	# Adding return types comment (both top and bottom of file)--------------------------------
	#regex: finding lines beginning with // [0-9] :-",. Finding the last number. Removing "// ". Removing everything after (and including) ":-"
	lastReturnType="$(sed -n '/\/\/\s\+[0-9]\+ :- /p' "$convertLoc" | tail -n 1 | sed 's/\/\/	//g' | sed 's/ :-.*//g')"
	if [ ! "$(isIntSh "$lastReturnType")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var lastReturnType (value: $lastReturnType) is not a valid int. Returning -1"
		return -1
	fi
	nextReturnTypeNum=$(expr $lastReturnType + 1)

	# Seeing if return type comment already exists
	if [ ! "$(grep "//	$lastReturnType :- $structName\*\*" "$convertLoc")" == "" ]; then
		myPerrorSh $F_DBSH $FF "Return type already listed in file $convertLoc. Not adding to comments. Nothing added. Program will continue"
		nextReturnTypeNum=$((lastReturnType))
	else
		insertTxtInFile "//	$nextReturnTypeNum :- $structName**" -p "//	$lastReturnType :-" -a -a "$convertLoc"
		myPerrorSh $? $F_DBSH $FF "WARNING: insertTxtInFile for $convertLoc for adding return types comment returned an error value (!=0). Code may not have been added to the file. Program will continue."
	fi
	#------------------------------------------------------------------------------------------

	# Add global return------------------------------------------------------------------------
	lineNumGlobalReturn="$(grep -n "int globalTypeConvert = 0;" "$convertLoc" | head -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNumGlobalReturn")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var lineNumGlobalReturn (value: $lineNumGlobalReturn) is not a valid int. Returning -1"
		return -1
	fi

	if [ ! "$(grep "struct $structName\*\* globalReturn${structName}Convert" "$convertLoc")" == "" ]; then
		myPerrorSh $F_DBSH $FF "$structName global variable (globalReturn$structName) already exists in c file $convertLoc. Nothing added. Program will continue"
	else
		insertTxtInFile "struct $structName** globalReturn${structName}Convert = NULL;" -l "$lineNumGlobalReturn" "$convertLoc"
		myPerrorSh $? $F_DBSH $FF "WARNING: insertTxtInFile for $convertLoc for adding global return returned an error value (!=0). Code may not have been added to the file. Program will continue."
	fi
	#------------------------------------------------------------------------------------------

	# Add accepted data types convert (increase size of arr)-----------------------------------
	if [ ! "$(sed -n '/char acceptedDataTypesConvert\[.*\] = /p' "$convertLoc" | sed -n "/$structName/p")" == "" ]; then
		myPerrorSh $F_DBSH $FF "\"$structName**\" already exists in the accepted data types string var in file $convertLoc. Nothing added to that string. Program will continue"
	else
		sizeAccDataTypesChar="$(sed -n '/char acceptedDataTypesConvert\[.*\] = /p' "$convertLoc" | sed 's/.*\[//g' | sed 's/\].*//g')"
		if [ ! "$(isIntSh "$sizeAccDataTypesChar")" == "1" ]; then
			myPerrorSh $F_DBSH $FF "Var sizeAccDataTypesChar (value: $sizeAccDataTypesChar) is not a valid int. Returning -1"
			return -1
		fi
		sizeAccDataTypesAdjusted=$(expr $sizeAccDataTypesChar + "$(echo "$structName" | wc -c)" + 2)
		newAccDataTypes="$(sed -n '/char acceptedDataTypesConvert\[.*\] = /p' "$convertLoc" | sed "s/\";/, $structName**\";/g" | sed "s/\[.*\]/\[$sizeAccDataTypesAdjusted\]/g")"
		newAccDataTypesAdjustedForSed="$(echo "$newAccDataTypes" | sed 's/\//\\\//g')"
		sed -i "s/char acceptedDataTypesConvert\[.*\] = .*/$newAccDataTypesAdjustedForSed/g" "$convertLoc"
		myPerrorSh $? $F_DBSH $FF "WARNING: Issue with running sed command for adding accepted data types convert var in $convertLoc. Program will continue"

	fi
	#------------------------------------------------------------------------------------------

	# Convert define---------------------------------------------------------------------------
	lineNumWhile="$(grep -n "} while(0);" "$convertLoc" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNumWhile")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var lineNumWhile (value: $lineNumWhile) is not a valid int. Returning -1"
		return -1
	fi

	stringToAdd="		else if (globalTypeConvert == $nextReturnTypeNum){ returnData = (struct $structName**) globalReturn${structName}Convert;} \\\\"
	if [ ! "$(echo "$stringToAdd" | sed 's/\\//g' | grep -F -f - "$convertLoc")" == "" ]; then
		myPerrorSh $F_DBSH $FF "$structName already found in convert define statement in c file $convertLoc. Nothing added. Program will continue"
	else
		insertTxtInFile "$stringToAdd" -l "$lineNumWhile" "$convertLoc"
		myPerrorSh $? $F_DBSH $FF "WARNING: insertTxtInFile for $convertLoc for adding to define in c file returned an error value (!=0). Code may not have been added to the file. Program will continue."
	fi
	#------------------------------------------------------------------------------------------

	# Convert Helper---------------------------------------------------------------------------
	lineNumBadArgs="$(grep -n "default: badArgPos1" "$convertLoc" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNumBadArgs")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var lineNumBadArgs (value: $lineNumBadArgs) is not a valid int. Returning -1"
		return -1
	fi

	stringToAdd="		struct $structName**: convertFromDbObjArr_${dbObjNum}ToCsv, \\\\"
	if [ ! "$(echo "$stringToAdd" | sed 's/\\//g' | grep -F -f - "$convertLoc")" == "" ]; then
		myPerrorSh $F_DBSH $FF "$structName already found in convertHelper define statement in c file $convertLoc. Nothing added. Program will continue"
	else
		insertTxtInFile "$stringToAdd" -l "$lineNumBadArgs" "$convertLoc"
		myPerrorSh $? $F_DBSH $FF "WARNING: insertTxtInFile for $convertLoc for adding to convertHelper in c file returned an error value (!=0). Code may not have been added to the file. Program will continue."
	fi
	#------------------------------------------------------------------------------------------

	# Convert all functions--------------------------------------------------------------------
	lineNumBackupDefines="$(grep -n "// BACKUP OF DEFINES" "$convertLoc" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNumBackupDefines")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var lineNumBackupDefines (value: $lineNumBackupDefines) is not a valid int. Returning -1"
		return -1
	fi
	lineNumBackupDefinesAdjusted=$(expr $lineNumBackupDefines - 4)

	stringToAdd="\n// BEGIN $structName\n$(genCodeConvertFromCsv)\n$(genCodeConvertToCsv)\n// END $structName\n"
	insertTxtInFile "$stringToAdd" -l "$lineNumBackupDefinesAdjusted" "$convertLoc"
	myPerrorSh $? $F_DBSH $FF "WARNING: insertTxtInFile for $convertLoc for adding to all functions (csvToDb, dbToCsv, etc.) returned an error value (!=0). Code may not have been added to the file. Program will continue."
	#------------------------------------------------------------------------------------------

	# Conditional in convert from str function-------------------------------------------------
	stringToAdd1="	else if (strcmp(dataTypeToConvertTo, \"$structName\") == 0){"
	stringToAdd2="		globalReturn${structName}Convert = convertFromCsvToDbObjArr_${dbObjNum}(strIn);"
	stringToAdd3="		globalTypeConvert = $nextReturnTypeNum;"
	stringToAdd4="		return globalReturn${structName}Convert;"
	stringToAdd5="	}"

	stringToAdd="\n$stringToAdd1\n$stringToAdd2\n$stringToAdd3\n$stringToAdd4\n$stringToAdd5\n"
	if [ ! "$(grep "return globalReturn${structName}Convert;" "$convertLoc")" == "" ]; then
		myPerrorSh $F_DBSH $FF "$structName already found in conditional statement in function convertFromStr() in c file $convertLoc. Nothing added. Program will continue"
	else
		insertTxtInFile "$stringToAdd" -p "\\/\\/ Invalid data type" -l -b "$convertLoc"
		myPerrorSh $? $F_DBSH $FF "WARNING: insertTxtInFile for $convertLoc for adding in conditional to convertFromStr() function returned an error value (!=0). Code may not have been added to the file. Program will continue."
	fi
	#------------------------------------------------------------------------------------------

	# Add to header file defines---------------------------------------------------------------
	local headerLoc="${incLoc}convert.h"
	if [ ! -f "$headerLoc" ]; then
		myPerrorSh $F_DBSH $FF "Header file $headerLoc for $convertLoc was not found. Nothing added. Program will continue"
	fi

	# Adding to convert global return type- - - - - - - - - - - - - - - - - - - - - - - - - - -
	lineNumWhile="$(grep -n "} while(0);" "$headerLoc" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNumWhile")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var lineNumWhile for header file $headerLoc (value: $lineNumWhile) is not a valid int. Returning -1"
		return -1
	fi

	stringToAdd="		else if (globalTypeConvert == $nextReturnTypeNum){ returnData = (struct $structName**) globalReturn${structName}Convert;} \\\\"
	if [ ! "$(echo "$stringToAdd" | sed 's/\\//g' | grep -F -f - "$headerLoc")" == "" ]; then
		myPerrorSh $F_DBSH $FF "$structName already found in convert define statement in header file $headerLoc. Nothing added. Program will continue"
	else
		insertTxtInFile "$stringToAdd" -l "$lineNumWhile" "$headerLoc"
		myPerrorSh $? $F_DBSH $FF "WARNING: insertTxtInFile for $headerLoc for adding defines returned an error value (!=0). Code may not have been added to the file. Program will continue."
	fi
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	# Adding to convertHelper- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	lineNumBadArgs="$(grep -n "default: badArgPos1" "$headerLoc" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNumBadArgs")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var lineNumBadArgs (value: $lineNumBadArgs) is not a valid int. Returning -1"
		return -1
	fi

	stringToAdd="		struct $structName**: convertFromDbObjArr_${dbObjNum}ToCsv, \\\\"
	if [ ! "$(echo "$stringToAdd" | sed 's/\\//g' | grep -F -f - "$headerLoc")" == "" ]; then
		myPerrorSh $F_DBSH $FF "$structName already found in convertHelper define statement in header file $headerLoc. Nothing added. Program will continue"
	else
		insertTxtInFile "$stringToAdd" -l "$lineNumBadArgs" "$headerLoc"
		myPerrorSh $? $F_DBSH $FF "WARNING: insertTxtInFile for $headerLoc for adding to convertHelper returned an error value (!=0). Code may not have been added to the file. Program will continue."
	fi
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	#------------------------------------------------------------------------------------------

	return 0
}

