#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
globalVarsDbShLoc="$(findPath -f 389)" #globalVarsDb.sh
myScriptsLoc="$myScriptsVar"

accDataTypeValColNum=1 #HARDCODED_COL_NUM
colNamesStr="id" #HARDCODED_FIRST_COL
colDataTypesStr="int" #HARDCODED_FIRST_COL
colDefaultValsStr="-1" #HARDCODED_FIRST_COL

# Error Msg Constant Vars
F_DB2="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# SOURCING-----------------------------------------------------------------------------------------
source "$sharedScripts"
checkFileLocs $F_DB2
source "$globalVarsDbShLoc"
#--------------------------------------------------------------------------------------------------

# This function resets a database by removing all existing code in all .c and .h files, generating new code, and adding back to the files
# Args: 1 - dbName
resetDb(){
	# Vars
	local FF="${FUNCNAME[0]}"
	colNameAdjusted=$(expr $colNameColTypeListColNum + 1)
	dataTypeColAdjusted=$(expr $dataTypesColTypeListColNum + 1)

	# Getting Values Of Global Vars------------------------------------------------------------
	structName="$1"
	#regex: find all lines containing db csv. Find the col names from each line. Convert from multi-line string to single-line
	varNames="$(grep "${csvDir}$1\.csv," "$tableColTypeListLoc" | cut -d ',' -f "$colNameAdjusted" | tr '\n' ' ')"
        if [ "$varNames" == "" ]; then
                myPerrorSh $F_DBSH $FF "Error getting column names from tableColTypeList ($tableColTypeListLoc). Variable \"varNames\" is empty. Returning -1"
                myPerrorSh $F_DBSH $FF "	WARNING: File $tableColTypeList might not have been updated correctly or contain rows for a database that doesn't exist. Please check manually"
                return -1
        fi

	#regex: find all lines containing db csv. Find the data types from each line. Convert from multi-line string to single-line
        dataTypes="$(grep "${csvDir}$1\.csv," "$tableColTypeListLoc" | cut -d ',' -f "$dataTypeColAdjusted" | tr '\n' ' ')"
        if [ "$dataTypes" == "" ]; then
                myPerrorSh $F_DBSH $FF "Error getting column data types from tableColTypeList ($tableColTypeListLoc). Variable \"dataTypes\" is empty. Returning -1"
                myPerrorSh $F_DBSH $FF "	WARNING: File $tableColTypeList might not have been updated correctly or contain rows for a database that doesn't exist. Please check manually"
		return -1
        fi
	#------------------------------------------------------------------------------------------

	# Creating/updating struct
	genCodeCreateStructHeaderFile "$1"
	if [ ! $? -eq 0 ]; then myPerrorSh $F_DBSH $FF "genCodeCreateStructHeaderFile() function returned an error value. Returning -1"; return -1; fi
	
	# Adding Code To Files
	headerLocLocal="${incLoc}$structName.h"
	if [ ! -f "$headerLocLocal" ]; then myPerrorSh $F_DBSH $FF "header file $headerLoc not found. Returning -1"; return -1; fi
	
	echo "Removing existing code from files to be updated with new columns..."
	remDbCodeFromFiles "$headerLocLocal"
	myPerrorSh $? $F_DBSH $FF "remDbCodeFromFiles() function returned an error value (!=0). Program will continue"
	
	# Add code to files
	addDbCodeInFiles "$headerLocLocal"
	myPerrorSh $? $F_DBSH $FF "addDbCodeInFiles() function returned an error value (!=0). Program will continue"
}

