#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
objFiles=""
includedLibs=""
libName=""
makeFilePath=""
includedLibsHeaderFiles=""
objFilesHeaderFiles=""
runMakeFile=1

#File Locations
buildLoc="$(findPath -f 62)/" #build dir
libLoc="$(findPath -f 2)/" #lib dir
includeLoc="$(findPath -f 120)/" #include dir
cLibMkFileTemplateLoc="$(findPath -f 63)" #cLibMkFileTemplate.mk
compiledHeadersLoc="$(findPath -f 11)/" #compiledHeaders

# Error Message Constant Vars
F_MAKELIB="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# SOURCING-----------------------------------------------------------------------------------------
source "$sharedScripts"
checkFileLocs $F_AUTOEXEC
#--------------------------------------------------------------------------------------------------

# This function does data validation on the library name and adjusts it in case it includes an extension (.so) or begins with "lib"
#Args: 1 - first arg to mainMakeLib (should be name of library) 
adjustLibName(){
	# Vars
	local FF="${FUNCNAME[0]}"

	# Checking if file extension is correct, if one is provided
	if [[ $1 == *.* ]]; then #checking if file extension includes a '.'
		if [[ $1 =~ \.o$ ]]; then #checking if file extension is '.o' instead of '.so'. This signifies that I likely forgot to put the library name and just added the object files
			myPerrorSh $F_MAKELIB $FF "Arg 1 contains object file (.o) instead of shared object file (.so) or a name without an extension. Aborting"
			myPerrorSh $F_MAKELIB $FF "	Arg 1 doesn't expect file extensions (although it can accept them)"
			myPerrorSh $F_MAKELIB $FF "	The presence of an object file indicates that you might have forgotten to put the library/shared object name"
			return 1
		elif [[ ! $1 =~ \.so ]]; then
			myPerrorSh $F_MAKELIB $FF "Arg 1 contains an unsupported file extension. Aborting"
			return 1
		elif [[ $1 =~ \.so ]]; then
			myPerrorSh $F_MAKELIB $FF "removed file extension $(echo $1 | sed 's/.*\./\./g') from library name"
			libName=$(echo $1 | sed 's/\..*//g') #regex: remove everything after (and including) '.'
		fi
	else
		libName=$1
	fi



	# Checking if name begins with "lib"
	if [[ $libName == ^lib* ]]; then
		myPerrorSh $F_MAKELIB $FF "WARNING: Arg 1 (library name) begins with 'lib'. 'lib' will be added before the given file name, producing a file named \"lib$libName\""
		#this doesn't go to std error cuz if I supress all errors, this will expect an input and leave the program hanging
		echo "===$F_MAKELIB - $FF===		Do you want to remove the 'lib' at the beginning of the file name? (Y/n) " 
		
		read renameLib
		if [[ $renameLib =~ ^(y|Y)$ ]]; then
			#regex: removing lib if it is at the beginning of the argument
			libName=$(echo $libName | sed 's/^lib//g')
		elif [[ ! $renameLib =~ ^(n|N)$ ]]; then
			myPerrorSh $F_MAKELIB $FF "Invalid input, treating it as 'n' option"
		fi
	fi
	
	return 0
}

# This function creates a new makefile based off of a template if the make file doesn't already exist
#Args: none
createMakeFile(){
	# Vars
	local FF="${FUNCNAME[0]}"
	returnStatus=0

	makeFilePath="${buildLoc}$libName.mk"
	
	#if makefile doesn't already exist, then copy the template into a new file
	if [ ! -f "$makeFilePath" ]; then
		cp $cLibMkFileTemplateLoc $makeFilePath
		returnStatus=$?
		myPerrorSh $returnStatus $F_MAKELIB $FF "Failed to copy $cLibMkFileTemplateLoc to $makeFilePath. Returning $returnStatus"
	fi

	return $returnStatus
}

# This function adds the .o and .so files to the appropriate string variables (which will be used in later functions)
#Args: everything except library name (arg 1 or arg 2 is using --no-make option)
addArgumentsToStrings(){
	# Vars
	local FF="${FUNCNAME[0]}"
	
	for arg in $@; do #for every argument
		#if the arg ends in '.so' and is not the first arg (in case the first arg contains '.so'), then add to included libs array if it isn't already in there
		if [[ $(echo $arg | sed 's/.*\.//g') =~ ^so$ ]]; then
			if [[ $(grep $arg $makeFilePath) =~ ^$ ]]; then #if the arg is not already in the makeFile, then add it
				includedLibs+="\$(LIB_DIR)$arg " #add string to array, with a space
				#regex: replacing extension with '.h'
				includedLibsHeaderFile="$(echo $arg | sed 's/\..*/.h/g')"
				includedLibsHeaderFiles+="$includedLibsHeaderFile "
			fi
		elif [[ $(echo $arg | sed 's/.*\.//g') =~ ^o$ ]]; then #if arg ends in '.o', then add to object files array if it isn't already in there
			if [[ $(grep $arg $makeFilePath) =~ ^$ ]]; then #if the arg is not already in the makefile, then add it
				objFiles+="\$(OBJ_DIR)$arg " #add it to makefile with the OBJ_DIR variable (from makefile) and add a space at the end
				objFilesHeaderFile="$(echo $arg | sed 's/\..*/.h/g')"
				objFilesHeaderFiles+="$objFilesHeaderFile "
			fi
		else
			myPerrorSh $F_MAKELIB $FF "Input $arg doesn't have appropriate extension. Accepted extensions are: .o and .so"
			myPerrorSh $F_MAKELIB $FF "	Not adding arg to makefile. Program will continue running"
		fi
	done
	
	return 0
}

