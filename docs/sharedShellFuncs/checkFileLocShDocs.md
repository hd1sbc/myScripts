# CHECK FILE LOCATION IN BASH MANUAL #
- - -

### NAMES ###
- - -

checkFileLocs

### SYNOPSIS ###
- - -

checkFileLocs \<name of file calling the function\>

### DESCRIPTION ###
- - -

This program finds all variables in a file ending with Loc, loc, Dir, or dir and confirms that they are valid locations. If one or more is invalid, it exits.

This program doesn't check if it's input argument is a valid file, but checks the contents of the input argument to find the file location variables and verify they exist.

### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- lib/sharedShellScripts/checkFileLocSh.sh 

