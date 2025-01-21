#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
filesAndFoldersLoc="$(findPath -f 151)" #filesAndFolders.csv loc
symLinkDirLoc="$(findPath -f 315)/" #symLinks dir
sourcedShellScripts="$sharedScripts"

colNumSymLinkNum=9 #HARDCODED_COL_NUM
colNumFilePath=3 #HARDCODED_COL_NUM

# Error Message Constant Vars
F_CREATSLFROMCSV="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# SOURCING-----------------------------------------------------------------------------------------
source "$sharedScripts"
checkFileLocs $F_CREATSLFROMCSV
#--------------------------------------------------------------------------------------------------

# This function creates all the necessary symlinks based on the values in filesAndFolders.csv
#Args: none
createSymLinksFromCsv(){
	# Vars
	local FF="${FUNCNAME[0]}"
	adjustedColNumSymLinkNum=$(expr $colNumSymLinkNum + 1)
	numSymLinksCreated=0
	numSymLinksUpdated=0

	# Getting Values
	filesAndFoldersAsStr="$(cat "$filesAndFoldersLoc" | sed '1d')" #removing header row
	
	# Iterating over every row and creating a symlink if symLinkNum col in csv is not 0
	for row in $filesAndFoldersAsStr; do
		currSymLinkNum="$(echo "$row" | cut -d ',' -f $adjustedColNumSymLinkNum)"

		if [ ! $currSymLinkNum == "0" ]; then
			#getting target path from that row in the csv
			targetPath="$(echo "$row" | cut -d ',' -f $(expr $colNumFilePath + 1))"

			# Values to keep track. Help in debugging
			if [ -e "${symLinkDirLoc}symLink_$currSymLinkNum.sl" ]; then
				numSymLinksUpdated=$(expr $numSymLinksUpdated + 1);
			else 
				numSymLinksCreated=$(expr $numSymLinksCreated + 1);
			fi
			
			ln -sf "$targetPath" "${symLinkDirLoc}symLink_$currSymLinkNum" #creating symlink
			if [ ! $? == 0 ]; then
				myPerrorSh $F_CREATSLFROMCSV $FF "\"ln -sf\" command failed for $targetPath, which has symlink number $currSymLinkNum. Returning -1"
				return -1
			fi
		fi
	done

	echo "Number of symlinks created (didn't previously exist): $numSymLinksCreated"
	echo "Number of symlinks updated: $numSymLinksUpdated"
}

# Help Menu
helpMenu_CreateSlFromCsv(){
	echo "+++CREATE SYMLINKS FROM CSV - HELP MENU+++	Relative File Path: $0"
	echo "Purpose: create or update symlinks for files with non-zero symlink numbers in filesAndFolders.csv"
	echo "Options: none"
	echo "Usage:"
	echo "	<binNameOrAlias>"
}

# Main
main(){
	# if no args, then run normally. Would only have an arg for help menu
	if [ -z $1 ]; then createSymLinksFromCsv; return $!; fi
	helpMenu_CreateSlFromCsv
	return -1;
}

main $@
