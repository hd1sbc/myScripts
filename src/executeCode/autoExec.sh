#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
listOfAcceptedFileTypes="bash c c++ c# java perl python ruby"
listOfAcceptedExt=".sh .c .cpp .cs .java .pl .py .rb"
execCFlags=0

#File Locations
compiledCppLoc="$(findPath -f 488)/" #compiledCPP
compiledJavaLoc="$(findPath -f 489)/" #compiledJava
fileTypeLoc="$(findPath -f 350)" #fileType.py
execCLoc="$(findPath -f 336)" #execC.sh
sourcedShellScriptsLoc="$sharedScripts" #allMySharedShScripts.sh"

# Error Message Constant Vars
F_AUTOEXEC="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# SOURCING-----------------------------------------------------------------------------------------
source "$sharedScripts"
checkFileLocs $F_AUTOEXEC
#--------------------------------------------------------------------------------------------------

# Help Menu
helpMenuAutoExec(){
	echo "+++AUTO EXECUTE - HELP MENU+++	Relative File Path: $0"
	echo "Purpose: compile and execute a variety of different programming languages" 
	echo "Details: the program will compile and run based on the file extension"
	echo "	Accepted file types are: $listOfAcceptedFileTypes"
	echo "	Accepted extensions are: $listOfAcceptedExt"
	echo "Options: "
	echo "	-u, --no-auto-update		:- turns off auto-update for libraries and header files for c programs ONLY"
	echo "	--special=<type>		:- compiles and/or executes in a special way based on type"
	echo "		type options: "
	echo "		-overwrittenSysCalls	:- special compilation (and no execution) for overwrittenSysCalls.c file"
	echo "Usage: "
	echo "	<binNameOrAlias> <fileName> [args for function]"
	echo "Examples: "
	echo "	<binNameOrAlias> fileName.sh		:- this will execute a bash file"
	echo "	<binNameOrAlias> fileName.c arg1 arg2	:- this will execute a c program with its arguments"
	echo "	<binNameOrAlias> -u fileName.c arg1 	:- this will execute a c program with its arguments without updating libraries or header files"
	echo "Notes:"
	echo "	-C programs contain a far more sophisticated process, the code for which can be found across a number of files in the same folder as this program ($0)"
		
	return 0
}

# Main function
mainAutoExec(){
	FF="${FUNCNAME[0]}"

	# Data Validation
	if [ $# -eq 0 ]; then
		myPerrorSh $F_AUTOEXEC $FF "Invalid num of args. 0 were provided. At least 1 is required. Use -h or --help option for more details"
		return 1
	fi
	
	if [ $(isHelpSh $1) -eq 1 ]; then helpMenuAutoExec; return 1; fi
	
	# execC has a flag at the beggining to denote NO auto-update/check files
	if [ $1 == "-u" ] || [ $1 == "--no-auto-update" ]; then execCFlags=1; shift 1;
	elif [ $1 == "--special=overwrittenSysCalls" ]; then execCFlags=2; shift 1;
	fi

	fileType=$(python3 $fileTypeLoc $1)

	# Data Validation
	if [[ $fileType =~ ^$ ]]; then
		myPerrorSh $F_AUTOEXEC $FF "No extension provided. Aborting"
		return 1
	fi


	if [ -z $fileType ]; then
		myPerrorSh $F_AUTOEXEC $FF "File doesn't exist or file type (based on extension) is not supported"
	elif [ $fileType == "sh" ]; then
		bash $@
	elif [ $fileType == "c" ]; then
		bash $execCLoc $execCFlags $@ #HARDCODED_BASH
	elif [ $fileType == "pl" ]; then
		perl $@
	elif [ $fileType == "rb" ]; then
		ruby $@
	elif [ $fileType == "cpp" ]; then
		args=$(echo $@ | sed 's/^[^ ]*//; s/[[:space:]]*//') #getting the arguments, excluding the name of the file
		fileObjName=$(echo $1 | sed 's/.*\///g' | sed 's/\..*/\.o/g')
		fileObjPath=$compiledCppLoc$fileObjName
		gcc $1 -o $fileObjPath
		$fileObjPath $args
	elif [ $fileType == "py" ]; then
		python3 $@
	elif [ $fileType == "java" ]; then
		args=$(echo $@ | cut -d ' ' -f2-) #getting the arguments, excluding the name of the file
		javac $1 -d $compiledJavaLoc
		java $compiledJavaLoc $args
	elif [ $fileType == "cs" ]; then
		if [ $($(dotnet run &>/dev/null) | grep -q 'project') ]; then
			myPerrorSh $F_AUTOEXEC $FF "C# error occurred regarding the incompatability in compiling the file. Make sure you created a console and are executing the correct file."
			myPerrorSh $F_AUTOEXEC $FF "	Review the genFile code to see the steps to create a folder and file that is executable by the .NET framework"
		else	
			cd .. # cd up one dir because if user is running this command while in the project folder, it can often cause errors.
			dotnet run --project "$(pwd)/$(echo $1 | sed 's/\..*//g')" #removing the file extension from argument
		fi
	else
		myPerrorSh $F_AUTOEXEC $FF "Invalid extension $fileType. Accepted extensions are:"
		myPerrorSh $F_AUTOEXEC $FF "	$listOfAcceptedExt"
		myPerrorSh $F_AUTOEXEC $FF "	Use -h or --help option for details" 
	fi
}

mainAutoExec $@
