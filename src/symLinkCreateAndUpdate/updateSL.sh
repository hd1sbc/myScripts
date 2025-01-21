#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
filesAndFoldersCsvLoc="$(findPath -f 79)" #filesAndFolders.csv
filesAndFoldersInfoDirLoc="$(findPath -f 76)/" #fileAndFolderInfo dir
symLinksFolderLoc="$(findPath -f 146)/" #symLinks dir
sourcedShellScripts="$sharedScripts"

fileNames=""
filePaths=""
filesToCheck=""
fileIds=""
rowNum=0
createSymLinkBool=0

fileIdColNum=1 #HARDCODED_COL_NUM
filePathsColNum=4 #HARDCODED_COL_NUM
fileNamesColNum=5 #HARDCODED_COL_NUM
symLinkNumColNum=9 #HARDCODED_COL_NUM

# Error Message Constant Vars
F_CHANGESL="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# SOURCING-----------------------------------------------------------------------------------------
source "$sharedScripts"
checkFileLocs $F_CHANGESL
#--------------------------------------------------------------------------------------------------

# This function determines if the 1st arg to program is a file id, file name, or file path. Echo's 2, 0, or 1 respectively
#Args: 1 - key in csv (arg 1 to program. Key could be file id, name, or path)
getColType(){
	# Vars
	local FF="${FUNCNAME[0]}"
	firstArgAsStr="$1"

	if [ $(isPosIntSh $1) -eq 1 ] && [ $((firstArgAsStr)) -gt 0 ]; then echo 2; return 0; #if is a positive int > 0
	elif [ $(isPosIntSh $1) -eq 1 ] && [ $((firstArgAsStr)) -le 0 ]; then #if is a positive int <= 0, throw error
		myPerrorSh $F_CHANGESL $FF "Invalid 1st arg; value is less than or equal to 0. Echoing -1"
		echo -1
	elif [ $(regexMatchSh "\\/" $1) -eq 0 ]; then echo 0; return 0; #if doesn't contains '/' (denoting a file path)
	elif [ $(regexMatchSh "\\/" $1) -eq 1 ]; then echo 1; return 0 #if contains '/' (denoting a file path)
	else
		myPerrorSh $F_CHANGESL $FF "Invalid 1st arg. Echoing -1"
		echo -1
	fi

	return 0
}

# This function checks for duplicate keys (arg 1 to program. Key could be file id, name, or path)
#Args: 1 - key in csv (arg 1 to program. Key could be file id, name, or path), 2 - column type
checkForDupesInCsv(){
	# Vars
	local FF="${FUNCNAME[0]}"
	filesWithSameNameBool=-1
	listOfMatchingFiles=""
	
	# Determining which col to use to find key (arg 1 to program). Key is an id, file name, or file path
	if [ $2 -eq 0 ]; then filesToCheck="$fileNames";
	elif [ $2 -eq 1 ]; then filesToCheck="$filePaths";
	elif [ $2 -eq 2 ]; then filesToCheck="$fileIds";
	else myPerrorSh $F_CHANGESL $FF "Invalid 2nd arg; value is not 0, 1, or 2. Returning 2"; return 2; fi

	# Iterating over every row in column (where the column would be where to find key)
	for file in "$filesToCheck"; do
		if [ "$file" == "$1" ]; then #if match
			#if first match, set var to 0. If 2nd match (var already = 0), set var to 1
			if [ $filesWithSameNameBool -eq -1 ]; then filesWithSameNameBool=0; else filesWithSameNameBool=1; fi
			listOfMatchingFiles+="$file " #add to list of matching files
		fi
	done

	# Details on duplicates
	if [ $filesWithSameNameBool -gt 0 ]; then
		myPerrorSh $F_CHANGESL $FF "Duplicate files found for $1. Returning 1"
		myPerrorSh $F_CHANGESL $FF "	Duplicate files found are: $listOfMatchingFiles"
		return 1
	fi

	return 0
}

# This function gets the row from the csv with the matching key (id, name, or path)
#Args: 1 - key in csv (arg 1 to program). Key could be file id, name, or path
getFileRow(){
	# Vars
	local FF="${FUNCNAME[0]}"
	fileRow=""
	#starting at 2 cuz row 1 is header row
	#while filesToCheck var removes header row, rowNum is used when looking in csv file where header row still exists
	rowNum=2
	matchFound=0

	# Iterating over every row from the csv
	# 	-filesToCheck var is the col to find the match from arg 1
	# 	-input to program could be a num, file name, or file path. filesToCheck contains the columns based on the type of input
	for file in $filesToCheck; do
		if [ "$file" == "$1" ]; then #if a match
			fileRow="$(sed -n "${rowNum}p" "$filesAndFoldersCsvLoc")" #get whole row in csv at rowNum
			matchFound=1
			break
		fi
		rowNum=$(expr $rowNum + 1)
	done

	if [ $matchFound -eq 0 ]; then
		myPerrorSh $F_CHANGESL $FF "No matches found for arg 1 to program ($1). File wasn't in the files and folders csv. Returning empty string"
	fi

	echo "$fileRow"
	echo "$rowNum"
	return 0
}

