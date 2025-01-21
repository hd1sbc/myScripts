#!/bin/bash

# This function checks all of the sourced file locations
# This only looks for variables above the line in the file where this function is called & the var name ending with "Loc" or "loc"
#Args: 1 - file name (of the file calling this func. Usually stored in the F_SOMETHING variable)
checkFileLocs(){
	# Vars. Keeping local so it doesn't conflict with vars in the file where it is called
	local currLineNum=${BASH_LINENO[0]} #getting the line num in the file where this function is called
	local returnVal=0 #keeping track if 1 or more files dne

	# Getting the list of file location variables
	#regex: get all the lines from the top of file to the line num where this func was called. Find all lines with "loc=" or "Loc=". Remove everything after (and including) "="
	local fileLocs1="$(head -n $currLineNum  $1 | sed -n '/\(loc=\|Loc=\)/p' | sed 's/=.*//g')"
	local fileLocs2="$(head -n $currLineNum  $1 | sed -n '/\(dir=\|Dir=\)/p' | sed 's/=.*//g')"
	local fileLocs="$(echo -e "$fileLocs1\n$fileLocs2")"

	# Iterating over every file location in the file locations string	
	for fileLocStr in $fileLocs; do
		local fileLoc="${!fileLocStr}" #changing the variable name in string form to an actual variable

		if [ -z $fileLoc ]; then #if file loc is empty
			echo "===$1===	File stored in variable $fileLocStr is empty. Exiting" >&2
			returnVal=1
		elif [ ! -e $fileLoc ] || [ $fileLoc == "/dev/null" ]; then #if file loc dne
			echo "===$1===	File stored in variable $fileLocStr either doesn't exist or points to /dev/null. Exiting" >&2
			returnVal=1
		elif [[ $fileLoc =~ ".sl$" ]] && [ "$(readlink $fileLoc)" == "/dev/null" ]; then #if file loc at symlink dne
			echo "===$1===	File stored in variable $fileLocStr either doesn't exist or points to /dev/null. Exiting" >&2
			returnVal=1
		fi
	done

	# If 1 or more file dne, then exit this function and the script the function was called in
	if [ ! $returnVal -eq 0 ]; then
		exit $returnVal
	fi
}
