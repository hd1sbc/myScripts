#!/bin/bash

# This function sets the values of global variables used in the rest of the script based on header file path
#Args: 1 - header file path
getVarsFromHeaderFileAndColList(){
	# Vars
	local FF="${FUNCNAME[0]}"
	colNameAdjusted=$(expr $colNameColTypeListColNum + 1)
	dataTypeColAdjusted=$(expr $dataTypesColTypeListColNum + 1)
	tableNameColAdjusted=$(expr $tableNameColTypeListColNum + 1)
	dbObjNumAdjusted="$(expr $tableIdColTypeListColNum + 1)"
	defaultValsAdjusted="$(expr $defaultValColTypeListColNum + 1)"

	# Data Validation
	if [ ! $# -eq 1 ]; then
		myPerrorSh $F_DBSH $FF "Invalid number of args; 1 is expected $# were provided. No changes made to any files. Returning -1"
		return -1
	fi
	if [ ! -f "$1" ]; then
		myPerrorSh $F_DBSH $FF "Invalid 1st arg (file header path); path doesn't exist. No changes made to any files. Returning -1"
		return -1
	fi

	# Getting Values
	arg1FileName="$(getFileNameNoExtSh "$1" | sed 's/DbObj//g')" #FIXME remove sed from here once you change fAndF header name
	if [ "$arg1FileName" == "" ]; then
		myPerrorSh $F_DBSH $FF "getFileNameNoExtSh() function for arg1FileName produced an empty string. Returning -1"
		return -1
	fi

	# Getting Values Of Global Vars------------------------------------------------------------
	#regex: finding all lines containing path to database csv. Getting their col name. Turning multi-line string into single-line, space-delimeted string
	varNames="$(grep "/$arg1FileName\.csv," "$tableColTypeListLoc" | cut -d ',' -f "$colNameAdjusted" | tr '\n' ' ')"
	if [ "$varNames" == "" ]; then
		myPerrorSh $F_DBSH $FF "Error getting column names from tableColTypeList ($tableColTypeListLoc). Variable varNames is empty. Returning -1"
		return -1
	fi

	#regex: finding all lines containing path to database csv. Getting their data type. Turning multi-line string into single-line, space-delimeted string
	dataTypes="$(grep "/$arg1FileName\.csv," "$tableColTypeListLoc" | cut -d ',' -f "$dataTypeColAdjusted" | tr '\n' ' ')"
	if [ "$dataTypes" == "" ]; then
		myPerrorSh $F_DBSH $FF "Error getting column data types from tableColTypeList ($tableColTypeListLoc). Variable dataTypes is empty. Returning -1"
		return -1
	fi
	
	#regex: from header path, extract line containing "struct (any chars){". Remove "struct". Remove "{"
	structName="$(cat $1 | sed -n '/^struct .*{/p' | sed 's/struct //g' | sed 's/{//g')"
	if [ "$structName" == "" ]; then
		myPerrorSh $F_DBSH $FF "Error getting struct name from header file ($1). Variable structName is empty. Returning -1"
		return -1
	fi

	#regex: finding all lines containing path to database csv. Getting full database csv path (not just name.csv)
	csvLoc="$(grep "/$arg1FileName\.csv," "$tableColTypeListLoc" | cut -d ',' -f "$tableNameColAdjusted" | head -n 1)"
	if [ "$csvLoc" == "" ]; then
		myPerrorSh $F_DBSH $FF "Error getting csv path associated with struct. Variable csvLoc is empty. Returning -1"
		return -1
	fi
	if [ ! -f "$csvLoc" ]; then #checking if file exists
		myPerrorSh $F_DBSH $FF "File path $csvLoc, which is associated with struct, wasn't found. Returning -1"
		return -1
	fi

	#regex: finding all lines containing path to database csv. Getting database id
	dbObjNum="$(grep "/$arg1FileName\.csv," "$tableColTypeListLoc" | cut -d ',' -f "$dbObjNumAdjusted" | head -n 1)"
	if [ "$dbObjNum" == "" ]; then
		myPerrorSh $F_DBSH $FF "Error getting database id associated with struct. Variable dbObjNum is empty. Returning -1"
		return -1
	fi

	#regex: finding all lines containing path to database csv. Getting their default values. Turning multi-line string into single-line, space-delimeted string
	defaultVals="$(grep "/$arg1FileName\.csv," "$tableColTypeListLoc" | cut -d ',' -f "$defaultValsAdjusted" | tr '\n' ' ')"
	if [ "$defaultVals" == "" ]; then
		myPerrorSh $F_DBSH $FF "Error getting default values for each column. Variable defaultVals is empty. Returning -1"
		return -1
	fi
	#------------------------------------------------------------------------------------------

	return 0
}

# This function adds auto-generated code the appropriate files
#Args: 1 - header file path
addDbCodeInFiles(){
	# Vars
	local FF="${FUNCNAME[0]}"
	gcString=""
	funcToAdd=""
	returnVal=0
	returnVal1=0
	returnVal2=0
	returnVal3=0
	returnVal4=0
	returnVal5=0
	returnVa6=0
	returnVal7=0
	returnVal8=0
	returnVal9=0
	returnVal10=0
	returnVal11=0
	returnVal12=0

	# Data Validation
	if [ ! $# -eq 1 ]; then
		myPerrorSh $F_DBSH $FF "Invalid number of args; 1 is expected $# were provided. No changes made to any files. Returning -1"
		return -1
	fi
	if [ ! -f "$1" ]; then
		myPerrorSh $F_DBSH $FF "Invalid 1st arg (file header path); path doesn't exist. No changes made to any files. Returning -1"
		return -1
	fi

	# Getting Values
	getVarsFromHeaderFileAndColList $1 #filling in global vars
	if [ ! $? -eq 0 ]; then
		myPerrorSh $F_DBSH $FF "getVarsFromHeaderFileAndColList() function returned an error value (!=0). No changes made to any files. Returning -1"
		return -1
	fi

	# Adding to database-specific files--------------------------------------------------------
	
	# Get Struct Member Val- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	gcString="$(genCodeGetStructMemberVal)"
	if [ "$gcString" == "" ]; then
		myPerrorSh $F_DBSH $FF "WARNING: genCodeGetStructMemberVal() function returned an empty string. Nothing will be added to $getStructValLoc. Program will continue"
		returnVal=-1
	fi
	funcToAdd="\n// BEGIN $structName\n$gcString\n// END $structName\n"
	addCodeToFiles "$getStructValLoc" "$funcToAdd" "getStructMemberVal_$dbObjNum" 1
	returnVal=$?
	myPerrorSh $returnVal $F_DBSH $FF "WARNING: addCodeToFiles() for getStructMemberVal returned an error value (!=0). Code may not have been added to $getStructValLoc. Program will continue running"
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	# Set Struct Member Val- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	gcString="$(genCodeSetStructMemberVal)"
	if [ "$gcString" == "" ]; then
		myPerrorSh $F_DBSH $FF "WARNING: genCodeSetStructMemberVal() function returned an empty string. Nothing will be added to $setStructValLoc. Program will continue"
		returnVal2=-1
	fi
	funcToAdd="\n// BEGIN $structName\n$gcString\n// END $structName\n"
	addCodeToFiles "$setStructValLoc" "$funcToAdd" "setStructMemberVal_$dbObjNum" 1
	returnVal2=$?
	myPerrorSh $returnVal2 $F_DBSH $FF "WARNING: addCodeToFiles() for setStructMemberVal returned an error value (!=0). Code may not have been added to $setStructValLoc. Program will continue running"
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	
	# Create Struct- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	gcString="$(genCodeCreateStruct)"
	if [ "$gcString" == "" ]; then
		myPerrorSh $F_DBSH $FF "WARNING: genCodeCreateStruct() function returned an empty string. Nothing will be added to $creatStructLoc. Program will continue"
		returnVal3=-1
	fi
	funcToAdd="\n// BEGIN $structName\n$gcString\n// END $structName\n"
	addCodeToCreatStruct "$creatStructLoc" "$funcToAdd" "createStruct_$dbObjNum"
	returnVal3=$?
	myPerrorSh $returnVal3 $F_DBSH $FF "WARNING: addCodeToCreatStruct() function for creatStruct returned an error value (!=0). Code may not have been added to $creatStructLoc. Program will continue running"
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	# Add Row (add element to db arr)- - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	gcString="$(genCodeAddElementToDbArr)"
	if [ "$gcString" == "" ]; then
		myPerrorSh $F_DBSH $FF "WARNING: genCodeAddElementToDbArr function returned an empty string. Nothing will be added to $addRowLoc. Program will continue"
		returnVal4=-1
	fi
	funcToAdd="\n// BEGIN $structName\n$gcString\n// END $structName\n"
	addCodeToFiles "$addRowLoc" "$funcToAdd" "addElementToDbArr_$dbObjNum" 2
	returnVal4=$?
	myPerrorSh $returnVal4 $F_DBSH $FF "WARNING: addCodeToFiles() function for addRow (addElementToDbArr) returned an error value (!=0). Code may not have been added to $addRowLoc. Program will continue running"
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	# Deep Copy Single Pointer- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	gcString="$(genCodeDeepCopyObj)"
	if [ "$gcString" == "" ]; then
		myPerrorSh $F_DBSH $FF "WARNING: genCodeDeepCpy function returned an empty string. Nothing will be added to $deepCpyLoc. Program will continue"
		returnVal5=-1
	fi
	funcToAdd="\n// BEGIN $structName\n$gcString\n// END $structName\n"
	addCodeToFiles "$deepCpyLoc" "$funcToAdd" "deepCpyDbObj_${dbObjNum}" 1
	returnVal5=$?
	myPerrorSh $returnVal5 $F_DBSH $FF "WARNING: addCodeToFiles() function for deepCpyDbObj returned an error value (!=0). Code may not have been added to $deepCpyLoc. Program will continue running"
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	# Deep Copy Double Pointer- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	gcString="$(genCodeDeepCopyObjArr)"
	if [ "$gcString" == "" ]; then
		myPerrorSh $F_DBSH $FF "WARNING: deepCpy() (2 ptr) function returned an empty string. Nothing will be added to $deepCpyLoc. Program will continue"
		returnVal6=-1
	fi
	funcToAdd="\n// BEGIN $structName\n$(genCodeDeepCopyObjArr)\n// END $structName\n"
	addCodeToFiles "$deepCpyLoc" "$funcToAdd" "deepCpyDbObjArr_${dbObjNum}" 2
	returnVal6=$?
	myPerrorSh $returnVal6 $F_DBSH $FF "WARNING: addCodeToFiles() function for deepCpyDbObjArr returned an error value (!=0). Code may not have been added to $deepCpyLoc. Program will continue running"
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	# Remove Row- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	gcString="$(genCodeRemoveRowId)"
	if [ "$gcString" == "" ]; then
		myPerrorSh $F_DBSH $FF "WARNING: removeRowId() (removeElementFromDbArr) function returned an empty string. Nothing will be added to $remRowLoc. Program will continue"
		returnVal7=-1
	fi
	funcToAdd="\n// BEGIN $structName\n$gcString\n// END $structName\n"
	addCodeToFiles "$remRowLoc" "$funcToAdd" "removeElementFromDbArr_${dbObjNum}" 2
	returnVal7=$?
	myPerrorSh $returnVal7 $F_DBSH $FF "WARNING: addCodeToFiles() function for remRowId returned an error value (!=0). Code may not have been added to $remRowLoc. Program will continue running"
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	# Remove Row Based On Val- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	gcString="$(genCodeRemoveRowOnVal)"
	if [ "$gcString" == "" ]; then
		myPerrorSh $F_DBSH $FF "WARNING: removeRowOnVal (removeElementFromDbArrBasedOnVal) function returned an empty string. Nothing will be added to $remRowOnValLoc. Program will continue"
		returnVal8=-1
	fi
	funcToAdd="\n// BEGIN $structName\n$gcString\n// END $structName\n"
	addCodeToFiles "$remRowOnValLoc" "$funcToAdd" "removeElementFromDbArrBasedOnVal_${dbObjNum}" 2
	returnVal8=$?
	myPerrorSh $returnVal8 $F_DBSH $FF "WARNING: addCodeToFiles() function for remRowOnVal returned an error value (!=0). Code may not have been added to $remRowOnValLoc. Program will continue running"
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	# Find All Rows Where- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	gcString="$(genCodeFindAllRows)"
	if [ "$gcString" == "" ]; then
		myPerrorSh $F_DBSH $FF "WARNING: findAllRowsWhere function returned an empty string. Nothing will be added to $getRowsWhereLoc. Program will continue"
		returnVal9=-1
	fi
	funcToAdd="\n// BEGIN $structName\n$gcString\n// END $structName\n"
	addCodeToFiles "$getRowsWhereLoc" "$funcToAdd" "findAllElementsFromDbArrBasedOnVal_${dbObjNum}" 2
	returnVal9=$?
	myPerrorSh $returnVal9 $F_DBSH $FF "WARNING: addCodeToFiles() function for getRowsWhere returned an error value (!=0). Code may not have been added to $getRowsWhereLoc. Program will continue running"
	#- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	
	#------------------------------------------------------------------------------------------
	
	# Adding to generic functions--------------------------------------------------------------
	addCodeToConvert
	returnVal10=$?
	myPerrorSh $returnVal10 $F_DBSH $FF "WARNING: addCodeToConvert() function returned an error value. Code may not have been added to $convertLoc. Program will continue running"

	addCodeToMyFree
	returnVal11=$?
	myPerrorSh $returnVal11 $F_DBSH $FF "WARNING: addCodeToMyFree() function returned an error value. Code may not have been added to $myFreeLoc. Program will continue running"

	addCodeToGetArrSize
	returnVal12=$?
	myPerrorSh $returnVal12 $F_DBSH $FF "WARNING: addCodeToGetArrSize() function returned an error value. Code may not have been added to $getArrSizeLoc. Program will continue running"
	
	#FIXME add myPrint here
	
	#------------------------------------------------------------------------------------------

	if [ ! $returnVal -eq 0 ]; then return $returnVal; fi
	if [ ! $returnVal2 -eq 0 ]; then return $returnVal2; fi
	if [ ! $returnVal3 -eq 0 ]; then return $returnVal3; fi
	if [ ! $returnVal4 -eq 0 ]; then return $returnVal4; fi
	if [ ! $returnVal5 -eq 0 ]; then return $returnVal5; fi
	if [ ! $returnVal6 -eq 0 ]; then return $returnVal6; fi
	if [ ! $returnVal7 -eq 0 ]; then return $returnVal7; fi
	if [ ! $returnVal8 -eq 0 ]; then return $returnVal8; fi
	if [ ! $returnVal9 -eq 0 ]; then return $returnVal9; fi
	if [ ! $returnVal10 -eq 0 ]; then return $returnVal10; fi
	if [ ! $returnVal11 -eq 0 ]; then return $returnVal11; fi
	if [ ! $returnVal12 -eq 0 ]; then return $returnVal12; fi
	return 0
}

# This function combines previous functions to remove all generated db code from every file it was added to
#Args: 1 - header file name (for getVarsFromHeader...)
remDbCodeFromFiles(){
	# Vars
	local FF="${FUNCNAME[0]}"
	returnVal=0

	# Getting Values
	getVarsFromHeaderFileAndColList $1
	if [ ! $? -eq 0 ]; then
		myPerrorSh $F_DBSH $FF "getVarsFromHeaderFileAndColList() function returned an error value (!=0). No changes made to any files. Returning -1"
		return -1
	fi
	
	# Removing from database-specific files
	for fileLoc in "${listOfDbFilesToAddCodeTo[@]}"; do
		remCodeFromFiles "$fileLoc"
		returnValHolder=$?
		myPerrorSh $returnValHolder $F_DBSH $FF "WARNING: remCodeFromFiles() function returned an error message. Program will continue"
		if [ $returnVal -eq 0 ] && [ ! $returnValHolder -eq 0 ]; then returnVal=$returnValHolder; fi
	done

	# Removing from more generic files with other contents (which require specific remove functions)
	for fileLoc in "${listOfExistingFilesToAddCodeTo[@]}"; do
		if [ "$fileLoc" == "$convertLoc" ]; then
			remCodeFromConvert
			returnValHolder=$?
			myPerrorSh $returnValHolder $F_DBSH $FF "WARNING: remCodeFromConvert() function returned an error message. Program will continue"
			if [ $returnVal -eq 0 ] && [ ! $returnValHolder -eq 0 ]; then returnVal=$returnValHolder; fi
		elif [ "$fileLoc" == "$myFreeLoc" ]; then
			remCodeFromMyFree
			returnValHolder=$?
			myPerrorSh $returnValHolder $F_DBSH $FF "WARNING: remCodeFromMyFree() function returned an error message.Program will continue"
			if [ $returnVal -eq 0 ] && [ ! $returnValHolder -eq 0 ]; then returnVal=$returnValHolder; fi
		elif [ "$fileLoc" == "$getArrSizeLoc" ]; then
			remCodeFromGetArrSize
			returnValHolder=$?
			myPerrorSh $returnValHolder $F_DBSH $FF "WARNING: remCodeFromGetArrSize() function returned an error message.Program will continue"
			if [ $returnVal -eq 0 ] && [ ! $returnValHolder -eq 0 ]; then returnVal=$returnValHolder; fi
		#FIXME add printTable here/myPrint
		else
			myPerrorSh $F_DBSH $FF "$fileLoc not found in list of existing files to add code to. Program will continue"
		fi
	done
	
	return $returnVal
}

# This function auto-updates the compilation and header files of the files just changed
#Args: none
autoUp(){
	# Vars
	local FF="${FUNCNAME[0]}"
	
	listOfDbFilesToAddCodeTo2=("$getArrSizeLoc" "$myFreeLoc" "$getStructValLoc" "$setStructValLoc" "$creatStructLoc" "$addRowLoc" "$convertLoc" "$deepCpyLoc" "$remRowLoc" "$remRowOnValLoc" "$getRowsWhereLoc")
	
	for f in "${listOfDbFilesToAddCodeTo2[@]}"; do
		bash "$autoExecLoc" "$f"
		if [ ! $? -eq 0 ]; then
			myPerrorSh $F_DBSH $FF "Error compiling file. Stopping script"
			return -1
		fi
	done

	return 0
}
