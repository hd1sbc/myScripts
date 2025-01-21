# myScripts - Linux Utils #

## Author ##

**Michael Rudin**

## Overview ##

This ~260 file, ~19,000 lines of code (which doesn't include txt files, man pages, and readme's) repository contains a host of files, functions, scripts, and programs with a variety of different purposes that fall under 5 main categories:
- Custom-Built C Database
- Code Maintenance And Organization
- Code Generation
- Shell And Command Line Automation And Customization
- Ease Of Coding

## Important Notes ##

Given the scope of this repository, this readme will be very limited. For more details about the various files, programs, scripts, etc., please look at the in-depth documentation files found in the docs directory. In addition, source code files are heavily commented. 

This repository is not the most up-to-date version; it is a personal repository that I decided to recently make public. I have not yet built automation to remove any private information from the private version of the code base, which makes pushing changes here a manual, time consuming process.

## Breakdown of Categories ##

**Custom-Built C Database**
- I built a database management program that creates and manages databases. It generates c code which can be used to manipulate the file contents. See docs/executables/databaseDocs.md for details

**Code Maintenance And Organization**
- I built a host of functions and programs that help keep the code tidy, legible, easy-to-read, and easy to find. This includes libraries, a program to auto-create & auto-update header files, and a program to auto-update libraries and any associated code. See docs/executables/autoExecDocs.md and docs/executables/execCDocs.md for details of the last two points.

**Code Generation**
- I have programs that generate code to automate various processes. These include programs to create new symlinks, a program to create the repoStats.txt file, a program to create & update makefiles, and more. In-depth documentation for these programs can be found in the docs folder and the source code can be found in the src directory.

**Shell And Command Line Automation And Customization**
- I have programs that make it easier to use the terminal/shell as well as customizations for ***the best text editor and the only program I will every need to write code:*** vim. 

**Ease Of Coding**
- I have a host of c functions, and some shell functions, in the lib folder dedicated to making coding easier. Some of these include:
- a function (myFree) to automate free'ing in c, which can free various data types and double pointer versions of them as well
- a convert function (convert) to convert between across built-in data types and custom-made structs
- a function (myRealloc) to automate realloc'ing string and string arrays
- and many, many, many more (check out the entire lib directory, as well as the man pages (docs/man/3LibraryCalls directory))