# This function creates a header file for the library if one doesn't already exist and adds include guards
#Args: none
createLibHeaderFile(){
	# Vars
	headerFilePath="${includeLoc}$libName.h"
	local FF="${FUNCNAME[0]}"

	if [ -f "$headerFilePath" ]; then
		myPerrorSh $F_MAKELIB $FF "Header file for the library already exists. Returning 1"
		return 1
	fi


	touch "$headerFilePath"
	
	#converting library name to all uppercase
	defineName=$(echo "$libName" | tr '[:lower:]' '[:upper:]')

        # Add the define statement and include guards
	echo "#ifndef $defineName" >> "$headerFilePath"
	echo "#define $defineName" >> "$headerFilePath"
	echo "" >> "$headerFilePath"
	echo "#endif" >> "$headerFilePath"

	return 0
}


# This function adds new object files and libraries to the header file associated with the library
#Args: none
addNewFilesToHeaderFile(){
	# Vars
	local FF="${FUNCNAME[0]}"
	currLine=1
	
	# If header file doesn't exist, make one
	if [ ! -f "${includeLoc}$libName.h" ]; then
		myPerrorSh $F_MAKELIB $FF "WARNING: File header (arg 1) doesn't exits. Couldn't find ${includeLoc}$libName.h"
		myPerrorSh $F_MAKELIB $FF "	Creating new header file $libName.h at $includeLoc"
		createLibHeaderFile
	fi
	
	#get line num of last include
	lastLineNumOfInclude=$(grep -n "#include \"" "${includeLoc}$libName.h" | cut -d ':' -f 1 | tail -n 1)
	if [[ "$lastLineNumOfInclude" =~ ^$ ]]; then
		myPerrorSh $F_MAKELIB $FF "WARNING: no existing #include statements found in $libName.h. Program will continue, adding new ones to the line above the #endif"
		lastLineNumOfInclude=$(grep -n "#endif" "${includeLoc}$libName.h" | cut -d ':' -f 1 | tail -n 1)
		#subtracting 1 cuz further down, I'm adding currLine (which is 1) to lastLineNumOfInclude. I want to insert above/before "#endif", so I need to subtract 1
		lastLineNumOfInclude=$(expr $lastLineNumOfInclude - 1)
	fi

	# Getting the current list of includes in the file so I don't add duplicates
	listOfIncludes="$(cat $makeFilePath | grep "#include \"")"
	
	#add each included library (converted to a header file name) to library header file at line lastInclude+currLine
	for lib in $includedLibsHeaderFiles; do
		#if header file doesn't already exist
		if [ "$(echo $listOfIncludes | grep "$lib")" == "" ]; then
			lineToAddInclude=$((currLine+lastLineNumOfInclude))
			sed -i "${lineToAddInclude}i #include \"$lib\"" "${includeLoc}$libName.h"
			currLine=$(expr $currLine + 1)
		fi
	done

	#add each included object file (converted to a header file name) to library header file at line lastInclude+currLine
	for cFile in $objFilesHeaderFiles; do
		#if header file doesn't already exist
		if [ "$(echo $listOfIncludes | grep "$cFile")" == "" ]; then
			lineToAddInclude=$((currLine+lastLineNumOfInclude))
			sed -i "${lineToAddInclude}i #include \"$cFile\"" "${includeLoc}$libName.h"
			currLine=$(expr $currLine + 1)
		fi
	done
	
	return 0
}

