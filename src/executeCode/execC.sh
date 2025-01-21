#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
sharedObjList=""
execCFlags=1

#File Locations
compiledCLoc="$(findPath -f 21)/" #compiledC dir
libLoc="$(findPath -f 2)/" #lib dir
includeLoc="$(findPath -f 120)/" #include dir
executeCodeLoc="$(findPath -f 109)/" #execute code dir
sourcedShellScriptsLoc="$sharedScripts"
symLinkLoc="$(findPath -f 146)/" #symLinks dir
libCheckLoc="$(findPath -f 330)" #libraryCheck.sh

# Error Message Constant Vars
F_EXECC="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# SOURCING-----------------------------------------------------------------------------------------
source "$sharedScripts"
checkFileLocs $F_EXECC
#this file also indirectly sources "createCHeader.sh" cuz "libraryCheck.sh" (the file below) sources it
source "$libCheckLoc"
#--------------------------------------------------------------------------------------------------

# This function determines if the file (passed as an arg) includes non-standard libs (my libraries/libs not part of standard C library)
#Args: file name of the file to be compiled 
hasNonStandardLibs(){
	local FF="${FUNCNAME[0]}"

	extractNonStandardIncludes $1 #function from "makeLibrary.sh"
	if [ ! $? -eq 0 ]; then
		myPerror $F_EXECC $FF "Error running extractNonStandardIncludes() function. Returning 2"
		return 2
	elif [[ $potentialLibs =~ ^$ ]]; then
		return 0
	else
		return 1
	fi
}

# This function add "-l" before each library name so that I can just include the var "libsListInGcc..." in the compile command in main() for complex programs
#Args: 1 - libs
convertLibsToGccFormat(){
	# Vars
	libsListInGccFormatString=""
		
	for lib in $@; do #libs var is from "libraryCheck.sh"
		libsListInGccFormatString+="-l$lib "
	done

	echo "$libsListInGccFormatString"
}

# This function determines if the file header of the corresponding header potential library exists in include dir
#Args: 1 - potentialLibs
fileHeaderExists(){
	# Vars
	local FF="${FUNCNAME[0]}"
	
	for lib in $1; do
		if [ ! -f "${includeLoc}$lib.h" ]; then
			myPerrorSh $F_EXECC $FF	"$lib doesn't exist in $includeLoc. Echo-ing 0"
			echo 0
			return 0
		fi
	done

	echo 1
	return 0
}

# This function runs a simple c program with no non-standard headers or libraries
#Args: 1 - file name, 2 - fileObjPath, 3 - args
runSimpleC(){
	# Vars. Converting them to names so it is easier to understand
	fileObjPathIn=$2
	argsIn=$3
	local FF="${FUNCNAME[0]}"

	gcc -Wall -g $1 -o $fileObjPathIn
	if [ $? -eq 0 ]; then #checking if compilation was successful
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes .$fileObjPathIn $argsIn
	else
		myPerrorSh $F_EXECC $FF "Compilation failed, aborting execution"
		return 1
	fi
}

# This function runs a c program with non-standard headers, but no libraries
#Args: 1 - file name, 2 - fileObjPath, 3 - args
runComplexCNoLibs(){
	# Vars. Converting them to names so it is easier to understand
	fileObjPathIn=$2
	argsIn=$3
	local FF="${FUNCNAME[0]}"

	fileHeaderExistsBool=$(fileHeaderExists $potentialLibs)
	
	if [ $fileHeaderExistsBool -eq 1 ]; then # Checking to make sure the file header exists
		#compile with necessary include location and any object files associated with non-library headers. The nonLibraryHeader... var is from "libraryCheck.sh"
		#remove -Wl,--no-as-needed to see more linking info
		gcc -Wl,--no-as-needed -Wall -g $1 $nonLibraryHeaderObjFilePaths -o $fileObjPathIn -I$includeLoc -I$symLinkLoc

		if [ $? -eq 0 ]; then
			valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $fileObjPathIn $argsIn
		else
			myPerrorSh $F_EXECC $FF "Compilation failed, aborting execution"
			return 1
		fi
	else
		myPerrorSh $F_EXECC $FF "File header doesn't exist in $includeLoc. Aborting"
		return 1
	fi
}

# This function runs c program with non-standard libraries in which at least one points to a library
#Args: all
runComplexCLibs(){
	# Vars. Converting them to names so it is easier to understand
	fileObjPathIn=$2
	argsIn=$3
	local FF="${FUNCNAME[0]}"

	libsListInGccFormatString="$(convertLibsToGccFormat $libsListForGccCommand)"

	# This is for compiling newCFuncs. Make sure to comment out valgrind line below
	if [ $execCFlags -eq 2 ]; then
		#remove -Wl,--no-as-needed to see more linking info
		gcc -Wl,--no-as-needed -Wall -g $1 $nonLibraryHeaderObjFilePaths -o $fileObjPathIn -L$libLoc $libsListInGccFormatString -I$includeLoc -I$symLinkLoc -Wl,-rpath=$libLoc -ldl -shared -fPIC #compile with necessary lib and include locations
		if [ ! $? -eq 0 ]; then myPerrorSh $F_EXECC $FF "Compilation failed. Returning 1"; return 1; fi
		return 0
	fi

	# Normal compilation
	#remove -Wl,--no-as-needed to see more linking info
	gcc -Wl,--no-as-needed -Wall -g $1 $nonLibraryHeaderObjFilePaths -o $fileObjPathIn -L$libLoc $libsListInGccFormatString -I$includeLoc -I$symLinkLoc -Wl,-rpath=$libLoc #compile with necessary lib and include locations

	if [ $? -eq 0 ]; then
		export LD_LIBRARY_PATH=$libLoc
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes $fileObjPathIn $args
		# Use below to check for num of function calls for each function. After running autoExec, see output with command "callgrind_annotate callgrind.out.<pid>"
		#valgrind --tool=callgrind $fileObjPathIn $args
	else
		myPerrorSh $F_EXECC $FF "Compilation failed, aborting execution"
		return 1
	fi
}

