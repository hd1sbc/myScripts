#!/bin/bash

# This function generates c code to append to getArrSize.c
#Args: none
genCodeGetArrSize(){
	echo "int ${structName}GetArrSize(struct $structName** objIn){"
	echo "	int size = 0;"
	echo "	if (objIn == NULL){ return nullInput();} //data validation"
	echo "	while (objIn[size]->id != -1){ size++;}"
	echo "	return size;"
	echo "}"
}
