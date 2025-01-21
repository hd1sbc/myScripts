# INSERT TEXT IN FILE MANUAL #
- - -

### NAMES ###
- - -

insertTxtInFile

### SYNOPSIS ###
- - -

\<binNameOrAlias\> \<stringToAdd\> \< -l\> \<lineNum\> \<filePath\>					:- will add string at specific line num in file

\<binNameOrAlias\> \<stringToAdd\> \< -p\> \<patternString\> \< -f | -l | -a\> \< -b | -a\>	\<filePath\>	:- will ad string at line number(s) of matching pattern before or after each occurrence

### DESCRIPTION ###
- - -

This function inserts a given single or multi-line string into a file at either a given line number or at the first, last, or all occurrences of a pattern. All text after the given line number of occurrence(s) will be shift down a number of lines of the string

If using the "-p" option (occurrences of a pattern), the user can choose to add "before" the line or "after" the line. This option is not available for the \"-l\" option becuase the user can just increment by one to add after the line number.

The inserted text is, by default, added "before" the line number or pattern. This means it is actually inserted at the given line number (or line number of the matching pattern), but all code is shifted down. This gives the illusion that it was added "before" the line.

### OPTIONS ###
- - -

-l \<line number\>      :- insert at a specific line number

-p \<pattern\>          :- insert where there is a matching pattern

    -f                  :- insert at the first occurrence of a pattern

    -l                  :- insert at the last occurence of a pattern

    -a                  :- insert at all occurrences of a pattern

        -b              :- insert at the line of pattern (aka "before")
        
        -a              :- insert at the line after pattern

### EXAMPLES ###
- - -

\<binNameOrAlias\> \"abcd\\nefg\" -p \"hij\" -f -b \"someFilePath\"	:- will insert \"abcd\\nefg at the line num of the 1st occurrence \"hij\" and shift all subsequent lines (including line num of \"hij\") up one"

\<binNameOrAlias\> \"abcd\\nefg\" -l 10 \"someFilePath\" 			:- will insert \"abcd\\nefg\" at line 10 and shift all subsequent lines (including line num of \"hij\") up one"

### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- lib/sharedShellScripts/insertTxtInFile.sh
