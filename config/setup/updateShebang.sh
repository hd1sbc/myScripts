#!/bin/bash
# Default shebang is bash, but will be updated accordingly by the linuxUtilsSetup.sh file

#----------------------------------------------------------
#There are two options here: setup and checkAll
#setup :- this mode is for when setting up during installation. It assumes the shebangs are already correct (default bash) and will only update files if bash doesn't exist on system
#checkAll :- this mode is for checking all files for correct shebang.
#	If I'm editing the code on a system that doesn't have bash, the shebangs will have been updated from this file (and linuxUtilsSetup.sh) during installation. If I upload these files,
#	the shebangs will not be the default bash (#!/bin/bash). This option checks for that and updates them as needed. Since this requires going through all the files, it can take a lot
#	longer. That is why this is an option
#----------------------------------------------------------

newShell="bash" #setting the default

changeShebang () {
	i=0
	for row in ~/myScripts/dep/fileAndFolderInfo/files.csv; do
		if [ ! $i -eq 0 ]; then #if not the first row, because it is a header row/contains the column names
			filePath=$(echo $row | cut -d ',' -f 1) #for the current row/line, take the first field (full path)
			fileExt=$(echo $filePath | sed 's/.*\.//g') #removes everything before the last "." to get the file extension
			fileName=$(echo $filePath | sed 's/.*\//g') #remove everything before the last "/" to get just the file name
			
			if [[ $fileExt =~ sh ]] && [[ ! $fileName =~ INSTALL ]]; then #if a shell script (extention is .sh) and not INSTALL.sh script
				sed -i "1s/.*/$newShell/" $filePath #replacing the first line with new shebang
			fi
		fi

		i=$(expr $i + 1) #incrementing i in posix-compatible manner
	done
}

main () {
	#if the current/default shell isn't bash and the bash shell isn't on system
	if [[ ! $(echo $SHELL) =~ bash ]] && [[ ! $(which bash) =~ bin ]]; then 
		newShell="#\!$(echo $SHELL | sed 's/\//\\\//g')" #finding new shell path, adding backslashes to make it compatible with sed (for later in the script)
	fi

	if [[ $1 =~ ^setup$ ]] && [[ ! $newShell =~ bash ]]; then #if arg1 = setup and shell isn't bash
		changeShebang
	elif [[ $1 =~ ^checkAll$ ]]; then
		changeShebang
	fi
}

main