# This function extracts the symlink number at the file row before the file row is updated
#Args: 1 - file row
extractPrevSymLinkNum(){
	#the col num is designed for indexing (i.e. it assumes counting starts at 0). Since cut commands starts at 1, need to adjust for that
	symLinkColNumAdjusted=$(expr $symLinkNumColNum + 1)
	echo $(echo $1 | cut -d ',' -f $symLinkColNumAdjusted)
}


# This function updates the row to have the updated symlink number
#Args: 1 - file row, 2 - new symlink number
changeSymLinkNum(){
	# Converting file row string to array. Doing this so I can access the right column to change the value
	IFS=','
	read -r -a rowAsArr <<< "$1"
	unset IFS

	rowAsArr[$symLinkNumColNum]="$2" #changing the symlink num at the correct index

	# Converting back to string
	newRow=""
	for col in "${rowAsArr[@]}"; do
		newRow+="$col,"
	done
	newRow="$(echo "$newRow" | sed 's|,$||g')" #removing trailing comma

	echo "$newRow"
}

# This function updates the associated files and dependencies if the new symlink number is not 0
#Args: 1 - new file row, 2 - previous symlink number, 3 - new symlink number
updateCorrespondingFiles(){
	# Vars
	local FF="${FUNCNAME[0]}"
	foundOccurrences=0

	# Updating the row in files and folders csv by replacing the current row with the new one
	sed -i.bak "${rowNum} s|.*|$1|" "$filesAndFoldersCsvLoc"
	echo "Updated files and folders csv with the new symlink number ($3)"

	# Updating all files in files and folders csv that previously had referenced the old symlink number
	echo "Updating all files in files and folders csv that previously had referenced the old symlink number ($2)"
	for path in $filePaths; do #for every file in csv
		#if path not in filesAndFoldersInfo dir (don't want to update symlink details in csv) and the file exists
		if [[ ! "$path" =~ "$filesAndFoldersInfoDirLoc" ]] && [ -f "$path" ] &&
				#and the file isn't a swap file/backup (often contain null bytes that mess up sed command
				[[ ! "$path" =~ ".swp" ]] &&
				#and the file contains the old symlink value
				[[ ! "$(sed -n "/symLink_$2.sl/p" "$path")" =~ ^$ ]]; then
		
			# Getting replacement details
			#regex: print each match in the file on a new line. Count the number of lines
			numOccurrences="$(grep -o "symLink_$2.sl" "$path" | wc -l)"
			#regex: get the line num of each occurrence. Remove excess stuff to just get line num. Replace newline chars with commas. Remove trailing comma
			lineNumsOfOccurrences="$(grep -n "symLink_$2.sl" "$path" | cut -d ':' -f 1 | tr '\n' ',' | sed 's/,$//g')"
			
			# Updating all occurrences in the file and printing details
			# 	-using sudo in case files have different permissions
			#regex: replace all instances of old symlink with new one in the file
			sudo sed -i "s|symLink_$2.sl|symLink_$3.sl|g" "$path"
			
			#printing replacement details
			echo "Updated $numOccurrences occurrences in $path"
			echo "	The occurrences were found on lines: $lineNumsOfOccurrences"
			
			foundOccurrences=1
		fi
	done

	#if no occurrences, mention it
	if [ $foundOccurrences -eq 0 ]; then echo "No occurences in other files found. No files updated"; fi

	# Removing old symlink if the old symlink exists
	if [ -e "${symLinksFolderLoc}symLink_$2.sl" ] && [ ! $2 -eq 0 ]; then
		prevVal="$(readlink "${symLinksFolderLoc}symLink_$2.sl")"
		rm "${symLinksFolderLoc}symLink_$2.sl"
		echo "Program removed the old symlink (symLink_$2.sl)"
	else
		myPerrorSh $F_CHANGESL $FF "WARNING: symLink_$2.sl wasn't found in $symLinksFolderLoc. Couldn't remove old symlink"
	fi

	# Creating/updating the new symlink
	filePath="$(echo $1 | cut -d ',' -f $filePathsColNum)" #getting the file path
	#if the symlink points to a dir, then delete it. ln -sf command doesn't work if the symlink points to a dir
	if [ -d "$(readlink "${symLinksFolderLoc}symLink_$3.sl")" ]; then rm "${symLinksFolderLoc}symLink_$3.sl"; fi
	ln -sf "$filePath" "${symLinksFolderLoc}symLink_$3.sl" #updating symlink
	# Symlink updating/creation details
	if [ ! -e "${symLinksFolderLoc}symLink_$3.sl" ]; then
		echo "Symlink file symLink_$3.sl didn't exist. Created it and set its target file to $filePath"
	else
		echo "Updated target file at symLink_$3.sl"
	fi


	return 0
}

