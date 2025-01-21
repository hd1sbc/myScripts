#!/bin/bash

# This function adds code to define statements in both the header (.h) file and .c file (under BACKUP OF DEFINES)
#Args: 1 - file path; 2 - num pointers; 3 - function name
addCodeToDefines(){
	# Vars
	local FF="${FUNCNAME[0]}"
	structNameWithPointers=""
	structNameForGrep=""

	# Data Validation
	if [ ! $# -eq 3 ]; then
		myPerrorSh $F_DBSH $FF "Invalid num of args. 3 are expected, $# were provided. Returning -1"
		return -1
	fi
	if [ ! -f "$1" ]; then
		myPerrorSh $F_DBSH $FF "Invalid 1st arg (file path); file $1 doesn't exit. Returning -1"
		return -1
	fi

	# Getting Values---------------------------------------------------------------------------
	if [ "$2" == "0" ]; then
		structNameWithPointers="struct $structName:"
	elif [ "$2" == "1" ]; then
		structNameWithPointers="struct $structName*:"
	elif [ "$2" == "2" ]; then
		structNameWithPointers="struct $structName**:"
	else
		myPerrorSh $F_DBSH $FF "Invalid 2nd arg (num pointers); value $2 is not accepted. Accepted values are 0, 1, 2. Error for file input $1. Returning -1"
		return -1

	fi

	structNameForGrep="$(echo "$structNameWithPointers" | sed 's/\*/\\\*/g')" #replacing '*' with '\*" so it can be used with grep
	#------------------------------------------------------------------------------------------

	# Adding define code to .c file under "BACKUP OF DEFINES" section
	if [ "$(grep "$structNameForGrep" "$1")" == "" ]; then
		lineNumDefault="$(grep -n "default:" "$1" | tail -n 1 | cut -d ':' -f 1)"
		if [ ! "$(isIntSh "$lineNumDefault")" == "1" ]; then
			myPerrorSh $F_DBSH $FF "Line num $lineNumDefault for .c file is not a valid integer. Returning -1"
			return -1
		fi
		
		#using perl cuz sed doesn't include leading or trailing whitespace
		#regex: if current line = line num of default, insert the text at that line and move all subsequent lines down
		perl -i -pe "if ($. == $lineNumDefault) { print \"		$structNameWithPointers $3, \\\\\\n\"; }" "$1"
		myPerrorSh $? $F_DBSH $FF "WARNING: Issue with running perl command for .c file $1. Program will continue"
	else
		myPerrorSh $F_DBSH $FF "Code for define statement in .c file $1 already found. Nothing added. Program will continue"
	fi

	# Adding define code to header (.h) file
	local baseName="$(getFileNameNoExtSh "$1")" #finding basename
	local headerLoc="${incLoc}${baseName}.h" #finding header location based off of basename

	if [ ! -f "$headerLoc" ]; then
		myPerrorSh $F_DBSH $FF "Header file $headerLoc not found for $1. Returning -1"
		return -1
	fi

	if [ "$(grep "$structNameForGrep" "$headerLoc")" == "" ]; then
		lineNumDefault="$(grep -n "default:" "$headerLoc" | tail -n 1 | cut -d ':' -f 1)"
		if [ ! "$(isIntSh "$lineNumDefault")" == "1" ]; then
			myPerrorSh $F_DBSH $FF "Line num $lineNumDefault for .c file is not a valid integer. Returning -1"
			return -1
		fi
		
		#using perl cuz sed doesn't include leading or trailing whitespace
		#regex: if current line = line num of default, insert the text at that line and move all subsequent lines down
		perl -i -pe "if ($. == $lineNumDefault) { print \"		$structNameWithPointers $3, \\\\\\n\"; }" "$headerLoc"
		myPerrorSh $? $F_DBSH $FF "WARNING: Issue with running perl command for header file $1. Program will continue"
	else
		myPerrorSh $F_DBSH $FF "Code for define statement in .h file $1 already found. Nothing added. Program will continue"
	fi

	return 0
}

# This function combines the function above to add the auto-generated code and define statements to the file
#Args: 1 - file path; 2 - function code string; 3 - function name; 4 - num pointers
addCodeToFiles(){
	# Vars
	local FF="${FUNCNAME[0]}"
	returnVal1=0
	returnVal2=0

	# Data Validation
	if [ ! -f "$1" ]; then myPerrorSh $F_DBSH $FF "Invalid arg 1; file $1 doesn't exist. Returning -1"; return -1; fi
	if [ ! "$(grep "$3" "$1")" == "" ]; then #checking if function already exists
		myPerrorSh $F_DBSH $FF "Function $functionName already exists in $1. Nothing added to file. Returning -1"
		myPerrorSh $F_DBSH $FF "	Note that the function might have been removed, but not in the define statement"
		return -1
	fi

	# Getting line num to add code to back up of defines in .c file
	lineNumBackupDefines="$(grep -n "// BACKUP OF DEFINES" "$1" | head -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNumBackupDefines")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Var lineNumBackupDefines (value: $lineNumDefault) for file $1 is not a valid integer. Returning -1"
		return -1
	fi
	lineNumAdjusted=$(expr "$lineNumBackupDefines" - 5)

	# Adding to the .c file
	insertTxtInFile "$2" -l "$lineNumAdjusted" "$1"
	returnVal1=$?
	myPerrorSh $returnVal1 $F_DBSH $FF "WARNING: insertTxtInFile for file $1 for adding to backup of defines in .c file returned an error value (!=0). Code may not have been added to the file. Program will continue."

	# Adding to defines in .h file and "BACKUP OF DEFINES" section in .c file
	addCodeToDefines "$1" "$4" "$3"
	returnVal2=$?
	myPerrorSh $returnVal2 $F_DBSH $FF "WARNING: addCodeToDefines() for file $1 returned an error value (!=0). Code may not have been added to the file. Program will continue."

	# If one of the return vals doesn't equal 0, return that
	if [ ! $returnVal1 -eq 0 ]; then
		return $returnVal1
	elif [ ! $returnVal2 -eq 0 ]; then
		return $returnVal2
	else
		return 0
	fi

}