# This function adds database column and associated values to the list of tableColTyeList.csv
#Args: 1 - dbName; 2 - new database bool
addToTableColTypeList(){
	# Vars
	local FF="${FUNCNAME[0]}"
	stringToAdd=""
	currColNum=0
	colTypeArr=()
	colDefaultValsArr=()
	tableId=0
	
	# Getting Values---------------------------------------------------------------------------
	csvLoc="${csvDir}$1.csv" #setting global var csvLoc
	if [ ! "$2" == "1" ] && [ ! -f "$csvLoc" ]; then myPerrorSh $F_DBSH $FF "The csv file \"$csvLoc\" doesn't exist. Returning -1"; return -1; fi

	# converting string lists to arrays
	IFS=' '
	i=0; for colDataType in $colDataTypesStr; do colTypeArr[$i]="$colDataType"; i=$(expr $i + 1); done
	i=0; for colDefaultVal in $colDefaultValsStr; do colDefaultValsArr[$i]="$colDefaultVal"; i=$(expr $i + 1); done
	
	# finding table id. If an existing db, find its current id. If new database, find greatest database id and add one
	tableIdColNumAdj=$(expr "$tableIdColTypeListColNum" + 1)
	#FIXME this currently finds the last tableId. If I add a column to a table that is not the table with the greatest ID, this value could get fucked up. Change to find greatest id in list
	if [ "$2" == "1" ]; then
		lastTableId="$(cat "$tableColTypeListLoc" | tail -n 1 | cut -d ',' -f "$tableIdColNumAdj")"
		tableId=$(expr "$lastTableId" + 1)
	else
		tableIdStr="$(grep "${csvDir}$1\.csv" "$tableColTypeListLoc" | tail -n 1 | cut -d ',' -f "$tableIdColNumAdj")"
		if [ "$(isIntSh "$tableIdStr")" == "0" ]; then
			myPerrorSh $F_DBSH $FF "The csv file \"$csvLoc\" wasn't found in $tableColTypeListLoc. Returning -1"
			return -1
		fi
		tableId=$((tableIdStr))
	fi
	#------------------------------------------------------------------------------------------
	
	# Finding the greatest colId value for the database. If new database, the value will be 0
	colNumToAddTemp="$(grep "${csvDir}$1.csv," "$tableColTypeListLoc" | wc -l)"
	colNumToAdd=$((colNumToAddTemp)) #convert from str to int

	# Add new col by combining the necessary info to fill all the columns----------------------
	i=0
	for colName in $colNamesStr; do
		stringToAdd+="$colNumToAdd,$tableId,$csvLoc,$colName,${colTypeArr[$i]},${colDefaultValsArr[$i]}\n"
		i=$(expr $i + 1)
		colNumToAdd=$(expr $colNumToAdd + 1)
	done
	stringToAdd="$(echo "$stringToAdd" | sed 's/\n$//g')" #removing trailing '\n'

	echo -e "$stringToAdd" >> "$tableColTypeListLoc"
	sed -i '/^\s*$/d' "$tableColTypeListLoc" #removing all empty lines in the file (echo command above adds an empty line at the end
	#------------------------------------------------------------------------------------------

	return 0
}