# This function updates the associated files and dependencies if the new symlink number is 0
#Args: 1 - new row, 2 - previous symlink number, 3 - new symlink number
updateSymLinkNumZero(){
	# Vars
	local FF="${FUNCNAME[0]}"

	# Updating the row in files and folders csv by replacing the current row with the new one
	sed -i.bak "${rowNum} s|.*|$1|" "$filesAndFoldersCsvLoc"
	
	# Explaining why program doesn't update dependencies, but only if changing symlink not creating it
	if [ $createSymLinkBool -eq 0 ]; then
		echo "Updated files and folders csv with the new symlink number $3"
		echo "New symlink number is 0; not updating dependencies"
		echo "	symLink_0.sl points to /dev/null, and a lot of files point to that."
		echo "	In order to protect against updating the wrong files, program is not updating all the dependencies"
		echo "	Program is just setting the old symlink (symLink_$2.sl) to /dev/null"
	fi

	# Removing old symlink if the old symlink exists
	if [ -e "${symLinksFolderLoc}symLink_$2.sl" ] && [ ! $2 -eq 0 ]; then
		prevVal="$(readlink "${symLinksFolderLoc}symLink_$2.sl")"
		rm "${symLinksFolderLoc}symLink_$2.sl"
		echo "Program removed the old symlink (symLink_$2.sl)"
	elif [ $createSymLinkBool -eq 0 ]; then
		myPerrorSh $F_CHANGESL $FF "WARNING: symLink_$2.sl wasn't found in $symLinksFolderLoc. Couldn't remove old symlink"
	fi

	# Creating/updating the new symlink
	filePath="$(echo $1 | cut -d ',' -f $filePathsColNum)" #getting the file path
	ln -sf "$filePath" "${symLinksFolderLoc}symLink_$3.sl" #updating symlink
	# Symlink updating/creation details
	if [ ! -e "${symLinksFolderLoc}symLink_$3.sl" ] && [ $createSymLinkBool -eq 0 ]; then
		echo "Symlink file symLink_$3.sl didn't exist. Created it and set its target file to $filePath"
	elif [ $createSymLinkBool -eq 1 ]; then
		echo "Created symLink_$3.sl and set its target file to $filePath"
	else
		echo "Updated target file at symLink_$3.sl"
	fi
}

