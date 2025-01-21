#!/bin/bash

# This function gets a file name from a (possible) file path. It includes the file extension
getFileNameSh(){ 
        #regex: remove everything before last '/'
        echo $(echo "$1" | sed 's/.*\///g') 
}

# This function gets a file's extension from a file name or path
#Args: 1 - file path (including file name)
getFileExtSh(){
        #regex: removes everything before (and including) '.'
        echo $(echo "$1" | sed 's/.*\.//g') 

}

# This function gets the file's name without the extension
#Args: 1 - file path (including file name)
getFileNameNoExtSh(){
        fileName=$(getFileNameSh $1)
        #regex: removes everything after (and including) '.'
        echo $(echo "$fileName" | sed 's/\..*//g') 
}

# This function gets the file's path without the file's name and extension. The result DOES NOT include a trailing '/'
#Args: 1 - file path (including file name)
getFilePathNoFileNameSh(){
        #regex: removes everything after (and including) the last '/'
        echo $(echo "$1" | sed 's/\/.*//g')
}

