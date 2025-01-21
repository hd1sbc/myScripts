#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
associatedHeader=""
globalListOfIncludes=""

#File Locations
compiledHeadersLoc="$(findPath -f 11)/" #compiledHeaders dir
includeLoc="$(findPath -f 120)/" #include dir
sourcedShellScriptsLoc="$sharedScripts"

#Error Message Constant Vars
F_CREATEHEADER="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# SOURCING-----------------------------------------------------------------------------------------
source "$sourcedShellScriptsLoc"
checkFileLocs $F_CREATEHEADER
#--------------------------------------------------------------------------------------------------

# This function determines if the file is a header file by checking to see if there is a main() function
#Args: file name
isSourceCodeForHeaderFile(){
	#regex: check if int or void at beginning of line, space, main(, anything, ), possible space, and open bracket
	containsMainFunc=$(cat $1 | sed -n '/^\(int\|void\) main(.*)\s\?{\s\?$/p')
	if [[ $containsMainFunc =~ ^$ ]]; then echo 1; else echo 0; fi
	return 0
}

# This function finds the functions in a file and adds them to the global var "listOfFunctions"
#Args: 1 - file name
extractFunctions(){
	# Vars
	listOfFunctions=""

	#separating all of these as different vars to make it easier to read and debug
	
	#regex: looks for 3 basic return types followed by 0 or more pointers, name of function, (, anything, ). Removes everything after '{'. Removes main function
	functionsHolder=$(cat $1 | sed -n '/^\(int\|char\|void\)\** [a-zA-Z0-9_]\+\s\?(.*)\s\?{\s\?/p' | sed 's/{.*$/{/g' | grep -v "main")

	#regex: looks for struct followed by name of struct, rest copied from above
	functionsStructHolder=$(cat $1 | sed -n '/^struct [a-zA-Z0-9]\+\** [a-zA-Z0-9_]\+\s\?(.*)\s\?{\s\?/p' | sed 's/{.*$/{/g')

	#this looks for non-standard types that are defined using typedef
	#regex: remove lines beginning with functionHolder types, same as functionsHolder, but allowing for any type, not just int, char, or void
	functionsWithTypeDefHolder=$(cat $1 | sed -n '/^\(int\|char\|void\)/d; /^[a-zA-Z0-9_]\+\** [a-zA-Z0-9]\+\s\?(.*){\s\?/p' | sed 's/{.*$/{/g')

	# Removing everything (and including) '{' and replacing '}' with ';' to get just the function definition
	functions=$(echo "$functionsHolder"  | sed 's/\s\?{.*/;/g') 
	functionsStruct=$(echo "$functionsStructHolder"  | sed 's/\s\?{.*/;/g')
	functionsWithTypeDef=$(echo $functionsWithTypeDefHolder | sed 's/\s\?{.*/;/g')


	#adding all vars to single global var
	#keeping it as string instead of array to make it more POSIX-compatible
	if [[ ! $functions =~ ^$ ]]; then listOfFunctions+="$functions"; fi
	
	if [[ ! $functionsStruct =~ ^$ ]]; then
		if [[ ! $functions =~ ^$ ]]; then
			listOfFunctions+="\n$functionsStruct"
		else
			listOfFunctions+="$functionsStruct"
		fi
	fi

	if [[ ! $functionsWithTypeDef =~ ^$ ]]; then
		if [[ ! $functionsStruct =~ ^$ ]] || [[ ! $functions =~ ^$ ]]; then
			listOfFunctions+="\n$functionsWithTypeDef"
		else
			listOfFunctions+="$functionsWithTypeDef"
		fi
	fi

	echo "$listOfFunctions"

}

# This function finds the typedef and structs in a file and adds them to a list
#Args: 1 - file name
extractStructsAndTypeDefs(){

	listOfTypeDefs="$(sed -n '/typedef struct{/,/} [a-zA-Z0-9]\+;/p' $1)"
	listOfStructs="$(sed -n '/struct [a-zA-Z0-9]\s\?{/,/};/p' $1)"

	listOfExtractStructsAndTypeDefs=""
	listOfExtractStructsAndTypeDefs+="$listOfTypeDefs"
	if [[ ! "$listOfExtractStructsAndTypeDefs" =~ ^$ ]]; then
		listOfExtractStructsAndTypeDefs+="\n$listOfStructs"
	else
		listOfExtractStructsAndTypeDefs+="$listOfStructs"
	fi

	echo "$listOfExtractStructsAndTypeDefs"
}


