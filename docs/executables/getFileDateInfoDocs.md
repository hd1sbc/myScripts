# GET FILE DATE INFO MANUAL #
- - -

### NAMES ###
- - -

getFileDateInfo.sh

### SYNOPSIS ###
- - -

getFileDateInfo.sh \<filePath1\> [filePath2]

### DESCRIPTION ###
- - -

#### Single File Input ####
Provides easy-to-read information about the last modified and last accessed dates of a file.

#### Double File Input ####
Provides easy-to-read information about the difference between last modified and last accessed dates of two files.

### OPTIONS ###
- - -

-h, --help  :- prints help menu

### MODES ###
- - -

This program has 2 modes:

- single file input     :- only one file path is provided. Compares against today's date
- double file input     :- two (and no more) file paths are provided. Compares the differences in dates between them

### EXAMPLES ###
- - -

\<binNameOrAlias\> file1		:- prints how long ago the file was last accessed and modified

\<binNameOrAlias\> file1 file2	:- prints how long ago the file1 was last accessed and modified compared to file2


### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- src/getFileDateInfo.sh

