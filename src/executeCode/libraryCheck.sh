#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
libs=""
nonLibraryHeaderObjFilePaths=""
potentialLibs=""
dirsToLookIn=""
libsListForGccCommand=""

#File Locations
libLoc="$(findPath -f 2)/" #lib dir
buildLoc="$(findPath -f 62)/" #build dir
mainFolderLoc="$myScriptsVar"
includeLoc="$(findPath -f 120)/" #include dir
executeCodeLoc="$(findPath -f 109)/" #execute code
backupsLoc="$(findPath -f 43)/" #backups dir
compiledHeadersLoc="$(findPath -f 11)/" #compiledHeaders dir
makeLibShLoc="$(findPath -f 108)" #makeLib.sh
symLinkLoc="$(findPath -f 146)/" #symLinks dir
sourcedShellScriptsLoc="$sharedScripts"
createCHeaderLoc="$(findPath -f 115)" #createCHeader.sh

# Error Message Contant Vars
F_LIBCHECK="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# SOURCING-----------------------------------------------------------------------------------------
source "$sharedScripts"
checkFileLocs $F_LIBCHECK
source "$createCHeaderLoc"
#--------------------------------------------------------------------------------------------------

# This function extracts exclusively non-standard includes (those not in the standard C library). This function is different from extractIncludes() in "createHeader.sh"
#Args: 1 - file path
extractNonStandardIncludes(){
	# Vars
	local FF="${FUNCNAME[0]}"
	
	# Data Validation
	if [ ! -f $1 ]; then myPerrorSh $F_LIBCHECK $FF "File $1 not found. Returning 1"; return 1; fi

	#regex: looks for lines conntaining '#include ""'. Remove '#include "' part. Remove last '"'
	potentialLibsHolder=$(cat $1 | sed -n '/^#include ".*"/p' | sed 's/#include "//g' | sed 's/"//g')

	# Checking to see if every include statement is a symlink, and if so, find its target file	
	for potentLib in $potentialLibsHolder; do
		# If file ends in ".sl" (meaning it is a symlink)
		if [ $(regexMatchSh "\\.sl" $potentLib) -eq 1 ]; then
			if [ ! -f "${symLinkLoc}$potentLib" ]; then #checking if symlink exists in symLinks dir
				myPerrorSh $F_LIBCHECK $FF "WARNING: Couldn't find symlink $potentLib in $symLinkLoc. Not adding it to list of libraries"
			else
				#get the target file path
				symLinkTargetFilePath="$(readlink ${symLinkLoc}$potentLib)"
				#add the target file without the path and extension
				potentialLibs+="$(getFileNameNoExtSh $symLinkTargetFilePath) "
			fi
		else
			#get file name without path and extension
			potentialLibs+="$(getFileNameNoExtSh $potentLib) "
		fi
		

	done

	return 0
}

# This function finds the corresponding library for each non-standard include statement
#Args: none
findCorrespondingLib(){
	# Vars
	local FF="${FUNCNAME[0]}"

	# Adds non-standard include to libs variable if there is a library with that name
	for potentialLibName in $potentialLibs; do
		if [ -f "${libLoc}lib$potentialLibName.so" ]; then #sees if the shared object file (.so) exists
			libs+="$potentialLibName " #add to string
			libsListForGccCommand+="$potentialLibName "
		fi
	done

	return 0
}

# This function finds the corresponding header file in the include dir for each non-standard include statement that is not a header file for a library
#Args: none
findCorrespondingNonLibHeaderFile(){
	# Vars
	local FF="${FUNCNAME[0]}"

	# Adds non-standard include to non-library header object list if the name isn't already in libs var and the header file exists in the include dir
	for potentialLibName in $potentialLibs; do
		#if not a library header file, check if a normal header file exists. If so, finds its associated object file path
		if [ ! -f "${includeLoc}$potentialLibName.h" ]; then 
			myPerrorSh $F_LIBCHECK $FF "Include file $potentialLibName.h not found. Program will continue, but the compiler will likely throw an error. Details:"
			myPerrorSh $F_LIBCHECK $FF "	Did not find a library file named lib$potentialLibName.so in $libLoc"
			myPerrorSh $F_LIBCHECK $FF "	Did not find a header file named $potentialLibName.h in $includeLoc"
		elif [[ "$(echo "$globalLibs" | grep "$potentialLibsName")" =~ ^$ ]]; then 
			nonLibraryHeaderObj+="$potentialLibName.o "
			#if the obj file exists (could be a header file with no associated obj file) and obj file isn't already in list
			if [ -f "${compiledHeadersLoc}$potentialLibName.o" ] && 
					[ "$(echo "$nonLibraryHeaderObjFilePaths" | sed -n "/$potentialLibName.o/p")" == "" ]; then
				nonLibraryHeaderObjFilePaths+="${compiledHeadersLoc}$potentialLibName.o "
			fi
		fi
	done

	return 0
}

