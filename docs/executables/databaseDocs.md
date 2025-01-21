# DATABASE MANUAL #
- - -

### NAMES ###
- - -

database.sh

### SYNOPSIS ###
- - -

database.sh [interactive options]

### DESCRIPTION ###
- - -

#### Database Management ####
This is a custom database management software that generates associated files and c code to be used for manipulation of contents of associated tables. This interactive program contains the ability to:

- create a database
- remove a database
- edit structural aspects of an existing database
    - renaming a column
    - removing a column
    - adding a column
    - changing a column's default value
    - changing a column's data type

Any changes made to the structure of the database will be automatically reflected in all associated files and code.

#### Database Management Accepted Values ####
The accepted values for the various components of the database are listed as the user nagivates the menus. Below is breakdown:

- Database Name:
    - name must only include Latin alphabet characters (case-insensitive). No numbers or special characters permitted
	- name must be less than 128 characters long
    - cannot have the same name as an existing database
- Column Names:
    - name must only include Latin alphabet characters (case-insensitive). No numbers or special characters permitted, including "\_"
	- name must be less than 128 characters long
	- column names cannot be duplicated
    - the first column (with column id of 0) will be auto-generated with column name: id, column data type: int, and column default val: -1. It is auto-generated and its values cannot be changed
- Column Data Types (must be exact matches, case sensitive):
    - int
    - bool
    - float
    - char
    - string
- Column Default Values: must be a valid value for that data type
    - strings can be empty
    - bool cannot have values other than 0 or 1
    - int can only contain integers (with a possible negative sign in front of it)
    - other values follow the same logic

#### Generated C Code Capabilities ####
The generated c code creates a seperate struct for each new database where each column is a member of the struct with the datatype associated with that column. Edits to the structural aspects of the database do not create a new struct; the existing one is updated to reflect the changes. For each database (and its associated struct), the generated c code provides the ability to:

- convert from a csv file (with matching columns and valid values) to an array of structs where each column is a member of that struct with the associated data type
- convert from a struct array to a csv-formatted string (which can be written to a file)
- add elements to a struct
- remove elements from a struct based on ID column (column 0)
- remove elements from a struct based on a value and a column number
- find all rows where a value in a given column matches a given equality (current valid equalities: =,!=, >, <, >=, <=)
- deep copy a struct and/or a struct array (any pointers (like char\*) in the struct will be copied into a new one)
- set struct member values where the name of the member is stored in a variable
- get values of a struct member where the name of the member is stored in a variable
- get the size of a struct array
- free a struct and/or a struct array

These function do not require the struct type to be specified. Using gcc generics, the data type for the struct/struct array parameter is automatically detected and the associated function with that struct is called.

Although written in c, the functions associated with the abilities above (as well as all c code in this repo) have been built with memory leak prevention as a priority, including rigorous testing with valgrind.

Following the use of this program (database.sh), the header files need to be updated. By using autoExec.sh function when running a c program with the database library included, the header files and libraries will be automatically updated to reflect the changes in the c code.

### OPTIONS ###
- - -

None. This program is interactive.

### MODES ###
- - -

The program has 4 modes and 5 sub-modes:

- create a database     :- will create a new database with a csv file (including header row), c code, and column information added to tableColTypeList.csv
- remove a database     :- will remove a database, its csv file, associated c code, and column information in tableColTypeList.csv
    - it will also list other files in the repo directory that contain the name of the database in case the user wants to delete it from those files not created associated with this program
- edit database         :- will provide options (listed below) to change structural components of an existing database. Values will be auto-updated in all associated c code files, csv, and tableColTypeList.csv
- do nothing (exit)     :- graceful exit

The "edit structural aspects of an existing database" has the following 5 sub-modes:

- renaming a column
- removing a column
- adding a column
- changing column's default value
- changing column's data type


### SEE ALSO ###
- - -

- autoExecDocs.md

### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- src/dbCreateAndManage/
- lib/databaseFunctions/