# This function adds columns to a database
#Args: 1 - dbName, 2 - new database bool
addCols(){
	# Vars
	local FF="${FUNCNAME[0]}"
	endAddCols=0
	colNum=1
	listOfAcceptedDataTypes=""
	colNameAdjusted=$(expr $colNameColTypeListColNum + 1)
	dataTypeColAdjusted=$(expr $dataTypesColTypeListColNum + 1)
	existingColNamesStr=""
	returnString3=""

	# Getting Values
	accDataTypeValColNumAdj=$(expr $accDataTypeValColNum + 1)
	listOfAcceptedDataTypes="$(cat "$accDataTypesLoc" | cut -d ',' -f $accDataTypeValColNumAdj | tr '\n' ' ')"
	if [ "$2" == "0" ]; then
		existingColNamesStr="$(grep "${csvDir}$dbName\.csv," "$tableColTypeListLoc" | cut -d ',' -f "$colNameAdjusted" | tr '\n' ', ' | sed 's/,/, /g')"
	fi
	#reseting col strings to remove default 1st col values (col with name "id")
	if [ ! "$2" == "1" ]; then colNamesStr="";  colDataTypesStr=""; colDefaultValsStr=""; fi

	echo "ADDING COLUMNS-------------------------------------------------------------------------------------"
	echo "	-name must only include Latin alphabet characters (case-insensitive). No numbers or special characters permitted"
	echo "	-name must be less than 128 characters long"
	echo "	-column names cannot be duplicated"
	if [ "$2" == "1" ]; then
		echo "first column is auto-populated with name \"id\", data type of \"int\", and default value of \"-1\". This cannot be changed"
	fi
	echo "To stop adding columns, press Enter when prompted for a column name"
	echo "---------------------------------------------------------------------------------------------------"
	echo ""

	while [ $endAddCols -eq 0 ]; do
		# Show existing and to-be-added column info
		if [ ! "$2" == "1" ]; then echo "Existing column names:"; echo "$(echo "$existingColNamesStr" | sed 's/,\s\?$//g')"; fi
		echo "Column names to be added:"
		echo "$(echo "$colNamesStr" | tr '\n' ', ' | sed 's/,$//g')"
		echo ""

		# Column Name Validation-----------------------------------------------------------
		echo -n "Enter column name for column number $colNum"
		if [ "$colNum" == "1" ]; then echo " (column number 0 is for pre-populated id):"; else echo ": "; fi
		read colName

		if [ "$colName" == "" ]; then endAddCols=1; break; fi 
		if [ "$(echo "$colName" | sed -n '/^[a-zA-Z]\+$/p')" == "" ]; then
			myPerrorSh $F_DBSH $FF "Invalid column name; value contains special characters. Returning -1"
			return -1
		fi
		numLetters="$(echo $colName | wc -c)"
		if [ ! "$(isIntSh "$numLetters")" == "1" ] || [ $((numLetters)) -gt 127 ]; then
			myPerrorSh $F_DBSH $FF "Invalid column name; value contains too many characters. Returning -1"
			return -1
		fi
		for existingColName in $colNamesStr; do
			existingColNameAdj="$(echo "$existingColName" | sed 's/$/,/g')"
			if [ "$existingColNameAdj" == "$colName," ]; then
				myPerrorSh $F_DBSH $FF "Invalid column name; a column name already has that value. Returning -1"
				return -1
			fi
		done
		for existingColName in $existingColNamesStr; do
			existingColNameAdj="$(echo "$existingColName" | sed 's/ //g')"
			if [ "$existingColNameAdj" == "$colName," ]; then
				myPerrorSh $F_DBSH $FF "Invalid column name; a column name already has that value. Returning -1"
				return -1
			fi
		done
		colNamesStr+=" $colName"
		#----------------------------------------------------------------------------------

		# Column Data Type Validation------------------------------------------------------
		echo "Enter column data type:"
		echo "	-Accepted values: $listOfAcceptedDataTypes"
		read colDataType
		
		foundMatch=0
		for accDataType in $listOfAcceptedDataTypes; do
			if [ "$accDataType" == "$colDataType" ]; then foundMatch=1; fi
		done
		if [ ! $foundMatch -eq 1 ]; then myPerrorSh $F_DBSH $FF "Invalid data type; Returning -1"; return -1; fi

		colDataTypesStr+=" $colDataType"
		#----------------------------------------------------------------------------------
	
		# Column Default Value Validation--------------------------------------------------
		echo "Enter default value:"
		echo "	-must be a valid value based on data type (e.g. bools can only be 0 or 1, ints can only be numbers)"
		read colDefaultVal

		if [ "$colDataType" == "int" ]; then
			if [ "$(echo "$colDefaultVal" | sed -n '/^-\?\(0\|[1-9][0-9]*\)$/p')" == "" ]; then
				myPerrorSh $F_DBSH $FF "Invalid default value; value is not an integer with no leading 0's. Returning -1"
				return -1
			fi
		elif [ "$colDataType" == "bool" ]; then
			if [ "$(echo "$colDefaultVal" | sed -n '/^0\|1$/p')" == "" ]; then
				myPerrorSh $F_DBSH $FF "Invalid default value; value is not 1 or 0. Returning -1"
				return -1
			fi
		elif [ "$colDataType" == "float" ]; then
			if [ "$(echo "$colDefaultVal" | sed -n '/^-\?\(0\|[1-9][0-9]*\)\(\.[0-9]\+\)\?$/p')" == "" ]; then
				myPerrorSh $F_DBSH $FF "Invalid default value; value is not a valid int or decimal. Returning -1"
				return -1
			fi
		elif [ "$colDataType" == "char" ]; then
			numLetters="$(echo "$colDefaultVal" | wc -c)"
			if [ ! "$(isIntSh "$numLetters")" == "1" ] || [ $((numLetters)) -gt 1 ] || [ $((numLetters)) -eq 0 ]; then
				myPerrorSh $F_DBSH $FF "Invalid default value; value is not 1 character long. Returning -1"
				return -1
			fi
		fi

		colDefaultValsStr+=" $colDefaultVal"
		#----------------------------------------------------------------------------------

		# Clearing lines for adding this col
		i=0; while [ $i -lt 11 ]; do printf "\033[1A"; printf "\033[K"; i=$(expr $i + 1); done
		if [ ! "$2" == "1" ]; then printf "\033[1A"; printf "\033[K"; printf "\033[1A"; printf "\033[K"; fi
		colNum=$(expr $colNum + 1)

	done
	
	# Clearing lines for adding columns and adding cols info
	i=0; while [ $i -lt 13 ]; do printf "\033[1A"; printf "\033[K"; i=$(expr $i + 1); done

	# Adding to tableColTypeList
	addToTableColTypeList "$1" "$2"
	if [ ! $? -eq 0 ]; then myPerrorSh $F_DBSH $FF "addToTableColTypeList() function returned an error value. Returning -1"; return -1; fi
	
	# Adding column in database csv------------------------------------------------------------
	dbCsvLocNormal="${csvDir}$dbName.csv"
	dbCsvLoc="${csvDir}$dbName\.csv"; dbCsvLocAdj="$(echo "$dbCsvLoc" | sed 's/\//\\\//g')"
	if [ "$2" == "1" ]; then echo "$(echo "$colNamesStr" | sed 's/ /,/g')" >> "$dbCsvLocNormal"; fi
	colDefaultValsArr=()
	i=0; for colDefaultVal in $colDefaultValsStr; do colDefaultValsArr[$i]="$colDefaultVal"; i=$(expr $i + 1); done

	# Iterating over every line in the file
	i=0
	while IFS= read -r line; do
		# Converting the current line to an array where the delimeter is ','
		IFS=','
		read -r -a lineArray <<< "$line"

		# Iterating over every col in the colNamesStr
		j=0
		for colName in $colNamesStr; do
			#if the curr col is not an existing col and $i != 0 (not header row), then append default value to the line array. If $i == 0 (it is header row), append the col name
			if [[ ! " $colName " =~ "$existingColNamesStr" ]]; then
			       	if [ ! $i -eq 0 ]; then 
					lineArray+=("${colDefaultValsArr[$j]}")
				else
					colNameAdj="$(echo "$colName" | sed 's/ //g')"
					lineArray+=("$colNameAdj")
				fi
			fi
			j=$(expr $j + 1)
		done

		returnString3Temp="${lineArray[*]}" #convert lineArray to string, elements seperated by ','
		returnString3="$returnString3$returnString3Temp\n" #add new string to string of all elements
		i=$(expr $i + 1)
	done < "$dbCsvLocNormal"

	# Add string to csv
	returnString3="$(echo "$returnString3" | sed 's/\n$//g')" #removing trailing new line char
	truncate -s 0 "$dbCsvLocNormal"
	echo -e "$returnString3" >> "$dbCsvLocNormal"
	sed -i '/^\s*$/d' "$dbCsvLocNormal" #removing any trailing empty lines from file
	#------------------------------------------------------------------------------------------

	# Reseting db so new cols are updated
	resetDb "$1"
	if [ ! $? -eq 0 ]; then myPerrorSh $F_DBSH $FF "resetDb() function returned an error value. Returning -1"; return -1; fi

	return 0;
}

