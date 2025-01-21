#include "symLink_94.sl"
#include "symLink_76.sl"
#include "symLink_31.sl"
#include "symLink_93.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
char* symLinkFilePrefix = "symLink_"; //HARDCODED_PREFIX

// Error Message Constant Vars
char* F_SYSOBJSL = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function determines if the file at the given id is a symlink. Returns 1 if true, 0 if false
int isSymLinkAtId(int idIn){
	// Vars
	char* idInAsStr;
	char* fileType;
	char* csvAsStr;
	int returnBool = 0;
	char* FF = (char*) __func__;

	// Data Validation
	if (idIn < 1){ myPerror(F_SYSOBJSL, FF, "Invalid 1st parameter (file id); value is < 1. Returning -1"); return -1;}
	
	// Getting Values
	csvAsStr = convert(sysFilesAndFoldersListLoc, "string");
	if (csvAsStr == NULL){ myPerror(F_SYSOBJSL, FF, "convert() function returned a null value. Returning -1"); return -1;}
	if (idIn > getNumRows(csvAsStr)){ //checking if valid id again
		myPerror(F_SYSOBJSL, FF, "Invalid 1st parameter (file id); value is greater than the number of files in files and folders csv. Returning -1");
		free(csvAsStr);
		return -1;
	}
	free(csvAsStr);

	// Converting to string so I can use as the input for vLookUp
	idInAsStr = convert(idIn, "string");
	if (idInAsStr == NULL){ myPerror(F_SYSOBJSL, FF, "convert() function returned a null value. Returning -1"); return -1;}
	
	// Looking up ID in the csv
	fileType = vLookUp(idInAsStr, 0, 1, sysFilesAndFoldersListLoc);
	if (fileType == NULL){
		myPerror(F_SYSOBJSL, FF, "vLookUp() function returned a null value. Returning -1");
		free(idInAsStr);
		return -1;
	}
	
	//if the system object type is 'l', then a symlink
	//'l' is the data type returned for a symlink when running 'ls -l' command
	if (strcmp(fileType, "l") == 0){ returnBool = 1;}

	free(idInAsStr);
	free(fileType);
	return returnBool;
}

// This function gets symlink path associated with the id of the input file
char* getSymLinkById(int idIn){
	// Vars
	char* FF = (char*) __func__;
	char* symLinkFileName;
	char* idAsStr;
	char* symLinkFilePath;
	char* symLinkIdAsStr;
	char* csvAsStr;

	// Data Validation-------------------------------------------------------------------------
	csvAsStr = convert(sysFilesAndFoldersListLoc, "string");
	if (csvAsStr == NULL){ myPerror(F_SYSOBJSL, FF, "convert() function returned a null value. Returning null"); return NULL;}
	if (idIn > getNumRows(csvAsStr)){
		myPerror(F_SYSOBJSL, FF, "Invalid 1st parameter (file id); value is greater than the number of files in files and folders csv. Returning null");
		free(csvAsStr);
		return NULL;
	}
	free(csvAsStr);
	// Checking if file is a symlink at the id
	if (isSymLinkAtId(idIn) == 1){
		//if a symlink, just returning that file path
		myPerror(F_SYSOBJSL, FF, "WARNING: the file at the given index is a symlink. Returning the symlink path");
		symLinkFilePath = getFileById(idIn);
		if (symLinkFilePath == NULL){
			myPerror(F_SYSOBJSL, FF, "getFileById() function returned a null value. Returning null");
			return NULL;
		}
		return symLinkFilePath;
	}
	//-----------------------------------------------------------------------------------------

	// Creating the symlink name
	symLinkFileName = createStr();
	strcat(symLinkFileName, symLinkFilePrefix);
	idAsStr = convert(idIn, "string"); //converting to string to add to file name
	if (idAsStr == NULL){
		myPerror(F_SYSOBJSL, FF, "convert() function returned a null value. Returning null");
		free(symLinkFileName);
		return NULL;
	}
	symLinkIdAsStr = vLookUp(idAsStr, 0, 9, sysFilesAndFoldersListLoc); 
	strcat(symLinkFileName, symLinkIdAsStr);
	strcat(symLinkFileName, ".sl"); //adding extension

	// Getting file path in the csv file from the file name
	symLinkFilePath = getFilePathFromFileName(symLinkFileName);
	if (symLinkFilePath == NULL){
		myPerror(F_SYSOBJSL, FF, "getFilePathFromFileName() function returned a null value. Returning null");
		free(symLinkFileName);
		return NULL;
	}
	if(strlen(symLinkFilePath) == 0){ 
		myPerror(F_SYSOBJSL, FF, "WARNING: getFilePathFromFileName() returned empty string");
		fprintf(stderr, "%s%s%s%s%d%s", F_SYSOBJSL, FF, "	The associated symlink ", symLinkFilePrefix, idIn, " was not found in files and folders database\n");
	}


	free(symLinkFileName);
	free(symLinkIdAsStr);
	free(idAsStr);
	return symLinkFilePath;
	
}

// This function gets the id of the symlink associated with the file name
int getSymLinkIdFromFileName(char* fileNameIn){
	// Vars
	int id;
	char* associatedSymLink;
	int symLinkId;
	char* FF = (char*) __func__;

	// Data Validation
	if (fileNameIn == NULL){ myPerror(F_SYSOBJSL, FF, "Invalid 1st parameter; value is null. Returning -1"); return -1;}
	
	// Getting id from file name
	id = getIdFromFile(fileNameIn);
	if (id == -1){ myPerror(F_SYSOBJSL, FF, "getIdFromFile() function returned an error value (-1). Returning -1"); return -1;}

	// Checking if file is a symlink at the id
	if (isSymLinkAtId(id) == 1){
		myPerror(F_SYSOBJSL, FF, "WARNING: file at given index is a symlink. Returning file id of symlink");
		return id;
	}

	// Finding associated symlink
	associatedSymLink = getSymLinkById(id);
	if (associatedSymLink == NULL){
		myPerror(F_SYSOBJSL, FF, "getSymLinkById() function returned a null value. Returning -1");
		return -1;
	}
	
	// Finding id of the symlink
	symLinkId = getIdFromFile(associatedSymLink);
	if (symLinkId == -1){ myPerror(F_SYSOBJSL, FF, "getIdFromFile() function returned an error value (-1). Returning -1");}
	
	free(associatedSymLink);
	return symLinkId;	

}