# This function adds the included libraries and object files to the make file
#Args: none
addStringsToMakeFile(){
	# Vars
	local FF="${FUNCNAME[0]}"
	
	# Add included libs to file (if includedLibs variable exists)
	if [[ ! $includedLibs =~ ^$ ]]; then
		includedLibLineNum=$(grep -n "INCLUDED_LIB_NAME =" $makeFilePath | cut -d ':' -f 1)
		# Data Validation
		if [[ $includedLibLineNum =~ ^$ ]]; then 
			myPerrorSh $F_MAKELIB $FF "Variable \"INCLUDED_LIB_NAME\" not found in $makeFilePath. Aborting"
			return 1
		fi
		#regex: adding the included libs to the end of the line where the var name "INCLUDED_LIB_NAME" was found. Using '|' in sed command cuz '/' chars could be in the contents of the vars
		sed -i "$includedLibLineNum s|\s\?\$| $includedLibs|g" $makeFilePath
	fi

	# Add object files to file (if objFiles variable exists)
	if [[ ! $objFiles =~ ^$ ]]; then
		objFilesLineNum=$(grep -n "OBJ_FILES =" $makeFilePath | cut -d ':' -f 1)
		# Data Validation
		if [[ $objFilesLineNum =~ ^$ ]]; then
			myPerrorSh $F_MAKELIB $FF "Variable \"OBJ_FILES\" not found in $makeFilePath. Aborting"
			return 1 
		fi
		#regex: adding the object files to the end of the line where the var name "OBJ_FILES" was found. Using '|' in sed command cuz '/' chars could be in the vars
		sed -i "$objFilesLineNum s|\s\?\$| $objFiles|g" $makeFilePath
	fi

	# Add library name to target variable if the target variable is empty
	targetLineNum=$(grep -n "TARGET =" $makeFilePath | cut -d ':' -f 1)
	# Data Validation
	if [[ $targetLineNum =~ ^$ ]]; then
		myPerrorSh $F_MAKELIB $FF "Variable \"TARGET\" not found in $makeFilePath. Aborting"
		return 1
	fi
	#regex: find the content of the line number that contains "TARGET =" (represented by the var targetLineNum). Remove "TARGET =" from it
	targetVarContent=$(sed -n "${targetLineNum}p" $makeFilePath | sed 's/TARGET =\s\?//g')
	if [[ $targetVarContent =~ ^$ ]]; then #if the target line in the make file is empty
		#regex: adding the library name to the end of the line where the var name "TARGET" was found. Using '|' in sed command cuz '/' chars could be in the vars
		sed -i "$targetLineNum s|\s\?\$| $libName|g" $makeFilePath
	elif [[ ! $targetVarContent =~ ^$ ]] && [[ ! "$targetVarContent" == "$libName" ]]; then #if the target line content isn't empty and its value is different from the library name
		myPerrorSh $F_MAKELIB $FF "The library name provided - $libName - is not the same as the existing library name stored in the TARGET variable. Aborting"
		return 1
	fi
}

# This function checks to see if there are any missing object files and associated header files for each object file in the make file
#Args: none
checkAllFilesInMakeFileExist(){
	# Vars
	local FF="${FUNCNAME[0]}"
	objFilesInMakeFile="$(cat $makeFilePath | grep "OBJ_FILES =\s\?" | sed 's/OBJ_FILES =\s\?//g' | sed 's/$(OBJ_DIR)//g')"
	
	if [[ $objFilesInMakeFile =~ ^$ ]]; then
		runMakeFile=0
		myPerrorSh $F_MAKELIB $FF "WARNING: No object files"
	fi


	# Checking if each object file exists
	for objInMakeFile in $objFilesInMakeFile; do
		# Check if object in the make file exists
		if [ ! -f "${compiledHeadersLoc}$objInMakeFile" ]; then
			myPerrorSh $F_MAKELIB $FF "Object file $objInMakeFile in $libName.mk not found in $compiledHeadersLoc"
			runMakeFile=0
		fi

		# Get object's associated header file name
		headerFile="$(echo $objInMakeFile | sed 's/\..*/.h/g')"
		
		# Check if object's associated header file exists
		if [ ! -f "${includeLoc}$headerFile" ]; then
			myPerrorSh $F_MAKELIB $FF "Header file $headerFile not found in $includeLoc. The associated object file was included in $libName.mk"
			runMakeFile=0
		fi


	done

	#if one of the files doesn't exist, then print that the make command won't be run
	if [ $runMakeFile -eq 0 ]; then
		myPerrorSh $F_MAKELIB $FF "Not running \"make\" command for $libName.mk"
	fi


}

