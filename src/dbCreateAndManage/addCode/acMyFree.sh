#!/bin/bash

# This function adds code to myFree.c and myFree.h
#Args: none
addCodeToMyFree(){
	# Vars
	local FF="${FUNCNAME[0]}"

	# Data Validation
	if [ ! "$(cat "$myFreeLoc" | grep "// BEGIN $structName")" == "" ]; then
		myPerrorSh $F_DBSH $FF "$structName code already exists in file $myFreeLoc. Nothing added to file. Returning -1"
		return -1
	fi

	# Adding primary free function-------------------------------------------------------------
	lineNum="$(grep -n "void myFreeHelper(" "$myFreeLoc" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNum")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Line num $lineNum for primary free function in .c file is not a valid integer. Returning -1"
		return -1
	fi
	lineNumAdjusted=$(expr $lineNum - 1) #adding to line above myFreeHelper()

	stringToAdd="\n// BEGIN $structName\n$(genCodeMyFree)\n// END $structName\n"
	insertTxtInFile "$stringToAdd" -l "$lineNumAdjusted" "$myFreeLoc"
	myPerrorSh $? $F_DBSH $FF "WARNING: insertTxtInFile for $myFreeLoc primary free function returned an error value (!=0). Code may not have been added to the file. Program will continue."
	#------------------------------------------------------------------------------------------

	# Adding to define in c file---------------------------------------------------------------
	if [ "$(grep "struct $structName\**:" "$myFreeLoc")" == "" ]; then
		# Finding all instances of last "default" option for each num-of-args define statement
		lineNumsDefault="$(grep -n "default: badDataMyFree " "$myFreeLoc" | cut -d ':' -f 1)"
		if [ "$lineNumsDefault" == "" ]; then
			myPerrorSh $F_DBSH $FF "No \"default: badDataMyFree \" pattern was found in file $myFree. Returning -1"
			return -1
		fi

		# Iterating over each line num and adding the necessary code with the right function name and num pointers
		lineNumAdjuster=0 #adjust since we are adding new lines while iterating over file
		numArgsCounter=1 #to determine the function name associated with each generic type
		lineNumsDefault="$(echo "$lineNumsDefault" | tr '\n' ',' | sed 's/,$//g')"
		origIfs=$IFS
		IFS=','
		for lineNumDefault in $lineNumsDefault; do
			if [ ! "$(isIntSh "$lineNumDefault")" == "1" ]; then
				myPerrorSh $F_DBSH $FF "Line num $lineNumDefault for adding to define in c file of $myFreeLoc is not a valid integer. Returning -1"
				return -1
			fi

			lineNumDefault=$(expr $lineNumDefault + $lineNumAdjuster) #adjusting line num for previous in-place text removal
			# Getting define function name
			funcName=""
			if [ $numArgsCounter -eq 1 ]; then funcName="oneArgsFree$structName"; fi
			if [ $numArgsCounter -eq 2 ]; then funcName="twoArgsFree$structName"; fi
			if [ $numArgsCounter -eq 3 ]; then funcName="threeArgsFree$structName"; fi

			#FIXME why are these commented?? Uncomment them
			#perl -i -pe "if ($. == $lineNumDefault) { print \"		struct $structName**: ${funcName}Two, \\\\\\n\"; }" "$myFreeLoc"
			myPerrorSh $? $F_DBSH $FF "WARNING: Issue with running perl command for double pointer in file $myFreeLoc. Program will continue"
			#FIXME why are these commented?? Uncomment them
			#perl -i -pe "if ($. == $lineNumDefault) { print \"		struct $structName*: ${funcName}One, \\\\\\n\"; }" "$myFreeLoc"
			myPerrorSh $? $F_DBSH $FF "WARNING: Issue with running perl command for single pointer in file $myFreeLoc. Program will continue"

			# Incrementing values
			lineNumAdjuster=$(expr $lineNumAdjuster + 2)
			numArgsCounter=$(expr $numArgsCounter + 1)
		done
		IFS=$origIfs
	else
		myPerrorSh $F_DBSH $FF "$structName code already exists in define part of file $myFreeLoc. Nothing added to \"backup of defines\" section. Program will continue"
	fi
	#------------------------------------------------------------------------------------------

	# Adding db-type-to-function function------------------------------------------------------
	lineNum="$(grep -n "// DATABASE TYPE TO FUNCTION" "$myFreeLoc" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNum")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Line num $lineNum for adding db-type-to-function .c file is not a valid integer. Returning -1"
		return -1
	fi
	lineNumAdjusted=$(expr $lineNum + 2) #adding 2 line after

	# Functions based on num pointers and num  args
	stringToAdd1Args1Ptr="\nvoid oneArgsFree${structName}One(struct $structName* objIn, ...){ myFreeDbObjArr_$dbObjNum(1, (void*) objIn, 0);}"
	stringToAdd1Args2Ptr="\nvoid oneArgsFree${structName}Two(struct $structName** objIn, ...){ myFreeDbObjArr_$dbObjNum(2, (void*) objIn, 0);}"
	stringToAdd2Args1Ptr="\nvoid twoArgsFree${structName}One(struct $structName* objIn, int printBoolIn, ...){ myFreeDbObjArr_$dbObjNum(1, (void*) objIn, printBoolIn);}"
	stringToAdd2Args2Ptr="\nvoid twoArgsFree${structName}Two(struct $structName** objIn, int printBoolIn, ...){ myFreeDbObjArr_$dbObjNum(2, (void*) objIn, printBoolIn);}"
	stringToAdd3Args1Ptr="\nvoid threeArgsFree${structName}One(struct $structName* objIn, int printBoolIn, ...){ myFreeDbObjArr_$dbObjNum(1, (void*) objIn, printBoolIn);}"
	stringToAdd3Args2Ptr="\nvoid threeArgsFree${structName}Two(struct $structName** objIn, int printBoolIn, ...){ myFreeDbObjArr_$dbObjNum(2, (void*) objIn, printBoolIn);}"
	#combining the functions and adding begin and end comment markers
	stringToAdd="// BEGIN $structName$stringToAdd1Args1Ptr$stringToAdd1Args2Ptr$stringToAdd2Args1Ptr$stringToAdd2Args2Ptr$stringToAdd3Args1Ptr$stringToAdd3Args2Ptr\n// END $structName"

	insertTxtInFile "$stringToAdd" -l "$lineNumAdjusted" "$myFreeLoc" #inserting in file
	myPerrorSh $? $F_DBSH $FF "WARNING: insertTxtInFile for $myFreeLoc for adding db-type-to-function function returned an error value (!=0). Code may not have been added to the file. Program will continue."
	#------------------------------------------------------------------------------------------

	# Adding db-type-to-function function------------------------------------------------------
	lineNum="$(grep -n "// DATABASE TYPE TO FUNCTION" "$myFreeLoc" | tail -n 1 | cut -d ':' -f 1)"
	if [ ! "$(isIntSh "$lineNum")" == "1" ]; then
		myPerrorSh $F_DBSH $FF "Line num $lineNum for adding db-type-to-function .c file is not a valid integer. Returning -1"
		return -1
	fi
	lineNumAdjusted=$(expr $lineNum + 2) #adding 2 line after

	# Adding define to header file-------------------------------------------------------------
	# This is identical to adding to define in c file, except using headerLoc instead of $myFreeLoc
	local headerLoc="${incLoc}myFree.h" #HARDCODED
	if [ ! -f "$headerLoc" ]; then
		myPerrorSh $F_DBSH $FF "Header file $headerLoc not found for $myFreeLoc. Returning -1"
		return -1
	fi

	if [ "$(grep "struct $structName\**:" "$headerLoc")" == "" ]; then
		# Finding all instances of last "default" option for each num-of-args define statement
		lineNumsDefault="$(grep -n "default: badDataMyFree " "$headerLoc" | cut -d ':' -f 1)"
		if [ "$lineNumsDefault" == "" ]; then
			myPerrorSh $F_DBSH $FF "No \"default: badDataMyFree \" pattern was found in $headerLoc for file $myFree. Returning -1"
			return -1
		fi

		# Iterating over each line num and adding the necessary code with the right function name and num pointers
		lineNumAdjuster=0 #adjust since we are adding new lines while iterating over file
		numArgsCounter=1 #to determine the function name associated with each generic type

		lineNumsDefault="$(echo "$lineNumsDefault" | tr '\n' ',' | sed 's/,$//g')"
		origIfs=$IFS
		IFS=','
		for lineNumDefault in $lineNumsDefault; do
			if [ ! "$(isIntSh "$lineNumDefault")" == "1" ]; then
				myPerrorSh $F_DBSH $FF "Warning: Line num $lineNumDefault for adding to define in c file of $headerLoc is not a valid integer. Returning -1"
				return -1
			fi
			lineNumDefault=$(expr $lineNumDefault + $lineNumAdjuster) #adjusting line num for previous in-place text removal

			# Getting define function name
			funcName=""
			if [ $numArgsCounter -eq 1 ]; then funcName="oneArgsFree$structName"; fi
			if [ $numArgsCounter -eq 2 ]; then funcName="twoArgsFree$structName"; fi
			if [ $numArgsCounter -eq 3 ]; then funcName="threeArgsFree$structName"; fi

			perl -i -pe "if ($. == $lineNumDefault) { print \"		struct $structName**: ${funcName}Two, \\\\\\n\"; }" "$headerLoc"
			myPerrorSh $? $F_DBSH $FF "WARNING: Issue with running perl command for double pointer in file $headerLoc. Program will continue"
			perl -i -pe "if ($. == $lineNumDefault) { print \"		struct $structName*: ${funcName}One, \\\\\\n\"; }" "$headerLoc"
			myPerrorSh $? $F_DBSH $FF "WARNING: Issue with running perl command for single pointer in file $headerLoc. Program will continue"

			# Incrementing values
			lineNumAdjuster=$(expr $lineNumAdjuster + 2)
			numArgsCounter=$(expr $numArgsCounter + 1)
		done
		IFS=$origIfs
	else
		myPerrorSh $F_DBSH $FF "$structName code already exists in define part of file $headerLoc. Nothing added to \"backup of defines\" section. Program will continue"
	fi
	#------------------------------------------------------------------------------------------
	
	return 0
}