# This function is the main function for changing symlinks. It combines the functions above
#Args: all
changeSymLink(){
	# Vars
	local FF="${FUNCNAME[0]}"
	#regex: open csv. remove row 1 (header row). split into rows with ',' as delimeter and get field number stored in var
	fileNames="$(cat "$filesAndFoldersCsvLoc" | sed '1d' | cut -d ',' -f $fileNamesColNum)"
	#regex: open csv. remove row 1 (header row). split into rows with ',' as delimeter and get field number stored in var
	filePaths="$(cat "$filesAndFoldersCsvLoc" | sed '1d' | cut -d ',' -f $filePathsColNum)"
	#regex: open csv. remove row 1 (header row). split into rows with ',' as delimeter and get field number stored in var
	fileIds="$(cat "$filesAndFoldersCsvLoc" | sed '1d' | cut -d ',' -f $fileIdColNum)"

	# Data Validation--------------------------------------------------------------------------
	if [ $(isPosIntSh $2) -eq 0 ]; then
		myPerrorSh $F_CHANGESL $FF "Argument 2 (new symlink number) is not a positive int. Aborting program"
		return 1
	fi
	if [[ "$fileNames" =~ ^$ ]]; then 
		myPerrorSh $F_CHANGESL $FF "fileNames variable returned an empty string. Aborting program"
		return 1
	fi
	if [[ "$filePaths" =~ ^$ ]]; then 
		myPerrorSh $F_CHANGESL $FF "filePaths variable returned an empty string. Aborting program"
		return 1
	fi
	if [[ "$fileIds" =~ ^$ ]]; then 
		myPerrorSh $F_CHANGESL $FF "fileIds variable returned an empty string. Aborting program"
		return 1
	fi
	#------------------------------------------------------------------------------------------

	# Getting col type to check for arg 1 in that column type in the csv
	colType=$(getColType $1)
	if [ $colType -eq -1 ]; then
		myPerrorSh $F_CHANGESL $FF "getColType() function returned an error value (-1). Returning 1"
		return 1
	fi

	# Checking for duplicated values in of arg 1 in csv
	checkForDupesInCsv $1 $colType
	if [ ! $? -eq 0 ]; then
		myPerrorSh $F_CHANGESL $FF "checkForDupesInCsv() returned an error value (1). Returning 1"
		return 1
	fi
	
	# Gets the row associated with the key (arg 1). They key could be id, file name, or file path
	# Also gets row num in csv of key. Since func runs in subshell, need to bring it over here to set it to global var
	fileRowAndRowNum="$(getFileRow $1)"
	fileRow="$(echo "$fileRowAndRowNum" | head -n 1)"
	rowNum="$(echo "$fileRowAndRowNum" | tail -n 1)"
	if [[ "$fileRow" =~ ^$ ]]; then myPerrorSh $F_CHANGESL $FF "getFileRow() found no matches. Returning 1"; return 1; fi

	# Finds the previous symlink
	prevSymLinkNum=$(extractPrevSymLinkNum "$fileRow")
	if [[ "$prevSymLinkNum" =~ ^$ ]]; then
		myPerrorSh $F_CHANGESL $FF "extractPrevSymLinkNum() function returned an empty value. Returning 1"
		return 1
	fi
	if [ $prevSymLinkNum -eq 0 ] && [ $createSymLinkBool -eq 0 ]; then #if the prev symlink = 0 and not creating symlink
		myPerrorSh $F_CHANGESL $FF "Invalid previous symlink number; value is 0. Not changing value and updating dependencies. Returning 1"
		myPerrorSh $F_CHANGESL $FF "	symLink_0.sl points to /dev/null, and a lot of files point to that."
		myPerrorSh $F_CHANGESL $FF "	In order to protect against updating the wrong files, updating that symlink number is not allowed"
		return 1
	fi
	if [ $createSymLinkBool -eq 1 ] && [ ! $prevSymLinkNum -eq 0 ]; then #if prev symlink already exists/has non-zero value
		myPerrorSh $F_CHANGESL $FF "File already has an existing associated symlink, symLink_$prevSymLinkNum.sl. Returning 1"
		myPerrorSh $F_CHANGESL $FF "	Cannot create new symlink. Use \"-u\" or \"--update\" option to update an existing file's symlink"
		return 1
	fi	

	# Gets the updated row to add to function
	newRow="$(changeSymLinkNum $fileRow $2)"
	if [[ "$newRow" =~ ^$ ]]; then
		myPerrorSh $F_CHANGESL $FF "changeSymLinkNum() returned an empty string. Returning 1"
		return 1
	fi	

	# Update corresponding files
	# 	-if creating a new symlink or setting the new symlink to 0, use, update csv and & set old symlink to /dev/null 
	#	-else if new symlink != 0, then add normally
	if [ $createSymLinkBool -eq 1 ] || [ $2 -eq 0 ]; then updateSymLinkNumZero "$newRow" "$prevSymLinkNum" "$2";
	elif [ $2 -gt 0 ]; then updateCorrespondingFiles "$newRow" "$prevSymLinkNum" "$2";
	else myPerrorSh $F_CHANGESL $FF "Invalid 2nd arg to function. Returning 1"; return 1; fi
		

	return 0
}

# This function returns the largest symlink number in the files and folders csv, which represents the last symlink num used
#Args: none
getLastSymLink(){
	#the col num is designed for indexing (i.e. it assumes counting starts at 0). Since cut commands starts at 1, need to adjust for that
	adjustedSymLinkColNum=$(expr $symLinkNumColNum + 1)
	#regex: open csv. remove row 1 (header row). split into rows with ',' as delimeter and get field number stored in var
	symLinkNums="$(cat "$filesAndFoldersCsvLoc" | sed '1d' | cut -d ',' -f $adjustedSymLinkColNum)"
	#sort command sorts in ascending order, so need to find last element
	lastSymLinkNumAsStr="$(echo "$symLinkNums" | sort -n | tail -n 1)"
	echo $((lastSymLinkNumAsStr))
}