# This function removes elements from a makefile
#Args: all
removeElement(){
	# Vars
	local FF="${FUNCNAME[0]}"

	# Making sure lib name (arg 1) is correct
	adjustLibName $1
	if [ ! $? -eq 0 ]; then
		myPerrorSh $F_MAKELIB $FF "adjustLibName() function returned an error. Returning 1"
		return 1
	fi
	shift 2

	# Getting file path for make file
	makeFilePath="${buildLoc}$libName.mk"
	if [ ! -f "$makeFilePath" ]; then
		myPerrorSh $F_MAKELIB $FF "File $libName.mk doesn't exist in ${buildLoc}. Returning 1"
		return 1
	fi

	# for every arg
	for element in $@; do
		#regex: replace the arg anywhere in make file
		sed -i "s|$element||g" "$makeFilePath"
		
		# The following few lines removes the extra elements added to the name of the file so the makefile can compile
		#remove the preceding file location variable if there isn't anything proceeding the var
		sed -i 's/\$(OBJ_DIR) //g' "$makeFilePath"
		sed -i 's/\$(OBJ_DIR)$//g' "$makeFilePath"
		
		#remove "-l" if it isn't followed by anything
		sed -i 's/-l //g' "$makeFilePath"
		sed -i 's/-l$//g' "$makeFilePath"
		
		#remove extra spaces
		sed -i "s/  / /g" "$makeFilePath"
	done

	# Make sure all files exist
	checkAllFilesInMakeFileExist

	if [ $runMakeFile -eq 1 ]; then 
		make -f $makeFilePath
		rs=$?
		myPerrorSh $rs $F_MAKELIB $FF "Error making $libName.mk"
		return $rs
	fi

	return 0

	
}

# This function makes and/or adds args to make file by combining the functions above
#Args: all (except for optional --no-make)
makeAndAddElementsToLib(){
	adjustLibName $1
	if [ ! $? -eq 0 ]; then
		myPerrorSh $F_MAKELIB $FF "Error with adjustLibName() function. No changes made to makefile"
		return 1
	fi
	
	createMakeFile
	if [ ! $? -eq 0 ]; then myPerror $F_MAKELIB $FF "Error with createMakeFile() function. Returning 1"; return 1; fi
	shift 1
	
	addArgumentsToStrings $@
	
	addNewFilesToHeaderFile
	if [ ! $? -eq 0 ]; then myPerrorSh $F_MAKELIB $FF "Error with addNewFilesToHeaderFile() function. Returning 1"; return 1; fi
	
	addStringsToMakeFile
	if [ ! $? -eq 0 ]; then myPerrorSh $F_MAKELIB $FF "Error with addStringToMakeFile() function. Returning 1"; return 1; fi


	#if runMakeFile var is true, then check if all files exist. If not, runMakeFile var will be set to 0 (false)
	if [ $runMakeFile -eq 1 ]; then checkAllFilesInMakeFileExist; fi
	
	if [ $runMakeFile -eq 1 ]; then
		make -f $makeFilePath
		if [ ! $? -eq 0 ]; then myPerrorSh $F_MAKELIB $FF "Error making $makeFilePath. Returning 1"; return 1; fi
	fi
	
	return 0

}


# Help Menu Print Statements
helpMenu(){
	echo "+++MAKE LIBRARY - HELP MENU+++	Relative File Path: $0"
	echo "Purpose: create a new or update an existing makefile for a library and add the given object files and/or shared object files"
	echo "Deatils: if the library name given in the first argument already exist, this program will add the following arguments to the existing makefile. Otherwise, it will create a new makefile"
	echo "Options: "
	echo "	--no-make	:- don't run the \"make\" command. Used for when you add an object file that doesn't exist yet. If you include this option, it must be the first argument"
	echo "Usage: "
	echo "	<binNameOrAlias> <libraryName without extension or preceding lib> <object or sharedObject files>"
	echo "Examples: "
	echo "	<binNameOrAlias> libName object.o object2.o sharedObject.so"
	echo "Notes: "
	echo "	-The library name should not include the beginning 'lib' or '.so' extension. If you include either, the extension will be auto-removed, but you will be asked if you want to remove the 'lib' part"
}

# Main function for program
mainMakeLib(){
	# Vars
	local FF="${FUNCNAME[0]}"
	runMakeFile=1 #boolean to see if script should run the "make" command. If I add an obj file that doesn't exist yet, I don't want to run this function cuz the make command will throw an error. This is important when this script is used in other scripts

	# Data Validation
	if [ $(isHelpSh $1) -eq 1 ]; then helpMenu; return 1; fi
	if [ $# -lt 2 ]; then
		myPerrorSh $F_MAKELIB $FF "Invalid number of args. At least 2 are required, $# were provided"
		return 1
	fi
	if [ $(regexMatchSh $1 "--no-make") -eq 1 ]; then runMakeFile=0; shift 1; fi


	if [ "$2" == "-r" ] || [ "$2" == "--remove" ]; then
		removeElement $@
		rs=$?
		myPerrorSh $rs $F_MAKELIB $FF "Error removing element"
		return $rs
	else
		makeAndAddElementsToLib $@
		rs=$?
		myPerrorSh $rs $F_MAKELIB $FF "Error making and/or adding elements to makefile"
		return $rs
	fi

	return 0
}

mainMakeLib $@
