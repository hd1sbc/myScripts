#include "symLink_36.sl"
#include <unistd.h>

// GLOBAL VARS-------------------------------------------------------------------------------------
// Error Message Constant Vars
char* F_SYSOBJM = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

// Help Menu
void helpMenu(){
	puts("+++FIND SYSTEM OBJECT - HELP MENU+++	File Name: F_SYSOBJM");
	puts("Purpose: To find a file path/symlink path based on file ID or find file ID based on file name or path");
	puts("Details: user can provide either an int representing the ID of the file, a file path, or a file name.");
	puts("		Based on the type of input, the program will either return the symlink path based on the ID or the ID of the file name/path");
	puts("Options: ");
	puts("	-f		:- Get file ID or path literal, not symlink value");
	puts("Usage: ");
	puts("	< -f> [int id] || < -f> [char* filePathOrName]");
	puts("	id			:- The id (integer) of the file. This option will return the path of the symlink associated with that ID");
	puts("				   If the file is a symlink at that ID, then the program will just return that file's path");
	puts("	filePathOrName		:- The file path or file name. This will return the ID of the symlink associated with that file");
	puts("	-f			:- This optional arg will take find the literal values of the next option, not its associated symlink values");
	puts("				   This option works with either the integer ID or the file path/name");
	puts("Examples: ");
	puts("	<binName> 1		:- Returns the symlink path for the file whose ID in the files and folders DB is 1");
	puts("	<binName> 10		:- Say the file with the ID of 10 is a symlink, then this will return the file path of that file, not its associated symlink");
	puts("	<binName> -f 1		:- Returns the file path for the file whose ID in the files and folders DB is 1. It doesn't return the symlink path");
	puts("	<binName> -f 10		:- Say the file with the ID of 10 is a symlink, this does the same as <binName> 10");
	puts("	<binName> myScripts	:- Returns the ID of the symlink within the files and folders DB that is associated with that file path or name");
	puts("	<binName> symLink_0.sl	:- Return the ID of that file within the files and folders DB");
	puts("	<binName> -f myScripts	:- Returns the ID of the file (not the symlink) within the files and folders DB");
}

// This function check if the number of args is valid as well as checking if help menu flag was selected
// This function behaves differently if this is called from a script or the terminal
// 	-if from the terminal, help menu is displayed
int numArgsValid(int numArgsIn, char* argvIn[]){
	// Vars
	char* FF = (char*) __func__;
	int runningInTerminal = 0;
	if (isatty(STDIN_FILENO)){ runningInTerminal = 1;}

	// Checking if too few args
	if (numArgsIn == 1){
		myPerror(F_SYSOBJM, FF, "Invalid number of args provided. At least 1 is required, 0 were provided. Printing empty string");
		if (runningInTerminal == 1){ helpMenu();}
		puts("");
		return 1;
	}
	
	// Checking if is help menu
	if (isHelpMenu(argvIn[1]) == 1){
		if (runningInTerminal == 1){ helpMenu();}
		puts("");
		return 1;
	}

	// Checking if too many args
	if (numArgsIn > 3){
		myPerror(F_SYSOBJM, FF, "Invalid number of args provided. No more than 2 are accepted. Printing empty string");
		if (runningInTerminal == 1){ helpMenu();}
		puts("");
		return 1;
	}

	return 0;
}

