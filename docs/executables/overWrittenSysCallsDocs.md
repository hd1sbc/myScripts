# OVERWRITTEN SYSTEM CALLS MANUAL #
- - -

### NAMES ###
- - -

loadOwSysCalls.sh

loadOwSysCalls.o

liboverWrittenSysCalls.so

### SYNOPSIS ###
- - -

loadOwnSysCalls.sh

loadOwnSysCalls.o

UNSET LD\_PRELOAD; LD\_PRELOAD=loadOwnSysCalls.so

### DESCRIPTION ###
- - -

This program contains code to overwrite the following Linux system calls:
- read
- rename
- rmdir
- unlink
- write

The program overwrites the functions above to include functionality to prevent or warn users about performing those actions on files in the repository directory, based on the permissions set for each file by in the filesAndFolders.csv (specifically in the rwe and rmd columns). It also checks if csv files are valid databases before opening a file and before writing a file.

The columns "rwe" and "rmd" in filesAndFolders.csv refer to the permissions for each file. The rwe column refers to the same permissions (read, write, and execute) as in Linux and the value follows the same format as Linux permissions (0-7). The rmd column stands for rename, move, and delete. The values in its column follows the same integer naming conventions as Linux rwe permissions, with the order of rmd matching that of rwe. For example, a value of 6 in the rwe column grants read and write permissions. A value of 6 in the rmd column grants renaming and moving permissions.

The program will only present a warning message if the source of the function call is from the terminal or gui application. If the function call is from a program in the repository, it will not throw a warning message.


### NOTES ###
- - -

**CAUTION!!! THIS PROGRAM CAN CRASH YOUR DEVICE**
- Even if preloading just in the current shell, overwriting the system calls can affect every aspect of the computer and remove the ability to access anything
- The ability to set LD\_PRELOAD on many operating systems is restricted for obvious security reasons. Therefore, you might need to take complex and risky actions to enable this

### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- src/overWrittenSysCalls
