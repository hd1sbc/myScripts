#include "userInputWriteToFile.h"
#include "updateFAndF.h"
#include "userInputMissingFiles.h"
#include "userInputUnmarkedFiles.h"

// GLOBAL VARIABLES--------------------------------------------------------------------------------
// Error Msg Constant Vars
char* F_WRITETOFANDFCSV = (char*) __FILE__;
//-------------------------------------------------------------------------------------------------

int writeToFandFCsv(char* filesListIn, struct filesAndFoldersDbObj** dbIn, char* csvPathIn){
	// Vars
	char* FF = (char*) __func__;
	char* filesList = filesListIn;
	struct filesAndFoldersDbObj** db = dbIn;
	char* csvPath = csvPathIn;
	int returnVal = 0;


	puts("Choose which option: ");
	puts("	1) View new list of files and folders to be added to permenantly");
	puts("	2) Permanently write changes to file");
	puts("	3) Exit and don't permanently write changes to file");

	char option;
	scanf(" %c", &option);
	if (option == '1'){ myPrint(db);}
	else if (option == '3'){ puts("No changes written to file"); return 0;}
	else if (option != '2'){ puts("Invalid input. Not writing anything to file"); return 0;}

	puts("Are you sure you want to permanently write changes to files? (Y/n)");
	char write;
	scanf(" %c", &write);
	if (write == 'Y' || write == 'y'){ 
		// Getting header/column row
		char* writeToFileStr = getRow(0, filesList);
		if (writeToFileStr == NULL){
			myPerror(F_WRITETOFANDFCSV, FF, "getRow() function returned a null value. Returning -1");
			return -1;
		}
		
		// Add newline char
		writeToFileStr = myAppend(writeToFileStr, "\n");
		if (writeToFileStr == NULL){
			myPerror(F_WRITETOFANDFCSV, FF, "myAppend() function returned a null value. Returning -1");
			return -1;
		}
		
		// Convert database struct arr to string in csv-format
		char* dbAsCsv = convert(db, "string");
		if (dbAsCsv == NULL){
			myPerror(F_WRITETOFANDFCSV, FF, "convert() function returned a null value. Returning -1");
			return -1;
		}
		
		// Add database as csv-formatted string to writeToFileStr variable
		writeToFileStr = myAppend(writeToFileStr, dbAsCsv);
		if (writeToFileStr == NULL){
			myPerror(F_WRITETOFANDFCSV, FF, "myAppend() function returned a null value. Returning -1");
			return -1;
		}

		// Getting contents for backup file and writing that to the backup csv location
		char* backUpWriteToFileStr = createStr();
		backUpWriteToFileStr = myAppend(backUpWriteToFileStr, filesList);
		returnVal = writeToFile(filesAndFoldersBackupLoc, backUpWriteToFileStr);
		if (returnVal != 0){
			myPerror(F_WRITETOFANDFCSV, FF, "writeToFile() function returned an error value. Returning -1");
			return -1;
		}
		returnVal = writeToFile(csvPath, writeToFileStr);

		// Local Free's
		free(writeToFileStr);
		free(backUpWriteToFileStr);
		free(dbAsCsv);
	}
	else if (write == 'N' || write == 'n'){ puts("No changes written to file");}
	else{ puts("Invalid input. No changes written to file");}	
	
	return returnVal;
}
