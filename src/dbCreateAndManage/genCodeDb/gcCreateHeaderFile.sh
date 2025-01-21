#!/bin/bash

# This function generates c code to create or update a struct definition in a header file based on the columns in tableTypeColList.csv
#Args: 1 - data base name
genCodeCreateStructHeaderFile(){
	# Vars
	local FF="${FUNCNAME[0]}"
	dataTypesArr=()
	headerStructLoc="${incLoc}$1.h"

	# Truncate file if it exists, otherwise create it
	if [ -f "$headerStructLoc" ]; then truncate -s 0 "$headerStructLoc"; fi
	touch "$headerStructLoc"
	if [ ! -f "$headerStructLoc" ]; then myPerrorSh $F_DBSH $FF "Error creating/emptying file $headerStructLoc. Returning -1"; return -1; fi

	# Adding the c code
	echo "#ifndef $(echo "$structName" | tr '[:lower:]' '[:upper:]')" >> "$headerStructLoc"
	echo "#define $(echo "$structName" | tr '[:lower:]' '[:upper:]')" >> "$headerStructLoc"
	echo "" >> "$headerStructLoc"

	echo "struct $structName{" >> "$headerStructLoc"

	# Converting datTypes string to array. Using "read -r -a ..." wasn't working, so did it manually
	origIfs=$IFS
	IFS=' '
	for ele in $dataTypes; do
		dataTypesArr+=("$ele");
	done

	i=0
	for varName in $varNames; do
		if [ "${dataTypesArr[$i]}" == "string" ]; then
			echo "	char* $varName;" >> "$headerStructLoc"
		elif [ "${dataTypesArr[$i]}" == "bool" ]; then
			echo "	int $varName;" >> "$headerStructLoc"
		else
			echo "	${dataTypesArr[$i]} $varName;" >> "$headerStructLoc"
		fi
		i=$(expr $i + 1)
	done
	IFS=$origIfs

	echo "};" >> "$headerStructLoc"
	echo "" >> "$headerStructLoc"
	echo "#endif" >> "$headerStructLoc"

	return 0
}
