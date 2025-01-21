#!/bin/bash

# Checking if zenity is installed. If not, echo error return val ("x") and return -1
if [ "$(which zenity)" == "" ]; then
	echo "x"
	exit -1
fi

# This function opens a warning when user is trying to open a write-protected file
#Args: 1 - file name
openWarning(){
	warningText="WARNING: Modifying $1 could lead to unintended consequences and break other programs.\n\nAre you sure you want to continue?"
	if zenity --question --title="Opening Write-Protected File" --text="$warningText" 2>/dev/null; then
		echo "yes"
	else
		echo "no"
	fi
}

# This function opens a warning when user is trying to rename (but not move to a different dir) a rename-protected file
#Args: 1 - file name
renameWarning(){
	warningText="WARNING: Renaming $1 could lead to unintended consequences and break other programs.\n\nAre you sure you want to continue?"
	if zenity --question --title="Renaming Rename-Protected File" --text="$warningText" 2>/dev/null; then
		echo "yes"
	else
		echo "no"
	fi
}

# This function opens a warning when user is trying to move a move-protected file to a different dir
#Args: 1 - file name
moveWarning(){
	warningText="WARNING: Moving $1 to a different directory could lead to unintended consequences and break other programs.\n\nAre you sure you want to continue?"
	if zenity --question --title="Moving Move-Protected File" --text="$warningText" 2>/dev/null; then
		echo "yes"
	else
		echo "no"
	fi
}

# This function opens a warning when user is trying to rename a file to an already existing file path
#Args: 1 - file name
renameOverExistingFileWarning(){
	warningText="WARNING: Renaming a file to a an existing file path: $1. Renaming this file will delete the contents of the original file.\n\nAre you sure you want to continue?"
	if zenity --question --title="Renaming To An Existing Path" --text="$warningText" 2>/dev/null; then
		echo "yes"
	else
		echo "no"
	fi
}

# This function opens a warning when user is trying to delete a delete-protected file
#Args: 1 - file name
deleteWarning(){
	warningText="WARNING: Deleting $1 could lead to unintended consequences and break other programs.\n\nAre you sure you want to continue?"
	if zenity --question --title="Deleting Delete-Protected File" --text="$warningText" 2>/dev/null; then
		echo "yes"
	else
		echo "no"
	fi
}

# Main
main(){
	if [ $1 == "-o" ]; then
		openWarning $2
	elif [ $1 == "-r" ]; then
		renameWarning $2
	elif [ $1 == "-m" ]; then
		moveWarning $2
	elif [ $1 == "-ro" ]; then
		renameOverExistingFileWarning $2
	elif [ $1 == "-d" ]; then
		deleteWarning $2
	else
		echo "===${BASH_SOURCE[@]} - Main===	Invalid option"
	fi
}

main $@