# This function creates a symlink for a key (file id, file name, or file path) by creating a new symlink for it and updating the csv
#Args: all
createSymLink(){
	# Vars
	local FF="${FUNCNAME[0]}"
	lastSymLinkNum=-1
	newSymLinkNum=-1
	createSymLinkBool=1

	# Data Validation
	if [ ! $# -eq 1 ]; then
		myPerrorSh $F_CHANGESL $FF "Invalid number of args. Only 1 was expected, $# were provided. Returning 1"
		return 1
	fi
	
	# Getting Values
	lastSymLinkNum=$(getLastSymLink)
	if [ $(isIntSh $lastSymLinkNum) -eq 0 ]; then
		myPerrorSh $F_CHANGESL $FF "getLastSymLink() function didn't return an integer value. Returning 1"
		return 1
	fi
	newSymLinkNum=$(expr $lastSymLinkNum + 1) #getting next symlink value

	changeSymLink $1 $newSymLinkNum
	if [ ! $? -eq 0 ]; then
		myPerrorSh $F_CHANGESL $FF "changeSymLink() function returned an error value. Returning 1"
		return 1
	fi
}

# Help Menu
helpMenuChangeSL(){
	echo "+++CHANGE SYMLINK NUMBER FOR A FILE - HELP MENU+++	Relative File Path: $0"
	echo "Purpose: update or create a symlink for a file and all corresponding dependencies"
	echo "Details: update or create a symlink for a file by updating:"
	echo "	-files and folder csv"
	echo "	-removing the old symlink (if updating existing symlink)"
	echo "	-updating all files (in the files and folder csv) that pointed to old symlink (if updating existing symlink)"
	echo "Options: "
	echo "	-c, --create	:- Creates new symlink, but only if the previous symlink value is 0"
	echo "	-u, --update	:- Updates symlink. Can also be used to remove a library (setting symlink id/arg 3 to 0)"
	echo "Usage: "
	echo "	<binNameOrAlias> < -c| --create> <fileId | fileName | filePath>"
	echo "	<binNameOrAlias> < -u | --update> <fileId | fileName | filePath> <new symlink num>"
	echo "Examples: "
	echo "	<binNameOrAlias> -u 1 4		:- will update file with id 1 to point to symLink_4.sl"
	echo "	<binNameOrAlias> -u myScripts 4	:- will update file with name to point to symLink_4.sl"
	echo "	<binNameOrAlias> --update 1 0	:- will update file with id 1 to point to symLink_0.sl and remove the symlink"
	echo "	<binNameOrAlias> -c 1		:- will create a symlink with id 1 greater than the last symlink that points to file with id 1"
	echo "Notes: "
	echo "	-If the existing symlink number for that file is 0, the program won't update anything"
	echo "		-symLink_0.sl points to /dev/null, and a lot of files point to that"
	echo "		-In order to protect against updating the symlinks for the wrong files, updating that symlink number is not allowed"
	echo "	-If the new symlink is 0, no dependencies will be updated for the reasons mentioned above"
}

# Main
main(){
	# Vars
	local FF="${FUNCNAME[0]}"

	# Data Validation--------------------------------------------------------------------------
	if [ $(isHelpSh $1) -eq 1 ]; then helpMenuChangeSL; return 1; fi
	if [ $# -le 1 ] || [ $# -gt 3 ]; then
		myPerrorSh $F_CHANGESL $FF "Invalid number of args. $# were provided, 2 or 3 are expected. Returning 1"
		helpMenuChangeSL
		return 1
	elif [ $1 == "-u" ] && [ ! $# -eq 3 ]; then
		myPerrorSh $F_CHANGESL $FF "Invalid number of args for \"-u\" option. $# were provided, 3 are expected. Returning 1"
		helpMenuChangeSL
		return 1
	elif [ $1 == "-c" ] && [ ! $# -eq 2 ]; then
		myPerrorSh $F_CHANGESL $FF "Invalid number of args for \"-c\" option. $# were provided, 2 are expected. Returning 1"
		helpMenuChangeSL
		return 1
	fi
	#------------------------------------------------------------------------------------------

	if [ $1 == "-u" ] || [ $1 == "--update" ]; then
		shift 1
		changeSymLink $@
		if [ ! $? -eq 0 ]; then
			myPerrorSh $F_CHANGESL $FF "changeSymLink() function returned an error value. Returning 1"
			return 1
		fi
	elif [ $1 == "-c" ] || [ $1 == "--create" ]; then
		shift 1
		createSymLink $@
		if [ ! $? -eq 0 ]; then
			myPerrorSh $F_CHANGESL $FF "createSymLink() function returned an error value. Returning 1"
			return 1
		fi
	else
		myPerrorSh $F_CHANGESL $FF "Invalid 1st arg. Acceptable values are: -c, --create, -u, --update. Returning 1"
		helpMenuChangeSL
		return 1
	fi

	return 0
}

main $@

