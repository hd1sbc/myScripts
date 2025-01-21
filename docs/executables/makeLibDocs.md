# MAKE LIBRARY MANUAL #
- - -

### NAMES ###
- - -

makeLib.sh

### SYNOPSIS ###
- - -

makeLib.sh \[options\] \<library name without extension\> \<object or shared object file\> [additional object or shared object files]

### DESCRIPTION ###
- - -

This program creates a new or updates an existing makefile for a library and adds the given object files and/or shared object files. If a library with the library name argument already exists, it will update the existing one. Otherwise, it will create a new one

### OPTIONS ###
- - -

--no-make       :- don't run the \"make\" command. Used for when you add an object file that doesn't exist yet. If you include this option, it must be the first argument


### EXAMPLES ###
- - -

\<binNameOrAlias\> libName object.o object2.o sharedObject.so

### NOTES ###
- - -

- The library name should not include the beginning 'lib' or '.so' extension. If you include either, the extension will be auto-removed, but you will be asked if you want to remove the 'lib' part"

### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- src/makeFunctions