# This function initiates the actions for creating a new database
#Args: none
createDbOptions(){
	# Vars
	local FF="${FUNCNAME[0]}"
	numLetters=""
	filesWithDbName=""

	echo ""
	echo "ADD DATATBASE NAME---------------------------------------------------------------------------------"
	echo "	-name must only include Latin alphabet characters (case-insensitive). No numbers or special characters permitted"
	echo "	-name must be less than 128 characters long"
	echo "---------------------------------------------------------------------------------------------------"
	read dbName

	# Name Validation--------------------------------------------------------------------------
	# Character Validation
	if [ "$dbName" == "" ]; then myPerrorSh $F_DBSH $FF "Invalid database name; value is empty. Returning -1"; return -1; fi
	if [ "$(echo "$dbName" | sed -n '/^[a-zA-Z]\+$/p')" == "" ]; then
		myPerrorSh $F_DBSH $FF "Invalid database name; value contains special characters. Returning -1"
		return -1
	fi
	numLetters="$(echo $dbName | wc -c)"
	if [ ! "$(isIntSh "$numLetters")" == "1" ] || [ $((numLetters)) -gt 127 ]; then
		myPerrorSh $F_DBSH $FF "Invalid database name; value contains too many characters. Returning -1"
		return -1
	fi
	
	# Duplicate Name Validation
	echo "Confirming database doesn't already exist..."
	if [ ! "$(find "$myScriptsLoc" -name "$dbName.h" -o -name "$dbName\.csv" 2>/dev/null)" == "" ]; then
		myPerrorSh $F_DBSH $FF "Invalid database name; database with that name already exists. Returning -1"
		return -1
	fi
	# Separating out to catch a matching file name quickly before doing a deeper dive
	if [ ! "$(find "$myScriptsLoc" -name "$dbName" 2>/dev/null)" == "" ]; then
		myPerrorSh $F_DBSH $FF "Invalid database name; database with that name already exists. Returning -1"
		return -1
	fi
	echo "No files or folders found with matching name $dbName. Checking if any other files in $myScriptsDir contain \"$dbName\""
	filesWithDbName="$(egrep -rl "$dbName |$dbName,|$dbName\*|$dbName\." "$myScriptsLoc")" #not checking for just structName in case another db exists containing structName, but has other letters
	if [ ! "$filesWithDbName" == "" ]; then
		myPerrorSh $F_DBSH $FF "Invalid database name; code found in $(echo "$filesWithDbName" | wc -l) files throughout $myScriptsDir with that name"
		echo "Would you like to see the names of all the files with $dbName? (Y/n): " #using echo instead of myPerror cuz requires user input. If I supress stdout, program will hang
		read seeFilesWithDbName
		if [[ "$seeFilesWithDbName" =~ ^(y|Y)$ ]]; then echo "$filesWithDbName"; else echo "Returning -1"; fi
		return -1
	fi

	# Clearing lines above
	i=0; while [ $i -lt 7 ]; do printf "\033[1A"; printf "\033[K"; i=$(expr $i + 1); done
	
	echo "Database name: $dbName"
	echo "Database name is valid"
	#create new db.csv
	csvLoc="${csvDir}$dbName.csv"
	touch "$csvLoc"
	echo ""
	#------------------------------------------------------------------------------------------

	addCols "$dbName" "1" #add cols to new database
	if [ ! $? -eq 0 ];then
		myPerrorSh $F_DBSH $FF "addCols() function returned an error value. Returning -1"
		rm "${csvDir}$dbName.csv"
		return -1
	fi

	# Add to structs.h
	lineNumToAdd="$(grep -n "#endif" "$structsHeaderLoc" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNumToAdd")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "lineNumToAdd for adding struct to structs.h ($structsHeaderLoc) is not a valid integer. All other code was successfully generated. Returning -1"
		rm "${csvDir}$dbName.csv"
		return -1
	fi
	lineNumToAddAdj=$(expr "$lineNumToAdd" - 1)
	sed -i "${lineNumToAddAdj}i #include \"$dbName.h\"" "$structsHeaderLoc"
}

