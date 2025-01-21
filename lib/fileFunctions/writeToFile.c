#include "symLink_98.sl"
#include "symLink_95.sl"
#include "symLink_96.sl"
#include "symLink_97.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_WRITEFILE = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function writes a string to file. If the file doesn't exist, it will be created. If it does exist, it will overwrite the existing contents with the string input
int writeToFile(char* filePathOfFileToWriteIn, char* fileContentsIn){
	// Vars
	char* FF = (char*) __func__;
	FILE* file;

	// Data Validation-------------------------------------------------------------------------
	if (filePathOfFileToWriteIn == NULL){
		myPerror(F_WRITEFILE, FF, "Invalid 1st parameter (file path); value is null. Nothing written to file");
		return -1;
	}
	if (strlen(filePathOfFileToWriteIn) == 0){ 
		myPerror(F_WRITEFILE, FF, "Invalid 1st parameter (file path); path is empty. Nothing written to file");
		return -1;
	}
	if (fileContentsIn == NULL){ 
		myPerror(F_WRITEFILE, FF, "Invalid 2nd parameter (file contents); value is null. Nothing written to file");
		return -1;
	}
	if (strlen(fileContentsIn) == 0){
		myPerror(F_WRITEFILE, FF, "WARNING: 2nd parameter (file contents) is empty. Writing to file will empty the file");
		//using printf instead of perror in case I supress stderr, I won't see this and the output will
		printf("===SHELL COMMANDS IN C - writeToFile===	Do you wish to continue and remove the existing contents of the file? (y/N): ");
		char emptyFile;
		scanf("%c", &emptyFile);
		if (emptyFile == 'N' || emptyFile == 'n'){ printf("Nothing written to file"); return 0;}
		else if (emptyFile != 'Y' || emptyFile != 'y'){ printf("Invalid input. Nothing written to file"); return 0;}
	}
	if (fileExists(filePathOfFileToWriteIn, 0) != 1){
		myPerror(F_WRITEFILE, FF, "WARNING: File doesn't exist. File ", filePathOfFileToWriteIn, " will be created");
	}
	//-----------------------------------------------------------------------------------------
	
	// Opening file
	file = fopen(filePathOfFileToWriteIn, "w+");
	if (file == NULL){
		myPerror(F_WRITEFILE, FF, "fopen() standard c function returned a null value, meaning the file wasn't opened correctly. Returning -1");
		return -1;
	}
	
	// Writing to file
	int fprintfReturnVal = fprintf(file, fileContentsIn);
	if (fprintfReturnVal < 0){
		myPerror(F_WRITEFILE, FF, "fprintf() standard c function returned a negative value, meaning there was an error modifying the file. Returning -1");
		return -1;
	}
	
	// Closing file
	int fcloseReturnVal = fclose(file);
	if (fcloseReturnVal != 0){ 
		myPerror(F_WRITEFILE, FF, "Error closing file; fclose() returned a non-zero value. Returning -1");
		return -1;
	}
	
	return 0;
}
