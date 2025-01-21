#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
envVarsLoc="$(findPath -f 77)" #envVars.csv
compiledHeadersLoc="$(findPath -f 11)/" #compiledHeaders dir
incLoc="$(findPath -f 120)/" #include dir
csvDir="$(findPath -f 76)/" #csv dir

fieldValEnvVarsColNum=2 #HARDCODED_COL_NUM

# Error Msg Constant Vars
F_CHECKSPECVALS="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# SOURCING-----------------------------------------------------------------------------------------
source "$sharedScripts"
checkFileLocs $F_CHECKSPECVALS
#--------------------------------------------------------------------------------------------------

# This function checks if there are any .o, .so, or .h files with unassociated .c and/or .mk files
#Args: none
checkForUnAssociatedFiles(){
	# Vars
	local FF="${FUNCNAME[0]}"
	listOfObjFiles=""
	listOfHeaderFiles=""

	# Checking for unassociated object files
	echo "The following object files in $compiledHeadersLoc were found without an associated .c, .h, or .mk:"
	listOfObjFiles="$(find "$compiledHeadersLoc" -type f)" #finding all obj files
	for file in $listOfObjFiles; do
		baseName="$(getFileNameNoExtSh "$file")"
		#seeing if any files with the same basename but ending in ".c", ".h", or ".mk"
		similarFiles="$(find "$myScriptsVar" -type f -name "$baseName.c" -o -name "$baseName.h" -o -name "$baseName.mk")"
		if [ "$similarFiles" == "" ]; then echo "$file"; fi
	done

	# Checking for unassociated file headers
	echo ""
	echo "The following header files in $incLoc were found without an associated .c, .o, or .mk and were not found in the contents of any other files in $incLoc:"
	listOfHeaderFiles="$(find "$compiledHeadersLoc" -type f)"
	for file in $listOfHeaderFiles; do
		baseName="$(getFileNameNoExtSh "$file")"
		#seeing if any files with the same basename but ending in ".c", ".o", or ".mk"
		similarFiles="$(find "$myScriptsVar" -type f -name "$baseName.c" -o -name "$baseName.o" -o -name "$baseName.mk")"
		if [ "$similarFiles" == "" ]; then
			# If no associated files, check if header file is included in other header files (could be just a define header file)
			similarFiles="$(grep -rl "$file" "$incLoc")"
			if [ "$similarFiles" == "" ]; then echo "$file"; fi
		fi
	done

}

# This function checks if there are any error comments, defined in the envVars.csv
#Args: none
checkForErrorComs(){
	# Vars
	local FF="${FUNCNAME[0]}"
	listOfErrorComs=""
	fieldValEnvVarsColNumAdj=$(expr $fieldValEnvVarsColNum + 1)

	# Getting Values
	#regex: finding all lines containing "errorCom_" in envVarsLoc. Finding the value in the field value col. Replacing newlines in the string with spaces to make it a single-line string
	listOfErrorComs="$(sed -n '/errorCom_/p' "$envVarsLoc" | cut -d ',' -f "$fieldValEnvVarsColNumAdj" | tr '\n' ' ')"

	for errorCom in $listOfErrorComs; do #iterating over every error comment
		echo "The following files were found to contain the following error comment: $errorCom:"
		#regex: searching for files with at least one occurence of the current error comment in myScriptsVar, ignoring envVars.csv as well as any .swp files, object files, and shared object files
		grep -rl "$errorCom" "$myScriptsVar" | grep -v "$envVarsLoc" | grep -v -E "\.swp|\.o|\.so"
		echo ""
	done
}

# This function checks for the hardcoded keyword in any files, but ignore any hardcoded exception values
#Args: none
checkForHardcode(){
	# Vars
	local FF="${FUNCNAME[0]}"
	fieldValEnvVarsColNumAdj=$(expr $fieldValEnvVarsColNum + 1)
	hcVal=""
	hcExceptions=""

	# Getting Vals-----------------------------------------------------------------------------
	#regex: finding hardcoded row (trailing comma to distinguish from hardcoded_*). Getting value associated with field
	hcVal="$(sed -n '/hardcoded,/p' "$envVarsLoc" | cut -d ',' -f "$fieldValEnvVarsColNumAdj")"
	#regex: finding all lines with hardcoded_exception. Getting their values. Converting from multi-line string to single line. removing trailing space
	hcExceptions="$(sed -n '/hardcoded_exception/p' "$envVarsLoc" | cut -d ',' -f "$fieldValEnvVarsColNumAdj" | tr '\n' ' ' | sed 's/ $//g')"
	
	# Adjusting values for perl
	#regex: replacing beginning of line with '/'. Replacing end of line with "/i" (the i is for case insensitive)
	hcValAdjForPerl="$(echo "$hcVal" | sed 's/^/\//g' | sed 's/$/\/i/g')"
	#regex: converting single line string to multi-line. Replacing beginning of each line with "!/". Replacing end of each line with "/i" (the i is for case insensitive). Removing trailing new line
	hcExcAdjForPerl="$(echo "$hcExceptions" | tr ' ' '\n' | sed 's/^/\!\//g' | sed 's/$/\/i/g' | tr '\n' ' ' | sed 's/ $//g' | sed 's/ / \&\& /g')"
	#------------------------------------------------------------------------------------------

	# Combining the vars above in perl command
	echo "Files in $myScriptsVar containing in $hcVal, case insensitive:"
	echo "	-these files contain $hcVal without any following exception values"
	echo ""
	# For every file in myScripts, looking if it contains hcVal, but ignoring any instances of hc exceptions
	# 	If a file contains an hc exception, but not hcVal, file name will be printed
	# 	If it contains an hc val without anything after it (like an hc exception), it will print file name regardless if hc exceptions are there.
	# Sort and uniq remove duplicate prints
	# 	Perl will find all instances of this pattern in a file. If a file contains hcVal, I only want it printed once
	# Removing instances of this file or its .swp version as well as envVar.csv
	find "$myScriptsVar" -type f -exec perl -ne "if ($hcValAdjForPerl && $hcExcAdjForPerl) { print \"\$ARGV\n\"; }" {} + | sort | uniq | grep -v "$F_CHECKSPECVALS" | grep -v "$envVarsLoc" | grep -v -E "\.swp"
	
	# Same as above, but finding files containing beginning of hcVal (HAR, case sensitive) with a comment char(s) before it (//,#,/*) and not containing hcVal or any hc exceptions
	# Doing this in case of misspellings
	echo ""
	echo "Files in $myScriptsVar containing a comment char(s) (//,#,/*) followed by \"HAR\", case sensitive. Doing this in case of misspellings:"
	find "$myScriptsVar" -type f -exec perl -ne "if (m{//HAR|#HAR|/\*HAR} && !$hcValAdjForPerl && $hcExcAdjForPerl) { print \"\$ARGV\n\"; }" {} + | sort | uniq | grep -v "$F_CHECKSPECVALS" | grep -v "$envVarsLoc" | grep -v -E "\.swp"

}

