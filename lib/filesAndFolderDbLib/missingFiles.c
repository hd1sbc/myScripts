#include "symLink_78.sl"
#include "symLink_36.sl"
#include "symLink_77.sl"

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Message Constant Vars
char* F_FANDFMISSINGFILES = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

/*
// This typedef is used for threads for the run command
// This is commented out since it is auto-added to header file
typedef struct{
	int index;
	char* fileName;
} findStruct;
*/

// This function is the thread function for running the find command
void* findFileInWholeSysThread(void* findStructIn){
	// Vars
	char* FF = (char*) __func__;
	findStruct* f = (findStruct*) findStructIn; //convert from void
	char* command;
	char* output;
	int runComErr = 0;
	
	// Find command looking in home dir
	command = createStr();
	strcat(command, "find ~/ -name ");
	strcat(command, f->fileName);
	strcat(command, " ! -path *backups* 2>/dev/null");
	
	// Run find command
	output = runCommand(command);
	if (output == NULL){
		myPerror(F_FANDFMISSINGFILES, FF, "runCommand returned a null value. Empty string value will be added to global variable fileLocArr at index ", f->index);
		myPerror(F_FANDFMISSINGFILES, FF, "	This happened for thread associated with the file ", f->fileName);
		
		fileLocArr[f->index] = createStr();
		runComErr = 1;
	}
	free(command);

	// If not in home dir, look through system
	if (runComErr == 0 && strlen(output) == 0){
		free(output);

		// New run command
		command = createStr(256);
		strcat(command, "find / -name ");
		strcat(command, f->fileName);
		strcat(command, " ! -path \"");
		strcat(command, myScriptsLoc);
		strcat(command, "*\" 2>/dev/null");
		
		// Run find command
		output = runCommand(command);
		if (output == NULL){
			myPerror(F_FANDFMISSINGFILES, FF, "runCommand returned a null value. Empty string value will be added to global variable fileLocArr at index ", f->index);
			myPerror(F_FANDFMISSINGFILES, FF, "	This happened for thread associated with the file ", f->fileName);
		
			fileLocArr[f->index] = createStr();
			runComErr = 1;
		}
		free(command);

		if (runComErr == 0){ fileLocArr[f->index] = output;} //add to global array
		
	}
	else if (runComErr == 0){ fileLocArr[f->index] = output;} //if found in home dir, add to global array
	
	free(f);
	return NULL;

}

// This function looks for missing files in the system
// 	-adds their new locations to global char* array fileLocArr. If no matches, adds empty string
void missingOfficialFilesWholeSys(struct filesAndFoldersDbObj** missingFilesIn){
	// Vars
	int i = 0; //keeping i outside the for loop for later use
	int size;
	char* FF = (char*) __func__;

	// Data Validation
	if (missingFilesIn == NULL){
		myPerror(F_FANDFMISSINGFILES, FF, "Invalid 1st parameter; value is null. Exiting");
		errorBool = 1;
		return;
	}
	
	// Getting Values
	//setting size of threads array to be that of the number of missing files
	size = getArrSize(missingFilesIn);
	pthread_t threads[size];
	//setting the size of fileLocArr to store the output of the find commands from each thread
	fileLocArr = malloc(sizeof(char*)*(size+1));
	fileLocArr[size] = NULL; //setting last value to be null
	
	// For every missing file, create a separate thread to run the find command
	for (; missingFilesIn[i]->id != -1; i++){
		// Creating a find struct to store the values (index and file name) so I can pass it as a void ptr to thread function
		findStruct* f = malloc(sizeof(findStruct));
		f->index = i;
		f->fileName = missingFilesIn[i]->fileName;
		
		// Create thread and check if creation was successful
		if (pthread_create(&threads[i], NULL, findFileInWholeSysThread, f) != 0){
			myPerror(F_FANDFMISSINGFILES, FF, "Failed to create thread for file ", missingFilesIn[i]->fileName);
			errorBool = 1;
			free(f);
		}
	}

	// Join threads back together once done
	for (int t = 0; t < size; t++) {
		pthread_join(threads[t], NULL);
	}

}
