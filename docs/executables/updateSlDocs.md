# UPDATE SYMLINKS MANUAL #
- - -

### NAMES ###
- - -

updateSl.sh

### SYNOPSIS ###
- - -

updateSl.sh \< -c | --create\> \<fileId | fileName | filePath\>

updateSl.sh \< -u | --update\> \<fileId | fileName | filePath\> \<new symlink num\>

### DESCRIPTION ###
- - -

This program updates or creates a symlink for a file and updates all corresponding dependencies. Dependencies:

- filesAndFolders.csv
- removing the old symlink (if updating existing symlink)
- updating all files (in the files and folder csv) that pointed to old symlink (if updating existing symlink)

### OPTIONS ###
- - -

-c, --create	:- Creates new symlink, but only if the previous symlink value is 0

-u, --update	:- Updates symlink. Can also be used to remove a library (setting symlink id/arg 3 to 0)

### ARGUMENTS ###
- - -

fileId      :- id of file in filesAndFolders.csv

fileName    :- name of file

filePath    :- path of file (including file name)

newSymLinkNum   :- new symlink number

### MODES ###
- - -

create  :- create a new symlink

update  :- updates the existing value of a symlink

### EXAMPLES ###
- - -

\<binNameOrAlias\> -u 1 4		    :- will update file with id 1 to point to symLink\_4.sl

\<binNameOrAlias\> -u myScripts 4	:- will update file with name to point to symLink\_4.sl

\<binNameOrAlias\> --update 1 0	    :- will update file with id 1 to point to symLink\_0.sl and remove the symlink

\<binNameOrAlias\> -c 1		        :- will create a symlink with id 1 greater than the last symlink that points to file with id 1

### NOTES ###
- - -

- If the existing symlink number for that file is 0, the program won't update anything
- symLink\_0.sl points to /dev/null, and a lot of files point to that
- In order to protect against updating the symlinks for the wrong files, updating that symlink number is not allowed
- If the new symlink is 0, no dependencies will be updated for the reasons mentioned above

### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- src/symLinkCreateAndUpdate/updateSL.sh


	