# This function checks the myScripts dir (outside of csv dir) for files ending in .bak
#Args: none
checkForBaks(){ echo "Backup files outside of $csvDir:"; find "$myScriptsVar" -name "*.bak" ! -path "$csvDir*"; }

# This function checks for stdOut and stdErr files related to loadingBarSh.sh that is not in the sharedShellScripts dir
#Args: none
checkForLoadingBarStdErrStdOut(){
	echo "Loading bar standard out and standard error files not in $sharedScripts:"
	find "$myScriptsVar" ! -path "$(dirname "$sharedScripts")*" -name "*stdOutOutputLoadingBar*" -o ! -path "$(dirname "$sharedScripts")*" -name "*stdErrOutputLoadingBar*"
}

# This function checks if there are any error comments, defined in the envVars.csv
# Is a more robust version of previous function with similar function
#Args: none
checkForErrorComments(){
	# Vars
	local FF="${FUNCNAME[0]}"
	fieldValEnvVarsColNumAdj=$(expr $fieldValEnvVarsColNum + 1)
	errorVals=""

	# Getting Values. Finding error comment values and prepending comment char(s) before them and adding '|' between them for use as a regex for grep
	#regex: finding errorCom rows. Getting values from each row. Replacing \n with |. Removing trailing '|'. Replacing '|' with comment char(s). Adding comment char(s) to beginning of first errorCom value
	errorVals="$(sed -n '/errorCom/p' "$envVarsLoc" | cut -d ',' -f "$fieldValEnvVarsColNumAdj" | tr '\n' '|' | sed 's/|$//g' | sed 's/|/|\(\/\/|#|\/\\\*\)/g' | sed 's/^/\(\/\/|#|\/\\\*\)/g')"

	# Finding the error comments in the files. Using -i for case insensitive, 'I' to ignore obj files, 'E' for advanced regex, and 'r' for recursive. Further adding removal of .o and .so files and removing lines containing only lowercase remove
	echo "The following lines in the following files contain an error comment value:"
	allVals="$(grep -iIr -E "$errorVals" "$myScriptsVar" | grep -v -E ".*\.o|.*\.so" | grep -v "remove")"
	echo "$allVals"

	# Showing all lines containing "RE", case sensitive. Since lines containing both removeme and another error comment could have been been removed, this prints those missing lines
	echo "$(echo "$allVals" | grep "RE" | grep -v "$allVals")"
}

# This function checks if there is any hardcoded home paths
#Args: none
checkForHome(){
	echo "Checking for /home/ok:"
	homeGrep="$(grep -iIrl "/home/ok" "$myScriptsVar")"
	echo "$homeGrep"

	echo ""
	echo "Files containing linuxUtils:"
	#regex: finding all files (ignoring obj and shared obj files) containing myScriptsVar. Ignore previous findings and those preceded by "//"
	grep -iIr "linuxUtils" "$myScriptsVar" | grep -v "$homeGrep" | grep -v "//"
}

# This function checks for files named test, temp, or tmp 
#Args: none
checkForTestTempFiles(){ 
	echo "Checking for files named test, temp, or tmp:"
	find "$myScriptsVar" -name "*test*" -o -name "*temp*" -o -name "*tmp*"
}

# This function checks for files with 3 or more consecutive empty lines
#Args: none
checkForExcessEmptyLines(){
	echo "Checking for files with 3 or more consecutive lines:"
	# For each file, see if there are 3 or more consecutive empty lines. Print each file name only once
	find "$myScriptsVar" -type f -exec perl -lne 'if (/^\s*$/) { $empty++ } else { $empty = 0 } print $ARGV if $empty >= 3; END' {} + | sort | uniq

}

# Help Menu
helpMenuCheckErrVals(){
	echo "+++CHECK FOR ERROR VALUES - HELP MENU+++	Relative File Path: $0"
	echo "Purpose: check for a variety of error comments, values, or other itmes throughout the repo directory"
	echo "Options: none"
	echo "Usage:"
	echo "	<binNameOrAlias>"
}

# Main
main(){
#	checkForUnAssociatedFiles
#	checkForErrorComs
#	checkForHardcode
#	checkForBaks
#	checkForLoadingBarStdErrStdOut
#	checkForErrorComments
#	checkForHome
#	checkForTestTempFiles
#	checkForExcessEmptyLines
	echo "None of the functions selected"
}
main
