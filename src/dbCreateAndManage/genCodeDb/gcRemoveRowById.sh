#!/bin/bash

# This function generates c code to remove a struct element from a struct array based on id
#Args: none
genCodeRemoveRowId(){
	echo "struct $structName** removeElementFromDbArr_${dbObjNum}(struct $structName** objIn, int idIn){"
	echo "	// Vars"
	echo "	char* FF = (char*) __func__;"
	echo "	int size;"
	echo ""
	echo "	// Data Validation"
	echo "	if (idIn < 0){ myPerror($F_remRow, FF, \"Invalid 1st parameter (id); value is less than 0. Returning 2nd parameter\"); return objIn;}"
	echo "	if (objIn == NULL){ myPerror($F_remRow, FF, \"Invalid 2nd parameter (database); value is null. Returning null\"); return NULL;}"
	echo ""

	echo "	// Getting Values"
	echo "	size = getArrSize(objIn);"
	echo "	if (size < 0){ myPerror($F_remRow, FF, \"getArrSize() function returned an error value (<0). Returning null\"); return NULL;}"
	echo "	if (idIn > objIn[size-1]->id){"
	echo "		myPerror($F_remRow, FF, \"Invalid 1st parameter (id); value is larger than last id value: \", objIn[size-1]->id, \". Returning 2nd parameter\");"
	echo "		return objIn;"
	echo "	}"
	echo ""

	echo "	// Iterating over array, removing element with matching id and moving objects to next smaller index"
	echo "	for (int i = 0; i < size; i++){"
	echo "		if (objIn[i]->id == idIn){"
	echo "			myFreeDbObjArr_${dbObjNum}(1, (void*) objIn[i], 0);"
	echo "			objIn[i] = objIn[i+1];"
	echo "		}"
	echo ""
	echo "		if (objIn[i]->id > idIn){"
	echo "			objIn[i] = objIn[i+1];"
	echo "		}"
	echo "	}"
	echo "	objIn[size] = NULL;"
	echo ""

	echo "	return objIn;"
	echo "}"
}
