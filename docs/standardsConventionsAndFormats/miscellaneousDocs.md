# MISCELLANEOUS - ERROR MESSAGES, CSV's, FILE EXTENSIONS, FUNCTION PARAMETER NAMES, INTERACTIVE OPTIONS MANUAL #

## Error Messages ##
**Format:** ===filePath - functionName===\[tab\] Message

## Function Parameter Names ##
**Naming:** for variables that might have the same or similar names elsewhere in the program, append the funciton parameter input name with "In"

## File Extensions ##
All files need a file extension. Valid file extension:
- .c
- .h
- .o
- .so
- .a
- .py
- .cpp
- .rb
- .pl
- .rb
- .sh
- .man
- .1
- .2
- .3
- .5
- .md
- .yaml
- .txt

## Interactive Options Standardization ##
- **y/n:**
    - Required to put "(y/n): " for any user input that is yes or no
	- Capital is the recommended. If neither is recommended, both are lowercase
	- Blank answer is always the recommended option. If no recommended option, default is yes.
		- This functionality can be overwritten in the case of sensitive operations

## CSV's ##
- First row is always header row with column names
- Column names:
    - no spaces, use "\_" instead
	- no spaces allowed anywhere in csv unless absolutely necessary
