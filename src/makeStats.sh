#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
accFileExts=("c" "h" "sh" "mk" "py" "cpp" "java" "pl" "rb")
fileExtsUsed=()
totalLineCountByFileType=()
totalLCByFileTypeNoWhitespace=()
totalLCByFileTypeNoWhitespaceNoComs=() 
totalNumWordsByFileType=() 
totalNumCharsByFileType=()

totalNumFiles=""
totalLC=0
totalLCNoWhiteSpace=0
totalLCNoWhiteSpaceNoComs=0
totalNumWords=0
totalNumChars=0
#--------------------------------------------------------------------------------------------------

# This function gets the values for the global vars
#Args: none
gettingValsByFiles(){
	for accFileExt in ${accFileExts[@]}; do #for every accepted file extension (representing a valid code file)
		allFilesLines="$(find "$myScriptsVar" -type f -name "*.$accFileExt" -exec cat {} +)" #open all file with that ext and count the num of lines
		if [ ! "$allFilesLines" == "" ]; then #if at least one file exists with that ext
			fileExtsUsed+=("$accFileExt") #append to active file ext var
			totalLineCountByFileType+=("$(echo "$allFilesLines" | wc -l)") #count num lines from all files with that ext
			#regex: remove all lines containing only 0 or more instances of whitespace. Count new num of lines
			totalLCByFileTypeNoWhitespace+=("$(echo "$allFilesLines" | grep -v -E "^\s*$" | wc -l)")

			#--------------------------------------------------------------------------
			# Line nums excluding whitespace AND all comments (single-line & block comments)
			#finding single line comments
			totalLCByFileTypeNoWhitespaceNoComs1="" 
			if [[ "$accFileExt" =~ ^c$|^h$|^java$|^cpp$ ]]; then #if a coding language using "//" for comments
				#regex: remove all lines containing 0 or more instances of whitespace or containing ONLY 0 or more instances of whitespace followed by "//"
				totalLCByFileTypeNoWhitespaceNoComs1="$(echo "$allFilesLines" | grep -v -E "(^\s*$)|(^\s*(//))" | wc -l)"
			else #if not a coding language using "//", assume it uses "#"
				#regex: remove all lines containing 0 or more instances of whitespace or containing ONLY 0 or more instances of whitespace followed by "#"
				totalLCByFileTypeNoWhitespaceNoComs1="$(echo "$allFilesLines" | grep -v -E "(^\s*$)|(^\s*(#))" | wc -l)"
			fi
			
			#finding block comments (only for languages using "/* */")	
			totalLCByFileTypeNoWhitespaceNoComs2=""
			if [[ "$accFileExt" =~ ^c$|^h$|^java$|^cpp$ ]]; then
				#counting the number of lines between /**/ (inclusive) in all files
				totalLCByFileTypeNoWhitespaceNoComs2="$(echo "$allFilesLines" | sed -n '/\/\*/,/\*\//p' | wc -l)"
			else
				totalLCByFileTypeNoWhitespaceNoComs2="0" #if not a language using "/* */", set this val to 0
			fi
		
			#subtract num lines with "/* */" from total lines without singl-line comments
			totalLCByFileTypeNoWhitespaceNoComs+=("$(expr $totalLCByFileTypeNoWhitespaceNoComs1 - $totalLCByFileTypeNoWhitespaceNoComs2)")
			#--------------------------------------------------------------------------

			# Calculating num words and num chars
			totalNumWordsByFileType+=("$(echo "$allFilesLines" | wc -w)")
			totalNumCharsByFileType+=("$(echo "$allFilesLines" | wc -c)")
		fi
	done
}

# This function converts the values found for each file type (assuming more than 1 file with the extension) into a sum for that parameter
#Args: none
gettingValsTotals(){
	i=0
	# iterate over every file type in totalLineCountByFileType array
	for num in ${totalLineCountByFileType[@]}; do
		totalLC=$(expr $num + $totalLC)
		totalLCNoWhiteSpace=$(expr ${totalLCByFileTypeNoWhitespace[$i]} + $totalLCNoWhiteSpace)
		totalLCNoWhiteSpaceNoComs=$(expr $totalLCNoWhiteSpaceNoComs + ${totalLCByFileTypeNoWhitespaceNoComs[$i]})
		totalNumWords=$(expr $totalNumWords + ${totalNumWordsByFileType[$i]})
		totalNumChars=$(expr $totalNumChars + ${totalNumCharsByFileType[$i]})
		i=$(expr $i + 1)
	done
}

# This function prints the key values for the repo
#Args: none
printKeyVals(){
	echo "==================================================================================================="
	echo "KEY REPOSITORY VALUES:"
	echo "==================================================================================================="
	echo -ne "Total Number Of Lines Of Code\u00B9:" #unicode value for superscript 1
	echo "	$(perl -pe 's/(?<=\d)(?=(\d{3})+(?!\d))/,/g' <<< $totalLC)"

	echo -ne "Total Number Of Files:"
	totalNumFiles="$(find $myScriptsVar -type f | wc -l)"
	echo "		$totalNumFiles"

	echo -ne "Total Number Of Words:"
	#regex: converting into format with commas after every 3 numbers
	echo "		$(perl -pe 's/(?<=\d)(?=(\d{3})+(?!\d))/,/g' <<< $totalNumWords)"

	echo -ne "Total Number Of Characters:"
	#regex: converting into format with commas after every 3 numbers
	echo "	$(perl -pe 's/(?<=\d)(?=(\d{3})+(?!\d))/,/g' <<< $totalNumChars)"

	echo ""
	echo -n "1: the total number of lines of code is defined by the number of lines in files containing any of the follow extensions: "
	#regex: replacing space with ", ." (so each file name is separated by comma and preceded by '.'). Adding '.' before first element
	echo "$(echo ${accFileExts[@]} | sed 's/ /, \./g' | sed 's/^/\./g')"
	echo "==================================================================================================="
	echo ""
	echo ""
}

# This function prints the more advanced values for the repo
#Args: none
printAdvVals(){
	echo "==================================================================================================="
	echo "ADVANCED REPOSITORY VALUES:"
	echo "==================================================================================================="
	echo "FILE BREAKDOWN-------------------------------------------------------------------------------------"
	echo -en "Total Number Of Code Files\u00B2:" #unicode value for subscript 2
	#converting file extensions string array into a regex for sed
	#regex: replacing ' ' with "\|\.\". Adding "\." to before first element of string
	accFileExtAdj="$(echo "${accFileExts[@]}" | sed 's/ /\\|\\\.\\/g' | sed 's/^/\\\./g')"
	#finding all files ending in regex found above
	numCodeFiles="$(find $myScriptsVar -type f | sed -n "/$accFileExtAdj/p" | wc -l)"
	echo -n "			$numCodeFiles ("
	#finding percentage and formatting it to 2 decimal places
	echo "$(echo | awk "{ printf \"%.2f%%\", $numCodeFiles / $totalNumFiles * 100 }"))"

	echo -en "Total Number Of Csv, Txt, Md, Man, and Yaml Files:"
	#finding all files ending in .csv, .txt, etc
	numTxtFiles="$(find $myScriptsVar -type f | sed -n "/\.csv\|\.txt\|\.md\|\.man\|\.yaml\|\.yml\|\.1\|\.2\|\.3\|\.5/p" | wc -l)"
	echo -n "	$numTxtFiles ("
	#finding percentage and formatting it to 2 decimal places
	echo "$(echo | awk "{ printf \"%.2f%%\", $numTxtFiles / $totalNumFiles * 100 }"))"
	echo "---------------------------------------------------------------------------------------------------"
	echo ""

	echo "LINES OF CODE BREAKDOWN----------------------------------------------------------------------------"
	echo -ne "Total Number Of Lines Of Code Ignoring Whitespace\u00B3:"
	#regex: converting into format with commas after every 3 numbers
	echo "		$(perl -pe 's/(?<=\d)(?=(\d{3})+(?!\d))/,/g' <<< $totalLCNoWhiteSpace)"

	echo -ne "Total Number Of Lines Of Code Ignoring Whitespace & Comments\u2074:"
	#regex: converting into format with commas after every 3 numbers
	echo "	$(perl -pe 's/(?<=\d)(?=(\d{3})+(?!\d))/,/g' <<< $totalLCNoWhiteSpaceNoComs)"
	echo "---------------------------------------------------------------------------------------------------"

	echo ""
	echo -n "2: the total number of code files is defined as files containing any of the follow extensions: "
	#converting string array into a comma-seperated string and each element beginning with '.'
	#regex: replacing ' ' with ', .'. Prepending '.' before string
	echo "$(echo ${accFileExts[@]} | sed 's/ /, \./g' | sed 's/^/\./g')"

	echo "3: the total number of lines of code ignoring whitespace removes empty lines or lines only with whitespace in the files mentioned in (1)"
	echo "4: the total number of lines of code ignoring whitespace and comments performs the same whitespace removal as (2) and also removes lines containing ONLY comments and block comments."
	echo "	4a: removal of lines of code are defined as lines beginning with \"//\" or \"#\", optionally preceeded by exclusively whitespace"
	echo "	4b: removal of block comments is only accounted for in files that use \"/* */\" style block comments (.c, .h, .cpp, .java). Block comments in other languages are not accounted for, but do not exist in this repo, so the value is still accurate"
	echo "==================================================================================================="
	echo ""
	echo ""
}

# This function prints advanced values table broken down by active file type
#Args: none
printAdvValsByFile(){
	echo "==================================================================================================="
	echo "ADVANCED BREAKDOWN BY FILE TYPE - $1:"
	echo "==================================================================================================="

	# Creating Header Row----------------------------------------------------------------------
	echo -n "Parameter			|"
	i=0
	lenOfHeaderRow="$(echo "Parameter                       |" | wc -c)" #used spaces, not tabs, to get accurate num spaces
	for lc in ${totalLineCountByFileType[@]}; do #for every active file ext, print file ext with buffer spaces before and after
		printVar="    ${accFileExts[$i]}    |"
		echo -n "$printVar"
		lenOfHeaderRow=$(expr $lenOfHeaderRow + "$(echo "$printVar" | wc -c)")
		i=$(expr $i + 1)
	done
	echo ""

	for ((i=0; i < $lenOfHeaderRow-6; i++)); do echo -n "-"; done #add line below header row
	echo ""
	#------------------------------------------------------------------------------------------

	# List of params, written with correct tabs, to be added to the start of each line
	paramsList=("Number Of Files			|" "Total Line Count		|" "Total Line Count No Whitespaces	|" "Total Line Count No Comments	|" "Total Number Of Words		|" "Total Number Of Chars		|")
	totalsArr=("$totalNumFiles" "$totalLC" "$totalLCNoWhiteSpace" "$totalLCNoWhiteSpaceNoComs" "$totalNumWords" "$totalNumChars") #all the sum values for each parameter

	ii=0
	for param in "${paramsList[@]}"; do #for every parameter
		echo -n "$param"
		i=0
		for lc in ${totalLineCountByFileType[@]}; do #for every active file ext
			# find its value based on current parameter
			value=""
			if [ $ii -eq 0 ]; then value="$(find "$myScriptsVar" -type f -name "*.${accFileExts[$i]}" | wc -l)";
			elif [ $ii -eq 1 ]; then value=${totalLineCountByFileType[$i]};
			elif [ $ii -eq 2 ]; then value=${totalLCByFileTypeNoWhitespace[$i]};
			elif [ $ii -eq 3 ]; then value=${totalLCByFileTypeNoWhitespaceNoComs[$i]};
			elif [ $ii -eq 4 ]; then value=${totalNumWordsByFileType[$i]};
			elif [ $ii -eq 5 ]; then value=${totalNumCharsByFileType[$i]};
			fi

			#if getting vals, not percentages, convert to num with commas every 3 digits. Else format as decimal to 2 palces
			if [ "$1" == "vals" ]; then value="$(perl -pe 's/(?<=\d)(?=(\d{3})+(?!\d))/,/g' <<< $value)";
			else value="$(echo | awk "{ printf \"%.2f%%\", $value / ${totalsArr[$ii]} * 100 }")";
			fi

			numDigits="$(echo $value | wc -c)" #num digits in value
			sizeOfCol="$(expr 8 + $(echo "${accFileExts[$i]}" | wc -c))" #size of col. 8 is space buffers - 4 each sied
			numSpacesLeftTemp=$(expr $sizeOfCol - $numDigits - 1)
			numSpacesLeft="$(expr $numSpacesLeftTemp / 2)"
			numSpacesRight="$(expr $sizeOfCol - $numSpacesLeft - $numDigits)"
			
			# Formatting in table
			for ((j = 0; j < $numSpacesLeft; j++)); do echo -n " "; done
			echo -n "$value"
			for ((j = 0; j < $numSpacesRight; j++)); do echo -n " "; done
			echo -n "|"
			
			i=$(expr $i + 1)
		done
		echo ""
		ii=$(expr $ii + 1)
	done
	echo "==================================================================================================="
	echo ""
	echo ""
}

# Help Menu
helpMenuMakeStats(){
	echo "+++MAKE STATS - HELP MENU+++	Relative File Path: $0"
	echo "Purpose: create stats about repo and write them to repoStats.txt" #HARDCODED
	echo "Options: "
	echo "	none"
	echo "Usage: "
	echo "	<binNameOrAlias>"
}

# Main function
main(){
	if [ ! "$#" -eq 0 ]; then helpMenuMakeStats; return -1; fi

	echo "This file is automatically updated everytime it is pushed. Values are up-to-date"
	echo ""
	echo ""

	gettingValsByFiles
	gettingValsTotals
	printKeyVals
	printAdvVals
	printAdvValsByFile "vals"
	printAdvValsByFile "percent"
}

# Writing to file
if [ ! -f "${myScriptsVar}repoStats.txt" ]; then touch "${myScriptsVar}repoStats.txt"; fi #HARDCODED
truncate -s 0 "${myScriptsVar}repoStats.txt" #HARDCODED
main "$@" >> "${myScriptsVar}repoStats.txt" #HARDCODED