# This function finds the define statements in a file and adds them to the global var "listOfDefines"
#Args: 1 - file name
extractDefines(){
	#ASSUME: this assumes all define statements end with a '\'
	#regex: finds all backslashes at end of line (with possibility of being followed by whitespace). Adds a newline char before the lines beginning with '#define '. If it doesn't begin with that, it will not add a newline char (this sentence is what the '1' at the end of the awk statement is
	#doing a regex for just the '#define ' would not allow for multi-line defines. While this assumes correct syntax, it makes my life a lot easier. It also helps prevent against including the define statement for the header file (the line after "ifndef ...")
	defines=$(cat $1 | sed -n '/\\\s*$/{N; p;}' | awk '/^#define .*\\\s*/{print""; print; next}1')

	#not directly setting listOfDefines to the regex above in case I want to add more vars for special cases
	listOfDefines=""
	listOfDefines+="${defines}"
	if [[ ! $listOfDefines =~ ^$ ]]; then
		listOfDefines+="\\" #for some reason, the last '\' in the last line of listOfDefines would get cut off. Idk why
		listOfDefines+=$(echo -e "\n")
	fi

	echo "$listOfDefines"
	return 0
}

# This function finds the include statements in a file and adds them to the global var "listOfIncludes"
#Args: 1 - file name
extractIncludes(){
	standardIncludes=$(cat $1 | sed -n '/^#include <.*>/p') #for c library includes (ex: stdio.h)
	nonStandardIncludes=$(cat $1 | sed -n '/^#include ".*"/p') #for my libraries
	
	listOfIncludes=""
	listOfIncludes+="$standardIncludes"
	if [[ ! $listOfIncludes =~ ^$ ]]; then
		listOfIncludes+="$(echo -e "\n$nonStandardIncludes")"
	else
		listOfIncludes+="$nonStandardIncludes"
	fi
	
	echo "$listOfIncludes"
	return 0
}

# This function finds the global variables and adds them to the global var "listOfGlobalVars"
#Args: 1 - file name, 2 - listOfFunctions, 3 - listOfDefines
extractGlobalVars(){
	# Vars
	listOfGlobalVars=""
	local FF="${FUNCNAME[0]}"

	#ASSUME: this assumes that I wouldn't put global variables above include statements

	# Finding the line number in the file of the first non-include preprocessor statement
	listOfPreProcessors=$(echo -e "$3\n$2") #gets list of all preprocessors excluding includes
	firstElement="${listOfPreProcessors%%$'\n'*}" #finds first element in the list of preprocessors. Not really sure how this regex works, but it does
	firstElement=$(echo "$firstElement" | sed 's/).*//g') #removing everything (and including) after ')' cuz listOfPreProcessors turned the lines into declaration statements, which don't exactly match implementation. The sed can't be combined with the line above cuz it throws an error. idk why
	lineNum=$(cat $1 | grep -F -n "$firstElement" | cut -d ':' -f 1 | sed -n '1p') #getting the line num of first element and extracting the actual value (grep -n format is lineNum: lineMatchingPattern). Printing out the first line from that output. There could be multiple lines that match that first element if the first element is called elsewhere in that file. The 1st line from the grep statement is where the function must've been declared
	
	# This extracts the global vars from the start of the file until the line of the first non-include preprocessor
	if [[ ! $lineNum =~ ^$ ]]; then
		#regex: data type (with possible pointers), , variable name, ';' or =. Replace everything after (and including) '=' with ';'. Add the word extern to beginning of line
		listOfGlobalVarsHolder=$(head -n "$lineNum" $1 | sed -n '/^[a-zA-Z1-9]\+\** [a-zA-Z1-9]\+\s\?\(\[[0-9]*\]\)*\(;\|\s*=\)/p' | sed 's/\s\?=.*/;/g' | sed 's/^/extern /g')
		#regex: same as above, but with the word 'struct' at the beginning
		listOfGlobalVarsWithStructsHolder=$( head -n "$lineNum" $1 | sed -n '/^struct [a-zA-Z1-9]\+\** [a-zA-Z1-9]\+\s\?\(\[[0-9]*\]\)*\(;\|\s*=\)/p' | sed 's/ =.*/;/g' | sed 's/^/extern /g') #separating these two vars to make it easier to debug
		
		if [[ ! "$listOfGlobalVarsWithStructsHolder" =~ ^$ ]]; then listOfGlobalVars+="$listOfGlobalVarsWithStructsHolder\n"; fi
		if [[ ! "$listOfGlobalVarsHolder" =~ ^$ ]]; then listOfGlobalVars+="$listOfGlobalVarsHolder"; fi
	fi

	echo "$listOfGlobalVars"
}

