#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
F_REGEXMATCHSH="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# This function echo's 1 if the arg 1 is -h or --help and echo's 0 otherwise
#Args: 1 - string to test
isHelpSh(){
	if [[ $1 =~ ^(-h|--help)$ ]]; then
		echo 1
	else
		echo 0
	fi
}

# This function echo's 1 if there is a match between a regex pattern (arg 1) and a string (arg 2) and 0 if not
#Args: 1 - regex pattern; 2 - string to match
regexMatchSh(){
	# Vars
	FF="${FUNCNAME[0]}"

	if [ ! $# -eq 2 ]; then
		echo "===$F_REGEXMATCHSH - $FF===	Invalid number of parameters. Two are requries, $# were provided. Echoing 0 to stdout (the following lines are printed to stderr) and returning 1" >&2
		echo "===$F_REGEXMATCHSH - $FF===	Usage: " >&2
		echo "===$F_REGEXMATCHSH - $FF===		$FF \"regexPattern\" \"string\"" >&2
		echo "===$F_REGEXMATCHSH - $FF===	Examples: " >&2
		echo "===$F_REGEXMATCHSH - $FF===		$FF \"^[0-9]\\+$\" \"1234\"" >&2
		echo "===$F_REGEXMATCHSH - $FF===		$FF \"/home/root/\" \"/home/*\"" >&2
		echo "===$F_REGEXMATCHSH - $FF===		$FF \"\\/home\\/root\\/\" \"\\/home\\/*\"" >&2
		echo "===$F_REGEXMATCHSH - $FF===	Notes: " >&2
		echo "===$F_REGEXMATCHSH - $FF===		-This program uses sed command with the template: echo \$2 | sed -n \"/\$1/p\"" >&2
		echo "===$F_REGEXMATCHSH - $FF===		-If arg 1 (regex) contains \"/\", this program will automatically add '\\' before every '/' if a '\\' doesn't already precede it" >&2
		echo "===$F_REGEXMATCHSH - $FF===		-If the string (\$2) contains \"/\" and \"|\", this program echo 0 and print an error message" >&2
		echo 0
		return 1
	fi

	if [[ $2 =~ \| ]] && [[ $2 =~ / ]]; then
		echo "===$F_REGEXMATCHSH - $FF===	Arg 2 contains both '/' and '|'. Program doesn't function when both are present. Echoing 0 and returning 1 (this line was printed to stderr)" >&2
		echo 0
		return 1
	elif [[ $2 =~ / ]]; then
		#using perl instead of sed cuz I need to to look behind assertion and sed doesn't do that very well
		#regex: (?<!\\) negative look behind for '\', \/ is what we're matching for, and replacing it with '\/'
		#I'm replacing '/' with '\/' if '/' doesn't have '\' before it
		arg1=$(echo "$1" | perl -pe 's/(?<!\\)\//\\\//g')

		if [[ $(echo $2 | sed -n "/$arg1/p") =~ ^$ ]]; then # 
			echo 0
		else
			echo 1
		fi
	else
		if [[ $(echo $2 | sed -n "/$1/p") =~ ^$ ]]; then
			echo 0
		else
			echo 1
		fi
	fi
}

# This function echo's 1 if arg 1 is a positive or negative integer and 0 if not
#Args: 1 - string
isIntSh(){
	if [[ $(echo "$1" | sed -n '/^-\?[0-9]\+$/p') =~ ^$ ]]; then
		echo 0
	else
		echo 1
	fi
}

# This function echo's 1 if arg 1 is a positive integer and 0 if not
#Args: 1 - string
isPosIntSh(){
	if [[ $(echo "$1" | sed -n '/^[0-9]\+$/p') =~ ^$ ]]; then
		echo 0
	else
		echo 1
	fi
}

