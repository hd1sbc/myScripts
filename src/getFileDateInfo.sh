#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
arg1BaseName=""
arg2BaseName=""
echoStr="Last Modified: "

# Error Message Constant Vars
F_GETFILEDATE="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# SOURCING-----------------------------------------------------------------------------------------
source "$sharedScripts"
checkFileLocs $F_GETFILEDATE
#--------------------------------------------------------------------------------------------------

# This function compares the difference between two dates
# 	-if the two are the same or less than a day, it will return 1 or 2 respectively, and not print anything
#Args: 1 - "ls -la" command output for file 1; 2* - "ls -la" command output for file 2
simplifyYMD(){
	# Vars
	file2Year=""
	file2Month=""
	file2Day=''

	if [ $# -eq 1 ]; then
		todaysDay="$(date +%F)"
		file2Year="$(echo $todaysDay | cut -d '-' -f 1)"
		file2Month="$(echo $todaysDay | cut -d '-' -f 2)"
		file2Day="$(echo $todaysDay | cut -d '-' -f 3)"
	else
		file2Year="$(echo $2 | cut -d '-' -f 1)"
		file2Month="$(echo $2 | cut -d '-' -f 2)"
		file2Day="$(echo $2 | cut -d '-' -f 3)"
	fi

	fileYear="$(echo $1 | cut -d '-' -f 1)"
	fileMonth="$(echo $1 | cut -d '-' -f 2)"
	fileDay="$(echo $1 | cut -d '-' -f 3)"

	yearDiff=$(( file2Year - fileYear ))
	monthDiff=$(( file2Month - fileMonth ))
	dayDiff=$(( file2Day - fileDay ))

	if [ $(( yearDiff + monthDiff + dayDiff )) -eq 0 ]; then
		return 1
	elif [ $(( yearDiff + monthDiff )) -eq 0 ] && [ $dayDiff -le 1 ] && [ $dayDiff -ge -1 ]; then
		return 2
	elif [ $(( yearDiff + monthDiff )) -eq 0 ]; then
		if [ $dayDiff -lt 0 ]; then
			newDayDiff="$(echo $dayDiff | sed 's/-//g')"
			echoStr+="$arg1BaseName is newer by $newDayDiff days compared to $arg2BaseName "
		elif [ $# -eq 2 ]; then echoStr+="$arg2BaseName is newer by $dayDiff days compared to $arg1BaseName ";
		else echoStr+="$dayDiff days ago ";
		fi
	elif [ $yearDiff -eq 0 ]; then
		if [ $monthDiff -lt 0 ]; then
			newMonDiff="$(echo $monthDiff | sed 's/-//g')"
			echoStr+="$arg1BaseName is newer by $newMonDiff months compared to $arg2BaseName "
		elif [ $# -eq 2 ]; then echoStr+="$arg2BaseName is newer by $monthDiff months compared to $arg1BaseName ";
		else echoStr+="$monthDiff months ago ";
		fi
	else
		if [ $yearDiff -lt 0 ]; then
			newYearDiff="$(echo $yearDiff | sed 's/-//g')"
			echoStr+="$arg1BaseName is newer by $newYearDiff years compared to $arg2BaseName "
		elif [ $# -eq 2 ]; then echoStr+="$arg2BaseName is newer by $yearDiff years compared to $arg1BaseName ";
		else echoStr+="$yearDiff years ago ";
		fi
	fi

	return 0

}

# This function compares the difference between two dates
#Args: 1 - "ls -la" command output for file 1; 2* - "ls -la" command output for file 2
simplifyHMS(){
	# Vars
	file2Hour=""
	file2Min=""
	file2Sec=""

	if [ $# -eq 1 ]; then
		file2Hour="$(date +%H)"
		file2Min="$(date +%M)"
		file2Sec="$(echo $(date +%S) | sed 's/^0*//g')" #using sed to get rid of leading 0's
	else
		file2Hour="$(echo $2 | cut -d ':' -f 1)"
		file2Min="$(echo $2 | cut -d ':' -f 2)"
		file2Sec="$(echo $2 | cut -d ':' -f 3 | cut -d '.' -f 1)"
	fi

	fileHour="$(echo $1 | cut -d ':' -f 1)"
	fileMin="$(echo $1 | cut -d ':' -f 2)"
	fileSec="$(echo $1 | cut -d ':' -f 3 | cut -d '.' -f 1)"

	hourDiff=$(( file2Hour - fileHour ))
	minDiff=$(( file2Min - fileMin ))
	secDiff=$(( file2Sec - fileSec ))
	
	
	if [ $(( hourDiff + minDiff + secDiff )) -eq 0 ]; then
		if [ $# -eq 2 ]; then echoStr+="$arg1BaseName and $arg2BaseName dates are the same ";
		else echoStr+="right now ";
		fi
	elif [ $(( hourDiff + minDiff )) -eq 0 ]; then
		if [ $secDiff -lt 0 ]; then
			newSecDiff="$(echo $secDiff | sed 's/-//g')"
			echoStr+="$arg1BaseName is newer by $newSecDiff seconds compared to $arg2BaseName "
		elif [ $# -eq 2 ]; then echoStr+="$arg2BaseName is newer by $secDiff seconds compared to $arg1BaseName ";
		else echoStr+="$secDiff seconds ago ";
		fi
	elif [ $hourDiff -eq 0 ]; then
		if [ $minDiff -lt 0 ]; then
			newMinDiff="$(echo $minDiff | sed 's/-//g')"
			echoStr+="$arg1BaseName is newer by $newMinDiff minutes compared to $arg2BaseName "
		elif [ $# -eq 2 ]; then echoStr+="$arg2BaseName is newer by $minDiff minutes compared to $arg1BaseName ";
		else echoStr+="$minDiff minutes ago ";
		fi
	else
		if [ $hourDiff -lt 0 ]; then
			newHourDiff="$(echo $hourDiff | sed 's/-//g')"
			echoStr+="$arg1BaseName is newer by $newHourDiff hours compared to $arg2BaseName "
		elif [ $# -eq 2 ]; then echoStr+="$arg2BaseName is newer by $hourDiff hours compared to $arg1BaseName ";
		else echoStr+="$hourDiff hours ago ";
		fi
	fi
}

# This function combines the functions above to determine which one should be run
#Args: 1 - file date info for file 1; 2* - file date info for file 2
simplifyDate(){
	colNum=0
	sameDayBool=0

	if [ $# -eq 1 ]; then
		for col in $1; do
			if [ $colNum -eq 1 ]; then simplifyYMD $col; sameDayBool=$?; fi
			if [ $colNum -eq 2 ] && [ ! $sameDayBool -eq 0 ]; then simplifyHMS $col; fi

			let colNum=colNum+1
		done
	else
		col1Arg1=""
		col2Arg1=""
		col1Arg2=""
		col2Arg2=""

		for col in $1; do
			if [ $colNum -eq 1 ]; then col1Arg1=$col; fi
			if [ $colNum -eq 2 ]; then col1Arg2=$col; fi
			let colNum=colNum+1
		done
		colNum=0
		for col in $2; do
			if [ $colNum -eq 1 ]; then col2Arg1=$col; fi
			if [ $colNum -eq 2 ]; then col2Arg2=$col; fi
			let colNum=colNum+1
		done	
		
		simplifyYMD $col1Arg1 $col2Arg1
		sameDayBool=$?
		if [ ! $sameDayBool -eq 0 ]; then
			simplifyHMS $col1Arg2 $col2Arg2
		fi
	fi
}

# This function simplifies the file date info for 1 file
#Args: 1- file path
getFileDate1Arg(){
	statInfo="$(stat $1)"
	
	lastModifiedSimple="$(echo "$statInfo" | grep "Modify:*")"
	simplifyDate "$lastModifiedSimple"
	echo $echoStr
	
	echoStr="Last Accessed: "
	lastAccessedSimple="$(echo "$statInfo" | grep "Access:*" | sed -n '2p')"
	simplifyDate "$lastAccessedSimple"
	echo $echoStr
}

# This function compares the file date info between 2 files
#Args: 1 - file path1; 2 - file path2
getFileDate2Args(){
	statInfoFile1="$(stat $1)"
	statInfoFile2="$(stat $2)"
		
	lastModifiedSimpleFile1="$(echo "$statInfoFile1" | grep "Modify:*")"
	lastModifiedSimpleFile2="$(echo "$statInfoFile2" | grep "Modify:*")"
	simplifyDate "$lastModifiedSimpleFile1" "$lastModifiedSimpleFile2"
	echo $echoStr
	
	echoStr="Last Accessed: "
	lastAccessedSimpleFile1="$(echo "$statInfoFile1" | grep "Access:*" | sed -n '2p')"
	lastAccessedSimpleFile2="$(echo "$statInfoFile2" | grep "Access:*" | sed -n '2p')"
	simplifyDate "$lastAccessedSimpleFile1" "$lastAccessedSimpleFile2"
	echo $echoStr


}

# Help Menu
helpMenuGetFileDate(){
	echo "+++GET FILE DATE - HELP MENU+++	Relative File Path: $0"
	echo "Purpose: provide info about the last modified and last accessed of a file or compare those two metrics between two files"
	echo "Usage: "
	echo "	<binNameOrAlias> <file1> [file2]"
	echo "Examples: "
	echo "	<binNameOrAlias> file1		:- prints how long ago the file was last accessed and modified"
	echo "	<binNameOrAlias> file1 file2	:- prints how long ago the file1 was last accessed and modified compared to file2"
}

# Effective Main Function
getFileDate(){
	# Vars
	FF="${FUNCNAME[0]}"

	# Data Validation
	if [ $(isHelpSh $1) -eq 1 ]; then helpMenuGetFileDate; return 1;
	elif [ ! $# -eq 1 ] && [ ! $# -eq 2 ]; then
		myPerrorSh $F_GETFILEDATE $FF "Invalid number of args. 1 or 2 args were expected, $# were provided. Aborting"
		myPerrorSh $F_GETFILEDATE $FF "	For help, use -h or --help option"
		return 1
	elif [ ! -f $1 ]; then myPerrorSh $F_GETFILEDATE $FF "File $1 not found. Aborting"; return 1;
	elif [ $# -eq 2 ] && [ ! -f $2 ]; then myPerrorSh $F_GETFILEDATE $FF "File $2 not found. Aborting"; return 1;
	fi
	
	if [ $# -eq 1 ]; then
		arg1BaseName=$(getFileNameSh $1)
		getFileDate1Arg $1
	else
		arg1BaseName=$(getFileNameSh $1)
		arg2BaseName=$(getFileNameSh $2)
		getFileDate2Args $1 $2
	fi
	

}

getFileDate $@