# This function gets the file names (without extension) of each object file in the library
#Args: 1 - libName without ext
extractObjFilesFromMkFile(){
	# Vars
	local FF="${FUNCNAME[0]}"
	local objFilesList=""

	# Data Validation
	objFilesVarInMakeFile=$(grep "OBJ_FILES" "${buildLoc}$1.mk")
	if [[ $objFilesVarInMakeFile =~ ^$ ]]; then
		myPerrorSh $F_LIBCHECK $FF "The vairable \"OBJ_FILES\" was not found in the make file \"${libLoc}$1.mk\". The program will continue running, but errors will likely occur"
	fi
	
	#regex: gets OBJ_FILES line. Removes "OBJ_FILES =" to just get the list of .o files
	objFilesListTemp=$(grep "OBJ_FILES =\s\?" "${buildLoc}$1.mk" | sed 's/OBJ_FILES =\s\?//g')

	#for each object (.o) file, remove the extension and the directory location (OBJ_DIR) and add it to an array
	for obj in $objFilesListTemp; do
		#regex: remove everything after (and including) '.o'. Remove "$(OBJ_DIR)"
		objNoExt=$(echo $obj | sed 's/\..*//g' | sed 's/$(OBJ_DIR)//g') #HARDCODED_MKFILE_VAR
		objFilesList+="$objNoExt " #add to array
	done
	
	echo "$objFilesList"
	return 0
}

# This function gets the included libraries from the makefile (which are stored in the function INCLUDED_LIB_NAME) according to "cLibMkFileTemplate.mk"
#Args: 1 - libName without ext
getIncludedLibsFromMkFile(){
	# Vars
	local FF="${FUNCNAME[0]}"
	local localIncludedLibsList=""
	
	includedLibsInMakeFile=$(grep "INCLUDED_LIB_NAME" "${buildLoc}$1.mk") #HARDCODED_MKFILE_VAR
	if [[ $includedLibsInMakeFile =~ ^$ ]]; then
		myPerrorSh $F_LIBCHECK $FF "The vairable \"INCLUDED_LIB_NAME\" was not found in the make file \"${libLoc}$1.mk\". The program will continue running, but errors will likely occur"
	fi
	
	#regex: remove "INCLUDED_LIB_NAME =" from line
	includedLibsListTemp=$(grep "INCLUDED_LIB_NAME =\s\?" "${buildLoc}$1.mk" | sed 's/INCLUDED_LIB_NAME =\s\?//g')
		
	#for each shared object (.so) file, add the library standard name and add to array
	for sharedObj in $includedLibsListTemp; do
		#regex: remove everything after (and including) '.'. Remove "$(LIB_DIR). Remove -l at beginning of line
		sharedObjNoExt=$(echo $sharedObj | sed 's/\..*//g' | sed 's/$(LIB_DIR)//g' | sed 's/^-l//g') #HARDCODED_MKFILE_VAR
		localIncludedLibsList+="$sharedObjNoExt "
	done

	echo "$localIncludedLibsList"

	return 0
}

