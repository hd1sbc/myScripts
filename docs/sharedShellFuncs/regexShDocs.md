# REGEX IN BASH MANUAL #
- - -

### NAMES ###
- - -

isHelpSh

regexMatchSh

isIntSh

isPosIntSh

### SYNOPSIS ###
- - -

isHelpSh \<string\>

regexMatchSh \<pattern compatible with sed\> \<string\>

isIntSh \<string\>

isPosIntSh \<string\>


### DESCRIPTION ###
- - -

These functions perform various regex matching. Their names are indicative of what they do.

isHelpSh        :- determines if the string input contains only \"-h\" or \"--help\"

regexMatchSh    :- uses sed command to see if a string matches a regex pattern. If the regex input contains \"/\" or \"|\" without a '\\' before it, one will automatically be added so it is compatible with sed

isIntSh         :- determines if string is an integer (includes negative numbers)

isPosIntSh      :- determines if string is a positive integer

### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- lib/sharedShellScripts/regexSh.sh