# This function removes a database, including the csv file, everything in the colTableTypeList, and all code in .c and .h files
#Args: none
remDb(){
	# Vars
	local FF="${FUNCNAME[0]}"
	
	echo "REMOVE DATABASE------------------------------------------------------------------------------------"
	echo "Enter database name:"
	read dbName

	echo "Are you sure you want to remove the database and all associated files? This action cannot be undone (y/N): "
	read cont
	if [[ ! "$cont" =~ ^y|Y$ ]]; then echo "Exiting. Nothing changed"; return 0; fi

	dbCsvLoc="${csvDir}$dbName\.csv"
	dbCsvLocAdj="$(echo "$dbCsvLoc" | sed 's/\//\\\//g')"

	if [ "$(grep "${csvDir}$dbName\.csv" "$tableColTypeListLoc")" == "" ]; then
		myPerrorSh $F_DBSH $FF "Invalid database name; database with that name wasn't found in $tableColTypeListLoc. Program will continue"
	fi

	# Setting Global Vars
	csvLoc="$(grep -o "${csvDir}$dbName\.csv" "$tableColTypeListLoc" | tail -n 1)"
	if [ ! -f "$csvLoc" ]; then myPerrorSh $F_DBSH $FF "Csv file associated with database not found. Program will continue"; fi
	
	structLoc="${incLoc}$dbName.h"
	if [ ! -f "$structLoc" ]; then myPerrorSh $F_DBSH $FF "No header file found. Returning -1"; return -1; fi
	
	# Removing db code from .c and .h files
	remDbCodeFromFiles "$structLoc"
	if [ ! $? -eq 0 ]; then myPerrorSh $F_DBSH $FF "remDbCodeFromFiles() function returned an error value. Returning -1"; return -1; fi

	# Removing the databse struct.h file (not structs.h)
	if [ -f "$structLoc" ]; then rm "$structLoc"; fi

	#remove from tableTypeListLoc
	if [ "$1" == "" ]; then sed -i "/$dbCsvLocAdj/d" "$tableColTypeListLoc"; fi

	#remove from structs.h
	sed -i "/#include \"$dbName\.h\"/d" "$structsHeaderLoc"
	
	# Checking if any other remenants of the file
	echo "Checking for any missing files containing $dbName..."
	filesWithDbName="$(egrep -rl "$dbName |$dbName,|$dbName\*|$dbName\." "$myScriptsLoc")" #not checking for just structName in case another db exists containing structName, but has other letters
	if [ ! "$filesWithDbName" == "" ]; then
		echo "Files found containing $dbName. It is recommended to remove $dbName from the following files:"
		echo "$filesWithDbName"
	fi
	
	rm "$csvLoc"

	return 0
}

