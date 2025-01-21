#!/bin/bash
source "$(findPath -f 312)"  #regexSh.sh

# This function prints an error message to stderr by taking the file name, function name, and any number of following args as inputs and formatting them
# If the first arg is an integer, then an error message will only be displayed if the arg != 0
myPerrorSh(){
	# Vars. Not using $1, and $2 directly cuz I need to shift 2 (or 3, depending on whether or not the first arg is an int) to get all args after the first 2 (or 3)
	arg1=$1
	arg2=$2


	if [ "$(isIntSh $arg1)" == 1 ]; then 
		arg3=$3
		shift 3

		if [ ! "$arg1" == "0" ]; then
			echo "===$arg2 - $arg3===	$@" >&2
		fi
	else
		shift 2
		echo "===$arg1 - $arg2===	$@" >&2
	fi
}

