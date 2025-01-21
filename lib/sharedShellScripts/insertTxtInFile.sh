#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
myPerrorShLoc="$(findPath -f 341)" #myPerrorSh.sh
regexShLoc="$(findPath -f 312)" #regexSh.sh

lineNumInsertText=-1
lineNumsInsertText=""
patternInsertText=""
patternInstanceInsertText="none"
fileNameInsertText=""
stringToAddInsertText=""
afterFlagInsertText=0

# Error Msg Constant Vars
F_INSERTTXT="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# SOURCING-----------------------------------------------------------------------------------------
source "$myPerrorShLoc"
source "$regexShLoc"
#--------------------------------------------------------------------------------------------------

# This function finds the line num(s) of a pattern within a file
#Args: none
findLineNum(){
	# Vars
	local FF="${FUNCNAME[0]}"

	patternInsertTextGrepAdj="$(echo "$patternInsertText" | sed 's/\*/\\\*/g' | sed 's/\./\\\./g')"
	if [ "$patternInstanceInsertText" == "-f" ]; then #1st instance
		lineNumInsertText="$(grep -n "$patternInsertTextGrepAdj" "$fileNameInsertText" | head -n 1 | cut -d ':' -f 1)"
	elif [ "$patternInstanceInsertText" == "-l" ]; then #last instance
		lineNumInsertText="$(grep -n "$patternInsertTextGrepAdj" "$fileNameInsertText" | tail -n 1 | cut -d ':' -f 1)"
	elif [ "$patternInstanceInsertText" == "-a" ]; then #all instances
		lineNumsInsertText="$(grep -n "$patternInsertTextGrepAdj" "$fileNameInsertText" | cut -d ':' -f 1)"
		lineNumInsertText="1" #setting as non-zero positive val to not trigger error
	fi

	# Checking result of lineNumInsertText
	if [ "$lineNumInsertText" == "" ]; then myPerrorSh $F_INSERTTXT $FF "No pattern found in file. Returning -1"; return -1; fi
	if [ "$(isIntSh "$lineNumInsertText")" == "0" ] || [ ! $((lineNumInsertText)) -gt 0 ]; then
		myPerrorSh $F_INSERTTXT $FF "Error in finding line number of pattern. Returning -1"
		return -1
	fi
	
	lineNumInsertText=$(expr "$lineNumInsertText" - 1) #subtracting 1 cuz the default is to insert "above" the line num
	return 0
}

# This function inserts a string into a file at given line num(s)
#Args: none
insertString(){
	if [ "$lineNumsInsertText" == "" ]; then #if only 1 line num (lineNumsInsertText is empty)
		# check if "after line num" flag is set. If so, add 1 to line num to add it after line num
		if [ $afterFlagInsertText -eq 1 ]; then lineNumInsertText=$(expr "$lineNumInsertText" + 1); fi

		beforeLineNum="$(head -n $lineNumInsertText "$fileNameInsertText")" #get file contents before (and/or including) line num
		afterLineNum="$(tail -n +$(expr $lineNumInsertText + 1) "$fileNameInsertText")" #get file contents after line num

		# Inserting string into file
		truncate -s 0 "$fileNameInsertText" #clear file
		echo "$beforeLineNum" >> "$fileNameInsertText"
		echo -e "$stringToAddInsertText" >> "$fileNameInsertText"
		echo "$afterLineNum" >> "$fileNameInsertText"
	else #if more than 1 line of matching patterns
		# Separate each line num and complete the same steps as in the conditional above
		lineNumAdjuster=0 #since we are changing multiple lines, the original line numbers will be different as we add new line. This keeps track of the additional lines added so the text will be added at the original place
		lineNumsInsertText="$(echo "$lineNumsInsertText" | tr '\n' ',')"
		origIfs=$IFS
		IFS=','
		for lineNumInsertTextTemp in $lineNumsInsertText; do
			# check if "after line num" flag is set. If so, add 1 to line num to add it after line num
			if [ $afterFlagInsertText -eq 1 ]; then lineNumInsertTextTemp=$(expr "$lineNumInsertTextTemp" + 1); fi
			
			lineNumInsertTextTemp=$(expr $lineNumInsertTextTemp + $lineNumAdjuster) #adjusting for added lines

			beforeLineNum="$(head -n $lineNumInsertTextTemp "$fileNameInsertText")" #get file contents before (and/or including) line num
			afterLineNum="$(tail -n +$(expr $lineNumInsertTextTemp + 1) "$fileNameInsertText")" #get file contents after line num

			# Inserting string into file
			truncate -s 0 "$fileNameInsertText" #clear file
			echo "$beforeLineNum" >> "$fileNameInsertText"
			echo -e "$stringToAddInsertText" >> "$fileNameInsertText"
			echo "$afterLineNum" >> "$fileNameInsertText"

			lineNumAdjuster=$(expr $lineNumAdjuster + "$(echo $stringToAddInsertText | wc -l)") #incrementing adjusted lines var by number of lines in string to be added
		done
		IFS=$origIfs
	fi

	return 0
}