# This function adds an include statement (if it doesn't already exist) of the header file that is about to/has just been created
#Args: 1 - file name (.c file), 2 - listOfIncludes
addSourceCodeAssociatedFileHeader(){
	#check if source code's associated file header exists
	fileName=$(echo $1 | sed 's/.*\///g')
	associatedHeader=$(echo $fileName | sed "s/\..*/\.h/g") #regex: changes file name from .c (or whatever extension) to .h
	fileHeaderExists=0 #var to check if file header already exists

	# Look through include statements to see if associated header file already exists
	for include in $globalListOfIncludes; do
		include="$(echo "$include" | sed 's/\"//g')" #regex: removing '"' from include var since includes in c require them
		
		# If include contains a symlink "ends in ".sl"), then set the include var to be the symlink target path
		if [ ! "$(echo "$include" | sed -n '/\.sl/p')" == "" ]; then
			include="$(getFileNameSh "$(readlink -f "${symLinkLoc}$include")")"
		fi
		
		if [ ! "$include" == "" ] && [ "$include" == "$associatedHeader" ]; then fileHeaderExists=1; fi
	done

	# Add include statement of file header if it doesn't exist
	if [ $fileHeaderExists -eq 0 ]; then
		headerLine="#include \"$associatedHeader\""
		sed -i "1i $headerLine" $1 #inserts at line 1 in the file
	fi

	return 0
}

# This function combines the functions above to create a new header file or update the existing one
#Args: 1 - file name, 2 - listOfIncludes, 3 - listOfGlobalVars, 4 - listOfDefines, 5 - listOfFunctions, 6 - structs & typedefs
createNewHeaderFile(){
	# Vars
	listOfIncludes=$2
	listOfGlobalVars=$3
	listOfDefines=$4
	listOfFunctions=$5
	listOfExtractStructsAndTypeDefs=$6

	fileName=$(echo $1 | sed 's/.*\///g') #this gets the file name in case the name is a path
	headerFileName=$(echo $fileName | sed "s/\..*/\.h/g") #regex: replaces everything after '.' with '.h'
	headerFilePath="${includeLoc}$headerFileName"
		
	if [ -f $headerFilePath ]; then #if header file already exists
		#ASSUME: I assume that if macros already exist in the header file, then any macros in the implementation file are no longer used/needed
		#	-The macros would only be in the implementation file during initial testing - before a header file is made. So once the header file is made, the macros would no longer be in use
		listOfDefines=$(extractDefines $headerFilePath) #getting existing defines from header file and setting those to be the ones to be added
		listOfExtractStructsAndTypeDefs=$(extractStructsAndTypeDefs $headerFilePath)
		truncate -s 0 $headerFilePath #empty the contents of the file
	else
		touch "$headerFilePath"
	fi

	# Create the name of the define for the file. It is the file name without the extension in all uppercase
	#regex: remove everything after (and including) '.'. Make everything uppercase
	defineName=$(echo "$headerFileName" | sed 's/\..*//g' | tr '[:lower:]' '[:upper:]')
	
	# Add the define statement and include guards, if they don't already exist
	if [[ $(sed "/$defineName/p" $headerFilePath) =~ ^$ ]]; then #if the define name is not found anywhere in the file
		echo "#ifndef $defineName" >> "$headerFilePath"
		echo "#define $defineName" >> "$headerFilePath"
		echo "" >> "$headerFilePath"
	else
		echo "" >> "$headerFilePath"
	fi

	# Add include statements, if they exist
	listOfIncludes=$(echo "$listOfIncludes" | grep -v "$associatedHeader") # this removes the associated header include found in the implementation file
	if [[ ! $listOfIncludes =~ ^$ ]]; then
		echo -e "$listOfIncludes" >> "$headerFilePath"
		echo -e "\n" >> "$headerFilePath"
	fi

	# Add global vars, if they exist
	if [[ ! $listOfGlobalVars =~ ^$ ]]; then
		echo "// GLOBAL VARS-------------------------------------------------------------------------------------" >> "$headerFilePath"
		echo -e "$listOfGlobalVars" >> "$headerFilePath"
		echo "//-------------------------------------------------------------------------------------------------" >> "$headerFilePath"
		echo "" >> "$headerFilePath"
	fi

	# Add list of structs and typedefs
	if [[ ! $listOfExtractStructsAndTypeDefs =~ ^$ ]]; then
		echo "//=================================================================================================" >> "$headerFilePath"
		echo "// STRUCTS AND TYPEDEFS" >> "$headerFilePath"
		echo "//=================================================================================================" >> "$headerFilePath"
		echo "" >> "$headerFilePath"
		echo -e "$listOfExtractStructsAndTypeDefs" >> "$headerFilePath"
		echo "//=================================================================================================" >> "$headerFilePath"
		echo "" >> "$headerFilePath"
	fi
		
	# Add list of defines, if they exist
	if [[ ! $listOfDefines =~ ^$ ]]; then
		echo "//=================================================================================================" >> "$headerFilePath"
		echo "// DEFINES" >> "$headerFilePath"
		echo "//=================================================================================================" >> "$headerFilePath"
		echo "" >> "$headerFilePath"
		echo -e "$listOfDefines\n" >> "$headerFilePath"
		echo "//=================================================================================================" >> "$headerFilePath"
		echo "" >> "$headerFilePath"
	fi

	# Add list of function declarations, if they exist
	if [[ ! $listOfFunctions =~ ^$ ]]; then
		echo "//=================================================================================================" >> "$headerFilePath"
		echo "// FUNCTION DECLARATIONS" >> "$headerFilePath"
		echo "//=================================================================================================" >> "$headerFilePath"
		echo "" >> "$headerFilePath"
		echo -e "$listOfFunctions" >> "$headerFilePath"
		echo "//=================================================================================================" >> "$headerFilePath"
	fi

	# Add endif for header define
	echo "" >> "$headerFilePath"
	echo "#endif" >> "$headerFilePath"

}