# This function removes columns from an existing database
#Args: 1 - db name
remCols(){
	# Vars
	local FF="${FUNCNAME[0]}"
	breakLoopBool=0
	existingColNamesStrLocal=""
	colNamesToRemLocal=""
	colNameAdjustedLocal=$(expr $colNameColTypeListColNum + 1)
	colNumsLocal=()
	returnString3=""
	
	echo "REMOVE COLUMNS-------------------------------------------------------------------------------------"
	dbName="$1"
	if [ "$(grep "${csvDir}$dbName\.csv" "$tableColTypeListLoc")" == "" ]; then
		myPerrorSh $F_DBSH $FF "Invalid database name; database with that name wasn't found in $tableColTypeListLoc. Returning -1"
		return -1
	fi

	# Getting Values
	existingColNamesStrLocal="$(grep "${csvDir}$dbName\.csv," "$tableColTypeListLoc" | cut -d ',' -f "$colNameAdjustedLocal" | tr '\n' ', ' | sed 's/,/, /g')"

	# Getting columns to remove
	while [ $breakLoopBool -eq 0 ]; do
		echo "Enter name of column you want to remove:"
		echo "To stop adding columns, press Enter without typing anything"
		read colName
	
		if [ "$colName" == "" ]; then breakLoopBool=1; break; fi

		# Setting if col name exists
		match=0
		colNumCounter=0
		for existingColName in $existingColNamesStrLocal; do
			existingColNameAdj="$(echo "$existingColName" | sed 's/ //g')"
			if [ "$existingColNameAdj" == "$colName," ]; then match=1; colNumsLocal+=("$colNumCounter"); fi
			colNumCounter=$(expr $colNumCounter + 1)
		done
		
		if [ $match -eq 0 ]; then
			myPerrorSh $F_DBSH $FF "Invalid column name; no column had that name. Nothing removed. Returning -1"
			return -1
		fi

		existingColNamesStrLocal="$(echo "$existingColNamesStrLocal" | sed "s/$colName, //g")"
		colNamesToRemLocal+="$colName "
	done

	# Removing all cols in string from tableTypeListLoc
	dbCsvLocNormal="${csvDir}$dbName.csv"
	dbCsvLoc="${csvDir}$dbName\.csv"; dbCsvLocAdj="$(echo "$dbCsvLoc" | sed 's/\//\\\//g')"
	for colName in $colNamesToRemLocal; do sed -i "/$dbCsvLocAdj,$colName,/d" "$tableColTypeListLoc"; done
	
	# Removing column in database csv----------------------------------------------------------
	while IFS= read -r line; do
		# Converting line to an array where the delimeter is ','
		IFS=','
		read -r -a lineArray <<< "$line"

		#iterating over every line and removing the index of column to remove
		for indexOfColToRem in "${colNumsLocal[@]}"; do unset lineArray[$indexOfColToRem]; done
		returnString3Temp="${lineArray[*]}" #converting array to comma-separated line
		returnString3="$returnString3$returnString3Temp\n" #add new line to all lines string
	done < "$dbCsvLocNormal"

	# Updating database.csv
	returnString3="$(echo "$returnString3" | sed 's/\n$//g')" #removing trailing new line char
	truncate -s 0 "$dbCsvLocNormal"
	echo -e "$returnString3" >> "$dbCsvLocNormal"
	sed -i '/^\s*$/d' "$dbCsvLocNormal" #removing any trailing empty lines from file
	#------------------------------------------------------------------------------------------

	# Resetting all db files with new values
	resetDb "$dbName"
	if [ ! $? -eq 0 ]; then myPerrorSh $F_DBSH $FF "resetDb() function returned an error value. Returning -1"; return -1; fi

	return 0
}