# This runs simple and complex c files so long as they have a main function (meaning they are not a header file implementation)
#Args: all
runC(){
	# Vars
	local FF="${FUNCNAME[0]}"

	#getting the arguments, excluding the name of the file
	#	Doing this cuz getting name of file in c (arg 0) can cause issues with this program
	#	I'm executing this program within the scope of the script, so arg 0 would be the name of this script
	#	By extracting the args, I avoid this issue
	#regex: remove everything up to (and including) first space. Remove leading spaces
	args=$(echo $@ | sed 's/^[^ ]*//; s/[[:space:]]*//') 
	


	#regex: remove everything after first space (to take the first arg). Remove everything before the last '/'. Replace extension with '.o'
	fileObjName=$(echo $1 | sed 's/^ .*//g' | sed 's/.*\///g' | sed 's/\..*/\.o/g')
	fileObjPath="$compiledCLoc$fileObjName"
	
	hasNonStandardLibs $1
	hasNonStandardLibsBool=$? #captures the return from the statement above
	if [ $hasNonStandardLibsBool -eq 2 ]; then
		myPerrorSh $F_EXECC $FF "Error with hasNonStandardLibs() function. Returnin 1"
		return 1
	fi

	# Separating "simple" c programs (those without any libs that I built) from "complex" ones
	# 	-the reasoning here is that I want to be able to automatically run simple programs without dealing with the complexities of libraries
	if [ $hasNonStandardLibsBool -eq 0 ]; then #simple program
		runSimpleC $1 $fileObjPath $args
	else #complex program
		
		# Running the appropriate functions based on execCFlags
		if [ $execCFlags -eq 0 ]; then #if no flags preventing checking/updating files
			updateIncludes $1 2>&4 #updating object files and libs as needed. Function from "libraryCheck.sh"
			if [ ! $? -eq 0 ]; then myPerrorSh $F_EXECC $FF "Error updating library and headers. Aborting"; return 1; fi
		else
			extractNonStandardIncludes $1
			findCorrespondingLib
			findCorrespondingNonLibHeaderFile
			
			for lib in $libs; do
				extractAllObjFilesFromLibNoUpdate $lib
			done			
		fi


		# if has a non-standard include statement(s), but none of them point to a library header file, then don't include any libraries
		if [[ $libs =~ ^$ ]]; then
			runComplexCNoLibs $1 $fileObjPath $args
			if [ ! $? -eq 0 ]; then myPerrorSh $F_EXECC $FF "Error running complex c file with no libraries"; return 1; fi
		else #if has a non-standard include statement that points to a library header file, then compile & run with library
			runComplexCLibs $1 $fileObjPath $args
			if [ ! $? -eq 0 ]; then myPerrorSh $F_EXECC $FF "Error running complex c file with libraries"; return 1; fi
		fi
	fi

	return 0
}

# This function runs any .c file
#Args: all
execCMain(){
	# Vars
	local FF="${FUNCNAME[0]}"

	execCFlags=$1
	shift 1
	
	# Data Validation
	if [ ! -f $1 ]; then #if file doesn't exist
		myPerrorSh $F_EXECC $FF "File doesn't exist. Aborting compilation and execution"
		return 1
	fi

	isHeaderFile=$(isSourceCodeForHeaderFile $1) #function from "createCHeader.sh"
	if [ $isHeaderFile -eq 1 ]; then #if a header file, then compile the header file. The var "isHeaderFile" is from "createCHeader.sh"
		if [ $execCFlags -eq 0 ]; then #if no flags preventing checking/updating files
			extractNonStandardIncludes $1
			findCorrespondingLib
			findCorrespondingNonLibHeaderFile
			for lib in $libs; do extractAllObjFilesFromLibNoUpdate $lib; done			

			libsListInGccFormatString="$(convertLibsToGccFormat $libsListForGccCommand)"
			
			compileHeaderImplementationFile $1 #function from "createCHeader.sh"
			returnStatus=$?
			myPerrorSh $returnStatus $F_EXECC $FF "Error compiling header file"
			return $returnStatus
		else
			updateIncludes $1 2>&4 #updating object files and libs as needed. Function from "libraryCheck.sh"
			if [ ! $? -eq 0 ]; then myPerrorSh $F_EXECC $FF "Error updating library and headers. Aborting"; return 1; fi
			compileImplementationFile $1 #just compile the implementation file. Function from "createCHeader.sh"
		fi
	else #if an executable c file (i.e. it has a main function)
		runC $@
		returnStatus=$?
		myPerrorSh $returnStatus $F_EXECC $FF "Error compiling running c file"
	fi

	return 0
}

execCMain $@

