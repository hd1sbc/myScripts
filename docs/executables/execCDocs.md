# EXEC C MANUAL #
- - -

### NAMES ###
- - -

none - part of autoExecute.sh

### Defintions ###
- - -

- Implementation Files      :- files with all source code and ending with ".c" (not ".h")
- Source Header Files       :- header files associated with implementation files
- Library Header Files      :- header files associated with a library file, which just includes the source header files for all of its source code members as well as the library header files for any libraries included in it
- Included Library          :- a library that is included in another library
- Object File               :- compiled source code
- Share Object File         :- compiled libraries
- Non-Standard Include      :- an include statement that points to a library not in the standard c library. They are differentiated by  using \" instead of \<\>
- Non-Standard Library      :- a library I created and not part of the c standard library
- Non-Main Files            :- c implemenation files that do not contain a main function. They are designed to be part of a library
- Main-Function Files       :- c implementation files containing a main function
- Simple C Program          :- a c implementation file that doesn't contain any non-standard libraries and doesn't include any non-standard headers
- Complex C Program         :- a main-function file in which at least one of the includes points to a header file not in the c standard library
- Complex C Program No Lib  :- a type of complex c program in which none of the includes points to a library header file
- Complex C Program Lib     :- a main-function file in which at least one of the includes points to a library header file

### Outline Of How It Works ###
- - -

The program is used to automatically create and/or update related ".c", ".h", ".o", and ".so" files for c programs, then compiles and runs them. It has a 4 step process:

- Determine the type of file
- Find associated files based on file type
- Update dependencies if needed by comparing the last modified dates of the associated files and determine if it needs to be updated
- Compile and run the program

### Details Of How It Works ###
- - -

#### Determine the type of file ####

The program determines, based on the existince of a main function, what type of c file the input file path is. The valid options are main-function files or non-main-function files.

#### Find Associated Files ####

- Extract all non-standard includes
- For each file base name, determine if it is a source header file or library header file
    - Find the file base name of each include statement
        - Remove the "#include \"" and trailing "\"" from each include line
        - Remove the ".h" (or whatever file extension)
    - See if the base name with extension ".mk" exists in the build directory. If so, it is a library header file. Otherwise, it is a source header file

#### Update Dependencies ####

- For each library header file (if one exists), open the associated makefile in the build directory
    - Extract all object files the line containing the OBJ\_FILES variable
    - For each object file, find the associated implementation file and source header files
        - If the implementation file was modified more recently than the header file, update the header file (see details in section "Create And Update Header File"
        - If either the implementation file or the header file were modified more recently than the object file, recompile the object file and the makefile
    - Extract all included libraries found in the line containing the INCLUDED\_LIB\_NAME variable
    - For each included library, find its associate makefile and recursively run the outer most for loop until there are no more included libraries
- If any changes were made to any of the associated files in the library, recompile the makefile
- For each non-standard header file not pointing to a library header file, create and/or update source header file

#### Create And Update Header ####

*Purpose:* to update header files to include any new additions in global variables, new functions, changes in function definitions, and more

- If a non-main file, then extract all global variables, structs, typedefs, function definitions, defines, and includes
- Where necessary, manipulate the strings to match formatting
    - For example, function definitions would remove anything after (and including) "{" on the function definition line and replace with ';'
- If a source header file already exists, extract the defines from the header file and remove all contents of the header file
- Add extracted contents from non-main file to updated header file.
    - If header file previously existed, replace the extracted defines from the implementation file with the extracted defines from the source header file. This is a legacy feature that I chose to retain, even if it makes things a bit more complicated
- Recompile non-main file

#### Compilation And Execution Of Original File ####

**Simple C Program**

- The program is compiled using gcc with only basic options
- The program is then run with valgrind

**Complex C Program Lib**

- After all dependencies have been updated, the file is compiled using gcc
- All object files from all of the libraries are included in the compilation
- All shared object files are included
- All object files associated with non-library header files are included, if a non-library header file was included
- The path to look for header files is set to the symlinks and include directories
- The path to look for shared object files is set to lib directory
- The compiled object file will be placed in the compiled directory
- The object file is executed using valgrind

**Complex C Program No Lib**

- After all dependencies have been updated, the file is compiled using gcc
- All object files associated with non-library header files are included
- The path to look for header files is set to the symlinks and include directories
- The compiled object file will be placed in the compiled directory
- The object file is executed using valgrind

**Non-Main File**

- After all dependencies have been updated, the file is compiled using gcc
- All object files associated with non-library header files are included
- The path to look for header files is set to the symlinks and include directories
- The compiled object file will be placed in the compiled/compiledHeaders/ directory
- The object file is NOT executed


### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- src/executeCode
- autoExecDocs.md