# This function renames columns in an existing database
#Args: 1 - db name
renameCols(){
	# Vars
	local FF="${FUNCNAME[0]}"
	existingColNamesStrLocal=""
	colNameAdjusted=$(expr $colNameColTypeListColNum + 1)
	oldName=""
	newName=""
	
	echo "RENAME COLUMNS-------------------------------------------------------------------------------------"
	echo "	-new name must only include Latin alphabet characters (case-insensitive). No numbers or special characters permitted"
	echo "	-new name must be less than 128 characters long"
	echo "---------------------------------------------------------------------------------------------------"

	# Getting Values
	dbName="$1"
	if [ "$(grep "${csvDir}$dbName\.csv" "$tableColTypeListLoc")" == "" ]; then
		myPerrorSh $F_DBSH $FF "Invalid database name; database with that name wasn't found in $tableColTypeListLoc. Returning -1"
		return -1
	fi
	existingColNamesStrLocal="$(grep "${csvDir}$dbName\.csv," "$tableColTypeListLoc" | cut -d ',' -f "$colNameAdjusted" | tr '\n' ', ' | sed 's/,/, /g')" #finding list of existing col names
	
	echo "Enter name of column you want to rename:"
	read colName
	
	# Determining if column name exists--------------------------------------------------------
	if [ "$colName" == "" ]; then myPerrorSh $F_DBSH $FF "Invalid column name; value is empty. Returning -1"; return -1; fi
	match=0
	colNumCounter=0
	for existingColName in $existingColNamesStrLocal; do
		existingColNameAdj="$(echo "$existingColName" | sed 's/ //g')"
		if [ "$existingColNameAdj" == "$colName," ]; then match=1; colNumsLocal+=("$colNumCounter"); fi
		colNumCounter=$(expr $colNumCounter + 1)
	done
		
	if [ $match -eq 0 ]; then
		myPerrorSh $F_DBSH $FF "Invalid column name; no column had that name. Nothing removed. Returning -1"
		return -1
	fi
	oldName="$colName"
	#------------------------------------------------------------------------------------------

	echo "Enter new name:"
	read colName
	# Column Name Validation-------------------------------------------------------------------
	if [ "$colName" == "" ]; then myPerrorSh $F_DBSH $FF "Invalid column name; value is empty. Returning -1"; return -1; fi
	if [ "$(echo "$colName" | sed -n '/^[a-zA-Z]\+$/p')" == "" ]; then
		myPerrorSh $F_DBSH $FF "Invalid column name; value contains special characters. Returning -1"
		return -1
	fi
	numLetters="$(echo $colName | wc -c)"
	if [ ! "$(isIntSh "$numLetters")" == "1" ] || [ $((numLetters)) -gt 127 ]; then
		myPerrorSh $F_DBSH $FF "Invalid column name; value contains too many characters. Returning -1"
		return -1
	fi
	for existingColName in $existingColNamesStrLocal; do
		existingColNameAdj="$(echo "$existingColName" | sed 's/ //g')"
		if [ "$existingColNameAdj" == "$colName," ]; then
			myPerrorSh $F_DBSH $FF "Invalid column name; a column name already has that value. Returning -1"
			return -1
		fi
	done
	newName="$colName"
	#------------------------------------------------------------------------------------------

	# Removing all cols in string from tableTypeListLoc
	dbCsvLocNormal="${csvDir}$dbName.csv"
	dbCsvLoc="${csvDir}$dbName\.csv"; dbCsvLocAdj="$(echo "$dbCsvLoc" | sed 's/\//\\\//g')"
	sed -i "s/$dbCsvLocAdj,$oldName,/$dbCsvLocAdj,$newName,/g" "$tableColTypeListLoc"
	
	# Resetting all db files with new values
	resetDb "$dbName"
	if [ ! $? -eq 0 ]; then myPerrorSh $F_DBSH $FF "resetDb() function returned an error value. Returning -1"; return -1; fi
}