# This function gets the name of all the directories to look in for a makefile
#Args: 1 - libName without ext
extractDirsToLookInFromMkFile(){
	# Vars
	local FF="${FUNCNAME[0]}"

	# Data Validation
	dirsToLookInVarInMakeFile=$(grep "DIRS_TO_LOOk_IN" "${buildLoc}$1.mk")
	if [[ $dirsToLookInVarInMakeFile =~ ^$ ]]; then
		myPerrorSh $F_LIBCHECK $FF "The variable \"DIRS_TO_LOOK_IN\" was not found in the make file \"${buildLoc}$1.mk\". The program will continue running, but errors will likely occur"
	fi
	
	#regex: remove "DIRS_TO_LOOK_IN =" from line
	dirsToLookIn=$(grep "DIRS_TO_LOOK_IN =\s\?" "${buildLoc}$1.mk" | sed 's/DIRS_TO_LOOK_IN =\s\?//g') #HARDCODED_MKFILE_VAR
	
	return 0
}

#UPDATE FILES---------------------------------------------------------------

# This function compares the last modified date of the implemenation file (.c), the associated header file (.h), & the associated obj file (.o) and recompiles/updates as needed
#Args: 1 -implementationFileDate, 2 - implementationFilePath, 3 - headerFileDate, 4 - objectFileDate, 5 - libraryName
compareAndUpdateFiles(){
	# Vars
	local FF="${FUNCNAME[0]}"
	
	# Data Validation
	if [ ! $# = 5 ]; then myPerrorSh $F_LIBCHECK $FF "Invalid number of arguments. Expected 5 arguments, received $# for file $2. Returning 1";	return 1; fi
	
	#if the implementation file is newer than the header file, update header file
	if [ $1 -ge $3 ]; then 
		#this will update both the header file and recompile the c file
		#I don't need to rebuild (run the make command) for the library in this if statement cuz if this is true, then the next if statement will run, and that contains the command
		compileHeaderImplementationFile $2
		if [ ! $? -eq 0 ]; then myPerrorSh $F_LIBCHECK $FF "Error compiling header. Returning 1"; return 1; fi
	fi

	#if the implementation or header file are newer than the associated object file, then recompile the c and header file and rebuild the makefile
	if [ $3 -gt $4 ] || [ $1 -gt $4 ]; then
		compileHeaderImplementationFile $2
		if [ ! $? -eq 0 ]; then myPerrorSh $F_LIBCHECK $FF "Error compiling header. Returning 1"; return 1; fi
		
		if [ ! $5 == "0" ]; then 
			make -f "${buildLoc}$5.mk"
			if [ ! $? -eq 0 ]; then myPerrorSh $F_LIBCHECK $FF "Error making makefile. Returning 1"; return 1; fi
		fi
	fi

	return 0
}

# This function finds the file path in a given directory based on the file type (specified in arg1)
#Args: 1 - file type, 2 - fileNameNoExt
findFilePath(){
	local FF="${FUNCNAME[0]}"
	
	if [ $1 == "-h" ]; then echo "$(find $includeLoc -name "$2.h" -type f ! -path "$backupsLoc*")";
	elif [ $1 == "-i" ]; then echo "$(find $mainFolderLoc -name "$2.c" -type f ! -path "$backupsLoc*")";
	elif [ $1 == "-o" ]; then echo "$(find $compiledHeadersLoc -name "$2.o" -type f ! -path "$backupsLoc*")";
	else myPerrorSh $F_LIBCHECK $FF "Invalid first arg. Returning -1 (using echo)"; echo "-1"
	fi
}

# This function checks if the file name appears in multiple places based on whether there is a space in the input
#Args: 1 - filePath
checkIfMultipleFiles(){
	local FF="${FUNCNAME[0]}"

	if [[ ! $(echo $1 | sed -n '/\s\+/p') =~ ^$ ]]; then #if the makeFile var contains a space, it means there are multiple elements. The file name could have a space, but I'm assuming that isn't the case
		myPerrorSh $F_LIBCHECK $FF "Multiple files were found with the same name, which might cause the program to stop"
		myPerrorSh $F_LIBCHECK $FF "	-List of files found: $1"
		myPerrorSh $F_LIBCHECK $FF "	-This error is thrown when a space is detected from the find command. If the file name contains a space, it will cause an error"
		myPerrorSh $F_LIBCHECK $FF "	-If this is the case, rename the file so it doesn't contain a space"
		return 1
	fi

	return 0
}

# This function echo's the last modified date of a file (as a number)
#Args: 1 - file path
getFileDate(){
	if [[ ! $1 =~ ^$ ]]; then
		echo $(stat -c %Y $1) #get last modified date
	else
		echo -1
	fi
}

# This function finds the last modified dates of the associated .c, .h, and .o file for each object in the library
#Args: 1 - fileNameNoExt, 2 - libNameNoExt
updateFile(){
	# Vars
	local FF="${FUNCNAME[0]}"

	# Header file info
	headerFile="$(findFilePath "-h" $1)"
	if [ "$headerFile" == "-1" ]; then myPerrorSh $F_LIBCHECK $FF "Error with findFilePath for header file. Returning 1"; return 1; fi
	checkIfMultipleFiles $headerFile
	if [ ! $? -eq 0 ]; then myPerrorSh $F_LIBCHECK $FF "Multiple files found for the header file. Aborting updating file and returning 1"; return 1; fi
	headerFileDate=$(getFileDate $headerFile)
	
	# Implementation (.c) file info
	implementationFile="$(findFilePath "-i" $1)"
	if [ "$implementationFile" == "-1" ]; then myPerrorSh $F_LIBCHECK $FF "Error with findFilePath for implementation file. Returning 1"; return 1; fi
	checkIfMultipleFiles $implementationFile
	if [ ! $? -eq 0 ]; then myPerrorSh $F_LIBCHECK $FF "Multiple files found for the implementation file. Aborting updating file and returning 1"; return 1; fi
	implementationFileDate=$(getFileDate $implementationFile)

	# Object (.o) file info
	objFile="$(findFilePath "-o" $1)"
	if [ "$objFile" == "-1" ]; then myPerrorSh $F_LIBCHECK $FF "Error with findFilePath for object file. Returning 1"; return 1; fi
	checkIfMultipleFiles $objFile
	if [ ! $? -eq 0 ]; then myPerrorSh $F_LIBCHECK $FF "Multiple files found for the implementation file. Aborting updating file and returning 1"; return 1; fi
	objFileDate=$(getFileDate $objFile)
	
	

	# Checking for missing files and updating as needed as well as notifying user of missing files
	if [[ $objFile =~ ^$ ]] && [[ ! $implementationFile =~ ^$ ]] && [[ ! $headerFile =~ ^$ ]]; then #if no object file, but a .c and .h file, then compile .o file
		compileHeaderImplementationFile $implementationFile
	elif [[ $objFile =~ ^$ ]] && [[ $implementationFile =~ ^$ ]] && [[ ! $headerFile =~ ^$ ]]; then #if no object file and no .c file, then return error and exit
		myPerrorSh $F_LIBCHECK $FF "WARNING: No object file and no implementation (\".c\") file found for $headerFile, but a header file found"
		return 0
	elif [[ $objFile =~ ^$ ]] && [[ $implementationFile =~ ^$ ]] && [[ ! $headerFile =~ ^$ ]]; then #if no object file and no header file, then create header file
		compileHeaderImplementationFile $implementationFile
	elif [[ ! $objFile =~ ^$ ]] && ( [[ $implementationFile =~ ^$ ]] || [[ $headerFile =~ ^$ ]] ); then #if object file exists, but either .c file or .h file don't exist
		myPerrorSh $F_LIBCHECK $FF "WARNING: Object file $objFile found, but either the associated implementation(\".c\") or header file couldn't be found. The program will continue with the existing object file"
		return 0
	fi
	
	#after all the necessary adjustments have been made, update files as needed	
	compareAndUpdateFiles $implementationFileDate $implementationFile $headerFileDate $objFileDate $2 #arg 2 is library name
	
	if [ ! $? -eq 0 ]; then myPerrorSh $F_LIBCHECK $FF "Error returned from compareAndUpdateFiles"; return 1; fi

	return 0
}

#END UPDATE FILES-------------------------------------------------------------------------------

# This function compares the last modified date of the library object (.so) and the library make file (.mk) and recompiles/updates as needed
#Args: 1 - libNameNoExt
compareAndUpdateMakeFile(){
	# Vars
	local FF="${FUNCNAME[0]}"
	
	# Make file info
	makeFile="${buildLoc}$1.mk"
	if [ ! -f $makeFile ]; then myPerrorSh $F_LIBCHECK $FF "Makefile ($makeFile) wasn't found. Returning 1"; return 1; fi
	makeFileDate=$(getFileDate $makeFile)

	# Shared object (.so) info 
	sharedObjFile="${libLoc}lib$1.so"
	if [ ! -f $sharedObjFile ]; then myPerrorSh $F_LIBCHECK $FF "Shared object file ($sharedObjFile) wasn't found. Returning 1"; return 1; fi
	sharedObjFileDate=$(getFileDate $sharedObjFile)

	# if the makeFile is newer than the shared object, then rebuild makefile
	if [ $makeFileDate -ge $sharedObjFileDate ]; then 
		make -f "${buildLoc}$1.mk"
		if [ ! $? -eq 0 ]; then myPerrorSh $F_LIBCHECK $FF "Error making ${buildLoc}$1.mk. Returning 1"; return 1; fi
	fi

	return 0
}

# This function looks for files in the dir and all subdirs of all values for the var "DIRS_TO_LOOK_IN" in the mk file
#Args: none
lookForNewFilesInAssociatedDirs(){
	# Vars
	local FF="${FUNCNAME[0]}"
	
	for dir in $dirsToLookIn; do
		if [ ! -f $dir ]; then myPerrorSh $F_LIBCHECK $FF "Couldn't find $dir, which was a folder to look in for ${buildLoc}$1.mk. Continuing program";
		else
			allFilesInDir="$(find $dir -type f | grep "\.c\|\.so" | sed 's/\.c/.o/g')"
			if [[ ! $allFilesInDir =~ ^$ ]]; then
				bash "$makeLibShLoc" "--no-make" $1 $allFilesInDir #HARDCODED_BASH
				myPerrorSh $? $F_LIBCHECK $FF "Error runnin makefile for $1. Continuing program"
			fi
		fi
	done
}

# This function finds all the necessary info to run a c program without updating any of the files
#Args: 1 - libName no ext
extractAllObjFilesFromLibNoUpdate(){
	# Vars
	local FF="${FUNCNAME[0]}"
	local currNumLibs=0
	local updateLibLocalIncludedLibs=""
	local updateLibObjFiles=""

	lookForNewFilesInAssociatedDirs $1
	updateLibOjbFiles="$(extractObjFilesFromMkFile $1)"

	updateLibLocalIncludedLibs+=$(getIncludedLibsFromMkFile $1)
	if [[ ! "$updateLibLocalIncludedLibs" =~ ^$ ]]; then
		for updateLibLocalIncludedLib in $updateLibLocalIncludedLibs; do
			extractAllObjFilesFromLibNoUpdate $updateLibLocalIncludedLibs
		done
	fi

			

	if [[ $(echo "$libsListForGccCommand" | grep "$updateLibLocalIncludedLibs") =~ ^$ ]]; then 
		libsListForGccCommand+="$updateLibLocalIncludedLibs "
	fi

	return 0
}

# This function updates all the files associated with a library
#Args: 1 - libName no ext
updateLib(){
	# Vars
	local FF="${FUNCNAME[0]}"
	local currNumLibs=0
	local updateLibLocalIncludedLibs=""
	local updateLibObjFiles=""

	lookForNewFilesInAssociatedDirs $1

	updateLibOjbFiles="$(extractObjFilesFromMkFile $1)"
	# For every obj in list of object from the makefile
	for obj in $updateLibOjbFiles; do
		updateFile $obj $1
		if [ ! $? -eq 0 ]; then
			myPerrorSh $? $F_LIBCHECK $FF "Error updating files associated with the object file $obj.o, which was found in the make file for $1. Returning 1"
			return 1
		fi
	done

	# Recursively update included libs in each makefile
	updateLibLocalIncludedLibs+=$(getIncludedLibsFromMkFile $1)
	if [[ ! "$updateLibLocalIncludedLibs" =~ ^$ ]]; then
		for updateLibLocalIncludedLib in $updateLibLocalIncludedLibs; do
			updateLib $updateLibLocalIncludedLib
			if [ ! $? -eq 0 ]; then
				myPerrorSh $F_LIBCHECK $FF "Error updating included library $includedLib, which was in included in $1.mk. Returning 1"
				return 1
			fi
		done
		

	fi
	
	# Add to libs list for use in execC.sh
	if [[ $(echo "$libsListForGccCommand" | grep "$updateLibLocalIncludedLibs") =~ ^$ ]]; then 
		libsListForGccCommand+="$updateLibLocalIncludedLibs "
	fi

	
	return 0
}

# This function combines the functions above as well as the functions in "createHeader.sh" to update files as necessary
#Args: file name to compile
updateIncludes(){
	# Vars
	local FF="${FUNCNAME[0]}"

	# Loading Bar
	#Putting it here instead of in execC cuz this function and file wouldn't work if it were run in the background
	#Instead, I'm running loadingBar() in background and sending an end signal to loading bar at the end of this function
	loadingBar $BASHPID 1 "Checking to see if any files associated with the included statement(s) need to be updated. Progress:" &
	
	extractNonStandardIncludes $1
	if [ ! $? -eq 0 ]; then 
		myPerrorSh $F_LIBCHECK $FF "Error extracting non standard includes. Returning 1";
		return 1;
	fi
	
	findCorrespondingLib
	findCorrespondingNonLibHeaderFile
	
	# For every library, update the files inside of it
	for lib in $libs; do
		updateLib $lib
		if [ ! $? -eq 0 ]; then
			myPerrorSh $F_LIBCHECK $FF "updateLib() returned an error. Returning 1"
			echo "stop" >&2 #send kill signal to loadingBar()
			wait #wait for loadingBar() to finish
			return 1
		fi

		compareAndUpdateMakeFile $lib
		if [ ! $? -eq 0 ]; then
			myPerrorSh $F_LIBCHECK $FF "Error comparing and updating makefile for the library $lib. Returning 1"
			echo "stop" >&2 #send kill signal to loadingBar()
			wait #wait for loadingBar() to finish
			return 1
		fi
	done

	# For every non-library header file, update the associated files
	for nonLibHeader in $nonLibsHeaders; do
		nonLibHeaderAdjusted="$(echo $nonLibHeader | sed 's/\.o//g')"
		updateFile $nonLibHeaderAdjusted "0"
		if [ ! $? -eq 0 ]; then
			myPerrorSh $F_LIBCHECK $FF "Error updating included non-library header $nonLibHeaderAdjusted. Returning 1"
			echo "stop" >&2 #send kill signal to loadingBar()
			wait #wait for loadingBar() to finish
			return 1
		fi
	done
	
	echo "stop" >&2 #send kill signal to loadingBar()
	wait #wait for loadingBar() to finish
	return 0

}





#-------------------------

#lookForNewFilesInAssociatedDirs <-- defined here, used later on. Aka, don't actually use it before "updateLib"
#	-extract all dirs in makefile from DIRS_TO_LOOK_IN variable
#	-check to make sure folder exists
#	-find all .c, .o, and .so files in dirs and subdirs and combine into 1 string
#		-if .c file, replace with .o. Don't need to check for header file or the existence of .o file. Other functions below take care of that
#	-run makeLib with the string above

#updateFiles $1 $2 -DONE
#	-get dates of .c file
#	-get dates of .o file
#	-get dates of .h file
#	-check for missing files and recompile as necessary
#		-update dates as needed
#	-update as needed


#updateLib 
#	-get makeFile name and path
#	-lookForNewFilesInAssociatedDirs
#	-extract all .o files from makefile
#	-for every .o file (in objFilesList):
#		-update file $file $lib
#	-extract .so files
#	-for each .so file (use for loop due to limited stack for recursion in bash)
#		-extract libName from $1
#		-updateLib

#updateIncludes
#	extract nonstandard includes
#	find correspondingLibOrHeaderFile
#	For every non-standard lib in program
#		-updateLib $potentialLib
#		-check if makefile was updated more recently than .so file. If so, run make -f ...
#	update non-lib headers








