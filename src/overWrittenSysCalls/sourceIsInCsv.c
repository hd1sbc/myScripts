#include "sourceIsInCsv.h"
#include "genericC.h"
#include "globalVarsOwSysCalls.h"
#include "origOwFuncs.h"
#include "setupAndSpecialFuncs.h"
#include "dbValidationSpecial.h"
#include "fileInfoOwSysCalls.h"

// GLOBAL VARS-------------------------------------------------------------------------------------
extern int (*orig_open)(const char*, int, ...);
extern char* execCodeDirLoc;

// Error Msg Constant Vars
char* F_SOURCEISINCSV = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// This function checks if source of open() function call is in path (filesAndFolder.csv)
int sourceIsInCsv(const char* pathIn, int flagsIn){
	// Vars
	char* FF = (char*) __func__;
	int openFileReturnVal;
	int returnVal = 0;
	char* command;
	char* output;
	char* currOutputLine;
	int currOutputLineLen = 0;
       
	// Data Validation
	if (pathIn == NULL){ myPerror(F_SOURCEISINCSV, FF, "Invalid 1st parameter (file path); value is null. Returning -2"); return -2;}
	
	// Getting Values
	// Opening file so I can use the commands/script below to create a stack trace of files/programs accessing the file
	openFileReturnVal = orig_open(pathIn, flagsIn);
	if (openFileReturnVal == -1){ myPerror(F_SOURCEISINCSV, FF, "Unable to open ", pathIn, ". Returning -2"); return -2;}
	
	// Running command to get parent process file name
	/* Command (written in readable format):
	 * 	unset LD_PRELOAD
	 * 	ppid=$(ps -p $(lsof -F p $pathIn | sed 's/^p//g' | tr '\\n' ',' | sed 's/,$//') --no-headers -o ppid=)
	 * 	processes=$(ps -p $(echo $ppid | sed 's/ /,/g') -o args= | sed 's/ /\\n/g')
	 * 	for line in $processes; do
	 * 		if [ -e $line ]; then
	 * 			echo $(realpath $line)
	 * 		else
	 * 			echo $line
	 * 		fi
	 * 	done
	 *
	 * Explanation:
	 * 	unset LD_PRELOAD
	 * 	- Unsetting LD_PRELOAD to prevent recursive open() function calls and speed up this script
	 * 	
	 * 	ppid=$(ps -p $(lsof -F p $pathIn | sed 's/^p//g' | tr '\\n' ',' | sed 's/,$//') --no-headers -o ppid=)
	 * 	- Finding the parent PID. Removing headers from command output and only getting the ppid column from output
	 *	-using lsof to find the PID's of all processes accessing the file
	 *		-pathIn is the actual file path name, not a variable (no '$' before it in the actual command)
	 *	-cleaning output of "lsof" to match the format needed to use "ps -p" command with multiple PID's
	 *		-removing all leading 'p''s created by using the -F option with lsof
	 *		-replacing all new line chars with commas to match "ps -p" format
	 *		-removing trailing comma
	 *	
	 * 	processes=$(ps -p $(echo $ppid | sed 's/ /,/g') -o args= | sed 's/ /\\n/g')
	 *	- Getting command line associated with the parent PID's found above
	 *	-replacing spaces with commas to match ps -p formatting
	 *	-replacing the spaces from the output to new lines
	 *		-I am trying to get the file path.
	 *		-However, the command could include other elements like "bash filePath".
	 *		-To deal with this, I separate each space into a new line to be processed by the c code below
	 *
	 *	for loop
	 *	- For every line from processes var, finding the absolute path of that line if the path exists
	 *		-I am trying to get the file path, but the path might be relative.
	 *		-So, if the path exists, echo the absolute path
	 */
	command = createStr(strlen(pathIn)+512);
	strcat(command, "unset LD_PRELOAD; ppid=$(ps -p $(lsof -F p "); 
	strcat(command, pathIn);
	strcat(command, " | sed 's/^p//g' | tr '\\n' ',' | sed 's/,$//') --no-headers -o ppid=); processes=$(ps -p $(echo $ppid | sed 's/ /,/g') -o args= | sed 's/ /\\n/g'); for line in $processes; do if [ -e $line ]; then echo $(realpath $line); else echo $line; fi; done");
	
	// Running the command
	output = runCommand(command);
	free(command);
	if (output == NULL){ myPerror(F_SOURCEISINCSV, FF, "runCommand returned a null value. Returning -2"); return -2;}

	// Iterating over every line in command output to see if the line is a file in filesAndFoldersCsv
	currOutputLine = createStr();
	for (int i = 0; output[i] != '\0'; i++){
			
		// If end of line (newline char) or end of file
		if (output[i] == '\n' || output[i+1] == '\0'){
			// if next char is end-of-file char, add current char to currOutputLine
			if (output[i+1] == '\0'){
				if ((currOutputLineLen+2)%128 == 0){ currOutputLine = myRealloc(currOutputLine);} //realloc if needed
				currOutputLine[currOutputLineLen] = output[i];
				currOutputLineLen++;
				currOutputLine[currOutputLineLen] = '\0';
			}
			
			// Seeing if currOutputLine is in executeCode dir
			int isInExecCodeDir = isInDir(execCodeDirLoc, currOutputLine);
			
			// If current output line isn't the pathIn and isn't part of the executeCode dir and a valid file 
			if (strcmp(currOutputLine, pathIn) != 0 && isInExecCodeDir == 1
								&& orig_open(currOutputLine, O_RDONLY) != -1){
				//checking if is in csv here instead of in the conditional to check for errors
				int inCsv = isTargetPathInCsv((const char*) currOutputLine);
				if (inCsv < 0){
					myPerror(F_SOURCEISINCSV, FF, "isTargetPathInCsv() function returned an error value (<0). Returning -2");
					free(output); free(currOutputLine);
					return -2;
				}
				else if (inCsv > 0){ //if in csv (row num > 0)
					returnVal = 1;
					free(currOutputLine);
					break;
				}
			}
			
			if (output[i+1] == '\0'){ free(currOutputLine); break;}
			else{ currOutputLine = resetStr(currOutputLine); currOutputLineLen = 0;} //reset curr output line
		}
		else{
			if ((currOutputLineLen+2)%128 == 0){ currOutputLine = myRealloc(currOutputLine);} //realloc if needed
			currOutputLine[currOutputLineLen] = output[i]; //add char to curr output line
			currOutputLineLen++;
			currOutputLine[currOutputLineLen] = '\0';
		}
	}
	
 	free(output);
	if (returnVal == 1){ return openFileReturnVal;}
	else{ return -1;}
}