# This function presents the options and runs the associated functions to peform those actions for editing an existing db
#Args: none 
editDb(){
	# Vars
	local FF="${FUNCNAME[0]}"

	echo "EDIT DATABSE---------------------------------------------------------------------------------------"
	echo "Enter database name:"
	read dbName

	if [ "$(grep "$dbName\.csv" "$tableColTypeListLoc")" == "" ]; then
		myPerrorSh $F_DBSH $FF "Invalid database name; database with that name wasn't found in $tableColTypeListLoc. Returning -1"
		return -1
	fi

	echo "Select the number associated with the option that you want to do:"
	echo "	1) Add columns"
	echo "	2) Remove columns"
	echo "	3) Rename column"
	echo "	4) Change column data type"
	echo "	5) Exit (do nothing)"
	read q1

	#FIXME add in return validation
	if [ "$q1" == "1" ]; then
		addCols "$dbName" "0"
	elif [ "$q1" == "2" ]; then
		remCols "$dbName"
	elif [ "$q1" == "3" ]; then
		renameCols "$dbName" #FIXME make sure you can't change any value for id col (col 0). Not just name, but also datatype and default value
	elif [ "$q1" == "4" ]; then
		echo "Function not built yet" #FIXME add feature
	elif [ "$q1" == "5" ]; then
		echo "Exiting"
	else
		myPerrorSh $F_DBSH $FF "Invalid option. Exiting with nothing having been done"
	fi
#FIXME add ability to change default value
	return 0

}

# This function presents the main menu and runs the associated action to perform those actions for a database
#Args: none
dbCAndMMain(){
	# Vars
	local FF="${FUNCNAME[0]}"
	
	echo "+++DATABASE AND TABLE CREATE AND MANAGE+++"
	echo "Select the number associated with the option that you want to do:"
	echo "	1) Create new database"
	echo "	2) Remove existing database"
	echo "	3) Edit an existing database"
	echo "	4) Exit (do nothing)"
	read q0

	if [ "$q0" == "1" ]; then
		createDbOptions
	elif [ "$q0" == "2" ]; then
		remDb
	elif [ "$q0" == "3" ]; then
		editDb
	elif [ "$q0" == "4" ]; then
		echo "Exiting"
	else
		myPerrorSh $F_DBSH $FF "Invalid option. Exiting with nothing having been done"
	fi

	return 0

}

dbCAndMMain

#:<<'com'
#pd=$(pwd)
#cd MYSCRIPTSVAR/dep/fileAndFolderInfo
#rm tableColTypeList.csv; cp tableColTypeList.csv.bak tableColTypeList.csv
#rm Removeme.csv; cp Removeme2.csv.bak Removeme.csv
#cd "$pd"
#resetDb "Removeme" #<-- for debugging. When you have everything in tableTypeColList and you need to add to files
#com