#TODO #FILLIN
updateBackendTrackingExecC(){
	echo "hello";
}

# Main function
#Args: 1 - file name
createHeaderFile(){
	local FF="${FUNCNAME[0]}"

	isHeaderFile=$(isSourceCodeForHeaderFile $1)
	if [ $isHeaderFile -eq 1 ]; then #if a header file
		listOfDefines=$(extractDefines $1)
		listOfIncludes=$(extractIncludes $1)
		globalListOfIncludes="$listOfIncludes"
		listOfExtractStructsAndTypeDefs=$(extractStructsAndTypeDefs $1)
		addSourceCodeAssociatedFileHeader $1
		listOfFunctions=$(extractFunctions $1)
		listOfGlobalVars=$(extractGlobalVars $1 $listOfFunctions $listOfDefines)
		createNewHeaderFile $1 "$listOfIncludes" "$listOfGlobalVars" "$listOfDefines" "$listOfFunctions" "$listOfExtractStructsAndTypeDefs"
	else
		myPerrorSh $F_CREATEHEADER $FF "WARNING: File $1 is not a header file. No header file created"
	fi
}

# This function compiles the header file using gcc
#Args: 1 - file name
compileImplementationFile(){
	fileName=$(echo $1 | sed 's/.*\///g') #this gets the file name in case the name is a path
	objectFileName=$(echo $fileName | sed "s/\..*/\.o/g")
	fileObjPathLocal="$compiledHeadersLoc$objectFileName"

	#adjusted string for use in sed by putting backslash before '/' and '.'
	fileObjPathAdj="$(echo "$fileObjPathLocal" | sed 's/\./\\\./g' | sed 's/\//\\\//g')"
	nonLibraryHeaderObjFilePaths="$(echo "$nonLibraryHeaderObjFilePaths" | sed "s/$fileObjPathAdj //g")"
	
	gcc -Wall -g -c -fPIC $1 $nonLibraryHeaderObjFilePaths -o $fileObjPathLocal -L$libLoc $libsListInGccFormatString -I$includeLoc -I$symLinkLoc -Wl,-rpath=$libLoc #compile with necessary lib and include locations
	#gcc -fPIC -c $1 -o $fileObjPathLocal -I$includeLoc #simple compile
}

# This function combines the function above to create and compile the header file
#Args: 1 - file name
compileHeaderImplementationFile(){
	createHeaderFile $1
	compileImplementationFile $1
}
