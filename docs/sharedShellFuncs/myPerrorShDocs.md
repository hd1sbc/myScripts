# MY PERROR IN BASH MANUAL #
- - -

### NAMES ###
- - -

myPerrorSh

### SYNOPSIS ###
- - -

myPerrorSh \[returnVal\] \[string 1 (usually file name or path)\] \[string 2 (usually) function name\] \[string 3 (usually message)\]

### DESCRIPTION ###
- - -

This function prints an error message to standard error in a standardize format. The expected format (although not enforced) contains the file name or path, the function name, and a message. The standardized output of this function is:

===filePathOrName - functionName===\[tab\] \[message\]

If given an integer in the first argument, the message (in the format above) will only be printed if the value isn't 0. This is used for return values when I want to print an error message if the value isn't 0 (the function returned an error). However, this is usually only used when there is an error, but want the program to continue.

This function

### OPTIONS ###
- - -

integer first argument       :- if the 1st argument is an integer, the message will only appear if the value isn't 0

### EXAMPLES ###
- - -

myPerrorSh $F\_SOMEFUNC $FF "my message"                :- will print error message

myPerrorSh $returnVal $F\_SOMEFUNC $FF "my message"     :- will only print if returnVal != 0

### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- lib/sharedShellScripts/myPerrorSh.sh
