# STANDARDS, CONVENTIONS, AND FORMATS FOR HELP MENU MANUAL #
- - -

## When To Use And Where To Put It##

- **When To Use:** with any primary file, regardless of file type, in the src directory that will either be associated with a alias or executable
- **Where:**
    - in a seperate function titled helpMenu, followed by some descriptor of the file name or what it does
    - in file with main function or effective main function, unless the help menu is exceptionally long and requires its own file

## Contents ##
- **Help Menu Descriptor:**
    +++\<Name of program (with spaces, all caps)\> - HELP MENU+++	Relative File Path: $0
- **Purpose:**
    Purpose: \<quick summary of what it does\>
- **Details:** (optional)
    Details: <more details if the purpose would be too long>
- **Names:** (optional)
    Executable File Name: <bin name>
	Aliases: {otherAliases}
- **Options:**
    Options: (if no options, write "none" on same line as this. Ex: Options: none)
        option1		:- Explanation (with capital first letter)
- **Usage:**
    Usage:
	    \<generic format of usage\> \[options\] \<requiredArgs\> | \{mainArgs\}
- **Examples:**
    Examples:
        \<binName\> | \<binName\> \<requiredArgs\>	:- Default, no arguments provided. If necessary, explain what it does or what minimum args provided does
		\<binName\> \[someOption\] \<requiredArgs\>	:- explain what it does
- **Notes:** (optional)
		Notes:
			[any important notes]
	

## Notes ##
- Name of program should be in plain English with spaces, not in camelSpace. Extend the title to make it more understandable
- Deatils section is optional and should really only be used if purpose would be too long
- If no options are required, show that, otherwise, show an example with required options
- If no man page, use "--more" option. If no "--more" is necessary, ignore it all together
- always use ":-" to explain what the example does
- All ":-" should line up