// This function checks the contents of the vars to make sure they're right
// This function behaves differently if this is called from a script or the terminal
// 	-if from the terminal, help menu is displayed
int argContentsValid(int numArgsIn, char* argvIn[]){
	// Vars
	char* FF = (char*) __func__;
	int runningInTerminal = 0;
	if (isatty(STDIN_FILENO)){ runningInTerminal = 1;}
	
	// Checking if there is 1 arg and it's a valid number (>0)
	if (numArgsIn == 2 && isInt(argvIn[1]) == 1 && atoi(argvIn[1]) < 0){
		myPerror(F_SYSOBJM, FF,	"Invalid arg at pos 1; the value is < 0. Printing empty string");
		if (runningInTerminal == 1){ helpMenu();}
		puts("");
		return 1;
	}

	// Checking if there is 1 arg and it contains the "-f" option
	if (numArgsIn == 2 && strcmp(argvIn[1], "-f") == 0){
		myPerror(F_SYSOBJM, FF, "Invalid arg at pos 1; optional arg '-f' provided, but a 2nd required arg wasn't provided. Printing empty string");
		if (runningInTerminal == 1){ helpMenu();}
		puts("");
		return 1;
	}

	// Checking if there are 2 args and if the 1st if the '-f' flag
	if (numArgsIn == 3 && strcmp(argvIn[1], "-f") != 0){
		myPerror(F_SYSOBJM, FF, "Invalid args sequence. 2 args were provided, but the 1st wasn't the required '-f' flag. Printing empty string");
		if (runningInTerminal == 1){ helpMenu();}
		puts("");
		return 1;
	}

	return 0;

}

// INPUT STRUCTURE---------------------------------------------------------------------------------
//< -f> [int id] || < -f> [char* filePathOrName]
// -f :- for file path literal (not symlink)
// id :- returns the path of the symlink associated with that id. If the file is a symlink at that id, will just return symlink path
// 		 With -f option, returns the file path (not symlink) of that id found in filesAndFolders.csv
//filePathOrName :- returns the id of the symlink associated with the file path or file name. If the file is a symlink at that id, will just return id of that file 
//		The -f option will return the id of the file itself (not the symlink)
//-------------------------------------------------------------------------------------------------
int main(int argc, char* argv[]){
	// Vars
	char* FF = (char*) __func__;

	// Data Validation-------------------------------------------------------------------------
	int numArgsValidBool = numArgsValid(argc, argv);
	if (numArgsValidBool != 0){ return numArgsValidBool;}
	numArgsValidBool = argContentsValid(argc, argv);
	if (numArgsValidBool != 0){ return numArgsValidBool;}
	//-----------------------------------------------------------------------------------------

	// Vars
	char* filePath = NULL;
	int fileId = -1;

	// Running Functions-----------------------------------------------------------------------
	
	//if 1 arg and an int, find file path of symlink associated with file path
	if (argc == 2 && isInt(argv[1]) == 1){filePath = getSymLinkById(atoi(argv[1]));}
	//if 1 arg and not an int, get id of symlink associated with file
	else if (argc == 2 && isInt(argv[1]) == 0){ fileId = getSymLinkIdFromFileName(argv[1]);}
	//if 2 args and 2nd arg is an int, find file path of literal id, not symlink
	else if (argc == 3 && isInt(argv[2]) == 1){ filePath = getFileById(atoi(argv[2]));}
	//if 2 args and 2nd arg isn't an int, find id of literal file path, not symlink
	else if (argc == 3 && isInt(argv[2]) == 0){ fileId = getIdFromFile(argv[2]);}
	//-----------------------------------------------------------------------------------------


	// Checking Function Output----------------------------------------------------------------
	
	//if both string and int vars haven't changed
	if (filePath != NULL && strlen(filePath) == 0){ 
		myPerror(F_SYSOBJM, FF, "WARNING: No match found in the database. Printing empty string");
		free(filePath);
		return 1;
	}

	//if file path isn't null
	else if (filePath != NULL){
		printf("%s\n", filePath);
		free(filePath);
	}
	
	//if file id hasn't changed
	else if (fileId == -1){ 
		myPerror(F_SYSOBJM, FF, "Error finding file name/path or file id. Printing empty string");
		puts("");
		return 1;
	}
	
	//if file id didn't find a match
	else if (fileId == -2){
		myPerror(F_SYSOBJM, FF, "WARNING: getIdFromFile() function didn't find a match. Printing empty string");
		puts("");
		return 1;
	}

	//if file id isn't an error value, then print it
	else{ printf("%d\n", fileId);}

	return 0;

}