# This function checks for valid inputs
#Args: all (from effective main function (insertTxtInFile))
dataValidationInsertText(){
	# Vars
	local FF="${FUNCNAME[0]}"

	if [ "$1" == "" ]; then myPerrorSh $F_INSERTTXT $FF "Invalid arg 1 (string to add); value empty. Returning -1"; return -1; fi

	# If "-l" option
	if [ "$2" == "-l" ]; then
		if [ ! $# -eq 4 ]; then #check num args (should be 4 instead of 6)
			myPerrorSh $F_INSERTTXT $FF "Invalid number of args; $# were provided and 4 args are required when using \"-l\" option. Returning -1"
			return -1
		fi

		arg3="$3"
		if [ "$(isIntSh "$arg3")" == "0" ] || [ ! $((arg3)) -gt 0 ]; then #check arg 3 is a valid num
			myPerrorSh $F_INSERTTXT $FF "Invalid arg 3; value is not a valid number or is less than 1. Returning -1"
			return -1
		fi
		
		if [ ! -f "$4" ]; then #check file path is valid
			myPerrorSh $F_INSERTTXT $FF "Invalid arg 4; file path doesn't exist. Returning -1"
			return -1
		fi
	# If "-p" option
	elif [ "$2" == "-p" ]; then 
		if [ "$3" == "" ]; then #checking pattern string
			myPerrorSh $F_INSERTTXT $FF "Invalid arg 3 (pattern string); value is empty. Returning -1"
			return -1
		fi

		if [ ! "$4" == "-f" ] && [ ! "$4" == "-l" ] && [ ! "$4" == "-a" ]; then #checking flags
			myPerrorSh $F_INSERTTXT $FF "Invalid arg 4; value is not an accepted flag. Please use -h or --help for info. Returning -1"
			return -1;
		fi

		if [ ! "$5" == "-a" ] && [ ! "$5" == "-b" ]; then #checking flags
			myPerrorSh $F_INSERTTXT $FF "Invalid arg 4; value is not an accepted flag. Please use -h or --help for info. Returning -1"
			return -1
		fi

		if [ ! -f "$6" ]; then #checking file path is valid
			myPerrorSh $F_INSERTTXT $FF "Invalid arg 6; file path doesn't exist. Returning -1"
			return -1
		fi

	else
		myPerrorSh $F_INSERTTXT $FF "Invalid arg 2; value is not an accepted flag. Please use -h or --help for info. Returning -1"
		return -1;
	fi

	return 0
}

# Help Menu
#Args: none
helpMenuInsertTxtInFile(){
	echo "+++INSERT TEXT IN FILE AT SPECIFIC LINE+++	Relative Path: $0"
	echo "Purpose: insert a string into a file at a specific location"
	echo "Details: insert a single or multi-line string into a file at a given line number or at the line number of a pattern"
	echo "Options: "
	echo "	-l <lineNum>		:- insert at line num, shifting subsequent lines (including previous text at line num) up by 1"
	echo "	-p <pattern>		:- insert below the occurrence of the pattern"
	echo "		-f		:- insert at the 1st occurrence of the pattern"
	echo "		-l		:- insert at the last occurrence of the pattern"
	echo "		-a		:- insert for every occurrence of the pattern"
	echo "			-b	:- insert at line num of pattern, shifting subsequent lines (including previous text at line num) up by 1"
	echo "			-a	:- insert at line num of pattern, shifting subsequent lines (excluding previous text at line num) up by 1"
	echo "Usage: "
	echo "	<binNameOrAlias> <stringToAdd> < -l> <lineNum> <filePath>					:- will add string at specific line num in file"
	echo "	<binNameOrAlias> <stringToAdd> < -p> <patternString> < -f | -l | -a> < -b | -a>	<filePath>	:- will ad string at line number(s) of matching pattern before or after each occurrence"
	echo "Examples: "
	echo "	<binNameOrAlias> \"abcd\\nefg\" -p \"hij\" -f -b \"someFilePath\"	:- will insert \"abcd\\nefg at the line num of the 1st occurrence \"hij\" and shift all subsequent lines (including line num of \"hij\") up one"
	echo "	<binNameOrAlias> \"abcd\\nefg\" -l 10 \"someFilePath\" 			:- will insert \"abcd\\nefg\" at line 10 and shift all subsequent lines (including line num of \"hij\") up one"
}

# This function resets the global values
resetValsInsertTxtInFile(){
	lineNumInsertText=-1
	lineNumsInsertText=""
	patternInsertText=""
	patternInstanceInsertText="none"
	fileNameInsertText=""
	stringToAddInsertText=""
	afterFlagInsertText=0
}

# Effective "main" function
#Args: string to add, -l lineNum || -p pattern -f (1st instance) || -l (last instance) || -a (all instances) -b || -a, file name
insertTxtInFile(){
	# Vars
	local FF="${FUNCNAME[0]}"

	# Help Menu
	if [ "$(isHelpSh "$1")" == "1" ]; then helpMenuInsertTxtInFile; return 1; fi

	# Data Validation--------------------------------------------------------------------------
	if [ $# -eq 4 ] || [ $# -eq 6 ]; then
		dataValidationInsertText "$@"
		if [ ! $? -eq 0 ]; then 
			myPerrorSh $F_INSERTTXT $FF "dataValidationInsertText() function returned an error value (!=0). Returning -1"
			return -1;
		fi
	else
		myPerrorSh $F_INSERTTXT $FF "Invalid number of args; $# were provided and 4 args are required when using options other than \"-l\" option. Returning -1"
		return -1
	fi
	#------------------------------------------------------------------------------------------

	# Getting Values
	stringToAddInsertText="$1"	
	if [ $# -eq 4 ]; then
		arg3="$3"
		lineNumInsertText=$(expr "$3" - 1) #subtracting 1 cuz default is to add "above" line num
		fileNameInsertText="$4"
	else
		patternInsertText="$3"
		patternInstanceInsertText="$4"
		if [ "$5" == "-a" ]; then afterFlagInsertText=1; fi #checking for "after line num" flag
		fileNameInsertText="$6"
		findLineNum
		if [ ! $? -eq 0 ]; then
			myPerrorSh $F_INSERTTXT $FF "findLineNum() function returned an error value (!=0). Returning -1"
			return -1
		fi
	fi

	insertString
	returnVal=$?
	resetValsInsertTxtInFile
	return $returnVal
}
