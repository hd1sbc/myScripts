# FIND PATH MANUAL #
- - -

### NAMES ###
- - -

findPath
findSysObjMain.o

### SYNOPSIS ###
- - -

\<binNameOrAlias\\>  \[options\] \<file identifier\\>

### DESCRIPTION ###
- - -

This program finds:

- The file path (including file name) given a file id
- The symlink path associated with a file based on file ID
- The file ID based on file name or path
- The file ID of the associated symlink based on file name or path

The program uses filesAndFolders.csv values in order to find these values.

### OPTIONS ###
- - -

-f      :- finds the literal file path (including file name) or id instead of the associated symlink path or symlink id

### MODES ###
- - -

- Find Path     :- finds the file path or symlink path when the file identifier input argument is an integer
- Find Id       :- finds the id of the file name (or path) or id of the associated symlink when the file identifier input argument is not an integer

### EXAMPLES ###
- - -

\<binName\> 1             :- Returns the symlink path for the file whose ID in the files and folders DB is 1

\<binName\> 10            :- Say the file with the ID of 10 is a symlink, then this will return the file path of that file, not its associated symlink

 \<binName\> -f 1          :- Returns the file path for the file whose ID in the files and folders DB is 1. It doesn't return the symlink path

 \<binName\> -f 10         :- Say the file with the ID of 10 is a symlink, this does the same as \<binName\> 10

 \<binName\> myScripts     :- Returns the ID of the symlink within the files and folders DB that is associated with that file path or name

 \<binName\> symLink\_0.sl  :- Return the ID of that file within the files and folders DB

 \<binName\> -f myScripts  :- Returns the ID of the file (not the symlink) within the files and folders DB

### NOTES ###
- - -

**CAUTION!! THIS PROGRAM CAN BREAK YOUR DEVICE**

*Circular References And Prevention*

- findSysObjMain.o, findSysObjMain.c, and the other files in lib/findFilesAndFolders/ directory contain references to its own object file. This creates a circular loop.
- To avoid this, a previous version of findSysObjMain.c that contains the hardcoded values is statically compiled. That object file is then set as findPath.
    - Since it is statically compiled and doesn't contain circular references, it can be used to compile the version of findSysObjMain.c that contains the findPath.
- In order to make this new version usable throughout the system, it needs to be compiled statically as well.

*Shell Startup*

- This program's use in .\*rc files (like bashrc and zshrc) must be compiled correctly or it can break the entire shell, including its startup
- This program's use across almost all programs require it be compiled and run perfectly. One should always keep a backup of a working version of the statically compiled object file

### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- lib/findFilesAndFolders
