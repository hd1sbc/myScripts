# STANDARDS, CONVENTIONS, AND FORMATS FOR COMMENTS MANUAL #
- - -

## Top-Level Function Comments ##
- - -

- **Purpose:** to describe what the function does at a high level
- **Location:** above the start of the function
- **Spacing:** space after comment initializer
- **Capitalization:** capital first letter
- **Subpoints:** new line, tab after comment initializer followed by "- ". Capital first letter

## Bash Additional Function Comments ##
- - -

- **Purpose:** to describe the parameters to function
- **Location:** above the start of the function, below top-level function comment
- **Spacing:** no space after comment initializer, followed by a semi-colon and space
- **Content:** Args: 
    - For every arg, add the argument number followed by " - " then descriptive name in camel case (unless very long)
        - If the comment needs more description, add it in parentheses
    - Each argument should end with ';'. Older code might end with ','

## In-Function Comments ##
- - -

### Comments For Small Bits Of Code###
- **Definition:** comments associated with 1-3 lines of code
- **Location:** to the right of the code or just above it
- **Spacing:** no space after comment initializer
- **Capitalization:** no capitalization of 1st letter
- **Content:** whatever needs to be said. If multi-line comment, follow the same pattern

### Comments For Medium Bits Of Code###
- **Definition:** comments associated with 4-9 lines of code
- **Location:** above the first line of the block of code
- **Spacing:** space after comment initializer is optional
- **Capitalization:** capitalization of 1st letter is recommended, but up to me
- **Content:** whatever needs to be said. If multi-line comment, all other lines follow "Small Bits Of Code" rules

### Comments For Large Bits Of Code###
- **Definition:** comments associated with 10+ lines of code
- **Location:** above the first line of the block of code
- **Spacing:** space after comment initializer
- **Capitalization:** capital 1st letter
- **Content:** General statement about its purpose. If multi-line comment, all other lines follow "Small Bits Of Code" rules

## Code Block Seperators ##
- - -

### "-" ###
- **Definition:** lines of "-"
- **Location:** next to a comment for big or medium bits of code. No space after end of comment
- **Length:** up to 99th column (in vim, cursor should be at column 100), irregardless of tabs
- **Usage:**
    - to segment blocks of code to make it easier to read and see how they connect
    - to segment top level blocks of standardized code
        - global variables
        - sourcing

### "- -" ###
- **Definition:** lines of "- -"
- **Location:** next to a comment for medium bits of code. No space after end of comment
- **Length:** up to 99th column (in vim, cursor should be at column 100), irregardless of tabs
    - If 99th column ends in a space, leave it
- **Usage:** to break up segments of code already withing a block of "-". Otherwise the same.

### "+" ###
- **Definition:** lines of "+"
- **Location:** next to a comment for big bits of code. No space after end of comment
- **Length:** up to 99th column (in vim, cursor should be at column 100), irregardless of tabs
- **Usage:** for largest blocks of code

### "=" ###
- **Definition:** lines of "="
- **Location:** above and below a comment for big bits of code or code with lots of functions. Also used in one line at the end of block of code. Comment must be in all caps
- **Length:** up to 99th column (in vim, cursor should be at column 100), irregardless of tabs
- **Usage:** for large blocks of code or standardized blocks of function code (usually in header files)

## Global Code Blocks ##
- - -

### Global Vars ###
- **When:** in all files (unless files were seperated out and share a set of global vars)
- **Where:** at the top of files. For c files, below include statements
- **Content:**
    - comment initializer with no leading whitespace followed by a space, followed by "GLOBAL VARS" (all caps), followed immediately (no space) by "-" until cursor reaches 100th column (last "-" will be at column 99)
    - At the end of global vars section, conclude with comment initializer with no leading whitespace followed by "-" until the cursor reaches the 100th column (last "-" will be at column 99)

### Sourcing ###
- **When:** in all files where a global variable needs to be assigned a value after compilation or ensure file values are valid
- **Where:** at the top of files below global vars section
- **Content:**
    - comment initializer with no leading whitespace followed by a space, followed by "SOURCING" (all caps), followed immediately (no space) by "-" until cursor reaches 100th column (last "-" will be at column 99)
    - At the end of global vars section, conclude with comment initializer with no leading whitespace followed by "-" until the cursor reaches the 100th column (last "-" will be at column 99)

### File Variable For Error Message ###
- **When:** in all production files (test files don't need it)
- **Where:** at the bottom of the global vars section
- **Content:**
    - comment initializer with no leading whitespace followed by a space, followed by "Error Msg Constant Vars" or "Error Message Constant Vars" ("Vars" can be replaced with "Var")
    - file name var follows below it (see Standardized Variables section below). Global vars ending "-" comment follows immediately below that


### Standardized Variables ###
- **FILE NAME:** all files must have a way to get the file name and/or path. The variable begins with "F\_" and is follwed by a shortened version of file name
- **FUNCTION NAME:** a variable to store the current function name. Must always be "FF"
