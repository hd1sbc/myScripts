# UPDATE FILES AND FOLDERS MANUAL #
- - -

### NAMES ###
- - -

updateListOfFAndFMain.o

### SYNOPSIS ###
- - -

updateListOfFAndFMain.o

### DESCRIPTION ###
- - -

This program is an interactive program to update the list of files and folders in the current repository. The program:

- Finds all files listed as in use in the csv and checks to see if they are not in the expected location
    - If one or more files isn't found, the user is presented with the option to look for the file(s)' base name in the system
        - If found, the path and old path columns are updated. The found file(s) are removed from the list of missing files
    - If one or more missing files are found, the user is presented with the option to mark those files as not in use. These files are not deleted from the csv, but the "inUse" column is set to 0
- Finds every file in current repository directory and checks to see if it is in the csv
    - For all files not listed in the csv, add it to the csv
- Confirms user wants to make changes
- Creates a backup of the previous csv (removing the old backup csv) and adds the changes to the existing csv

### OPTIONS ###
- - -

none - this is an interactive program

### IMPORTANT NOTES ###
- - -

- This program uses multi-threading. It's support on various operating systems may vary.


### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- src/updateListOfFilesAndFolders
