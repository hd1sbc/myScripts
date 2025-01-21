# AUTO EXECUTE MANUAL #
- - -

### NAMES ###
- - -

autoExec.sh

### SYNOPSIS ###
- - -

autoExec.sh \[options\] \<sourceCodeFilePath\> [args for sourceCodeFilePath]

### DESCRIPTION ###
- - -

#### ALL PROGRAMMING LANUAGES ####
This program compiles (if needed) and executes files using the compiler/interpreter associated with file extension in the file path argument (do not need to specify file type of compiler/intepreter). This program supports a variety of different programming languages:

- shell/bash (.sh)
- c (.c)
- c++ (.cpp)
- c# (.cs), but only on some platforms
- java (.java)
- perl (.pl)
- python (.py)
- ruby (.rb)

For programming languages with a compilation step, the compiled object files are stored in lib/compiled/ directory.

### OPTIONS ###
- - -

-u, --no-auto-update		:- turns off auto-update for libraries and header files for c programs only

--special=\<type\>	    	:- compiles and/or executes in a special way based on type

- type options: 
    - overwrittenSysCalls	:- special compilation (and no execution) for overwrittenSysCalls.c file"

### MODES ###
- - -

This program has 8 modes (1 for each programming language) and 4 sub-modes. The 4 sub-modes are all for c files. Please see the execCDocs.md for more details

- shell/bash (.sh)
- c (.c)    :- all object files are executed using valgrind
    - complex c programs with libraries     :- if an included header file is associated with a library not in the standard c library, this mode checks and auto-updates as necessary all libraries, included libraries, object files, and header files associated with the library or any of its dependencies. It will also check and auto-update any non-standard included header files that are not associated with a library
    - complex c programs without libraries  :- if an included header file is not in the standard c library but all non-standard includes are not associated with a library, then check and auto-update all files associated with header file
    - no main function file                 :- if the file does not contain a main function, a header file will be created or updated for that file, then it will be compiled use the "-c" option in gcc. It will not be executed
    -simple c program                       :- if the file contains a main function and all include statements are in the c standard library, then it will run without checking any files
- c++ (.cpp)
- c# (.cs), but only on some platforms
- java (.java)
- perl (.pl)
- python (.py)
- ruby (.rb)

### EXAMPLES ###
- - -

\<binNameOrAlias\> fileName.sh		            :- this will execute a bash file

\<binNameOrAlias\> fileName.c arg1 arg2	        :- this will execute a c program with its arguments

\<binNameOrAlias\> -u fileName.c arg1 	        :- this will execute a c program with its arguments without updating libraries or header files

\<binNameOrAlias\> --special=overWrittenSysCalls fileName.c :- this will compile (but not execute) the given file in a way that is designed for use for preloading overwritten system calls. This is designed for use with only overWrittenSysCalls.c

### NOTES ###
- - -

- c programs contain a far more sophisticated process in this program. Please see execCDocs.md for details

### SEE ALSO ###
- - -

- execCDocs.md

### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- src/executeCode/
