#include "symLink_102.sl" //findPathInC.h
#include "symLink_30.sl" //cStdLibs.h

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_MYFINDPATHC = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function runs findPath command within a shell and returns the result as a string
// This is different from runCommand in shellCommandsInC in that it doesn't require any other libs
char* myFindPathC(int filePathId){
        // Vars
        char* FF = (char*) __func__;
        FILE* shellOutput;
        char ch;
	char* command;
	char filePathIdAsStr[20];
	char* devNull;
        int arrInc = 128;
        int returnStrLen = 0;
        char* returnStr;

	// Data Validation
	if (filePathId == 0){ //returning /dev/null. Instead of using a string literal, alloc'ing so I don't get r--mapped error
		devNull = calloc(15, sizeof(char));
		strcat(devNull, "/dev/null");
		return devNull;
	}

	// Getting Values
	snprintf(filePathIdAsStr, sizeof(filePathIdAsStr), "%d", abs(filePathId));
        command = calloc(40, sizeof(char)); //size of 40 cuz 20 for size of FilePathIdAsStr and 20 for approximate size of rest of command
	strcat(command, "findPath ");
	if (filePathId > 0){ strcat(command, "-f ");} //getting path literal, not symlink path
	strcat(command, filePathIdAsStr);
	returnStr = malloc(sizeof(char)*128);
    	
	// Running command and getting output as file descriptor
        shellOutput = popen(command, "r");
        if (shellOutput == NULL){
		fprintf(stderr, "%s%s%s%s%s", "===", F_MYFINDPATHC, " - ", FF, "===	");
		fprintf(stderr, "%s%d%s\n", "popen() c standard function returned a null value for file id: ", filePathId, ". Exiting -1");
		free(command);
                exit(-1);
        }
	free(command);

        // Adding every char from output to return array
        while ((ch=fgetc(shellOutput)) != EOF){
                if ((returnStrLen+1)%arrInc == 0 && returnStrLen != 0){ returnStr = realloc(returnStr, returnStrLen+arrInc);}
                if (returnStr != NULL){ 
			returnStr[returnStrLen] = ch;
                        returnStr[returnStrLen+1] = '\0';
                        returnStrLen++;
                }
                else{
               		fprintf(stderr, "%s%s%s%s%s", "===", F_MYFINDPATHC, " - ", FF, "===	");
			fprintf(stderr, "%s%d%s\n", "Failure with realloc() function for file id: ", filePathId, ". It returned a null value. Exiting -1");
                        free(returnStr);
                        pclose(shellOutput);
	                exit(-1);
                }
        }
        pclose(shellOutput);

	// Checking if anything was printed to stdout
	if (returnStr == NULL || returnStr[0] == '\n' || returnStr[0] == '\0' || returnStr[0] == ' '){
		fprintf(stderr, "%s%s%s%s%s", "===", F_MYFINDPATHC, " - ", FF, "===	");
		fprintf(stderr, "%s%d%s\n", "Invalid output from findPath command for file id: ", filePathId, ". Nothing returned from command. Exiting -1");
		free(returnStr);
		exit(-1);
	}

        if (strlen(returnStr) > 0 && returnStr[strlen(returnStr)-1] == '\n'){ //if ends in new line char, remove it and mark new end-of-string position
                returnStr[strlen(returnStr)-1] = '\0';
        }

        return returnStr;
}
