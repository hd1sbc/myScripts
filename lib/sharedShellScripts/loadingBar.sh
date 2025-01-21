#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
stdErrFile=""
stdOutFile=""

# Error Message Constant Vars
F_LOADINGBAR="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

# Getting Values (not putting in function so exec will work)
if [ ! "$(echo "$F_LOADINGBAR" | sed -n '/\.sl$/p')" == "" ]; then
	stdErrFile="$(dirname "$(readlink "$F_LOADINGBAR")")/stdErrOutputLoadingBar.txt"
else
	stdErrFile="$(dirname "$F_LOADINGBAR")/stdErrOutputLoadingBar.txt"
fi
if [ ! "$(echo "$F_LOADINGBAR" | sed -n '/\.sl$/p')" == "" ]; then
	stdOutFile="$(dirname "$(readlink "$F_LOADINGBAR")")/stdOutOutputLoadingBar.txt"
else
	stdOutFile="$(dirname "$F_LOADINGBAR")/stdOutOutputLoadingBar.txt"
fi

exec 4> $stdErrFile
exec 3> $stdOutFile

# This function cleans up the loading bar in case of a SIGINT signal
cleanupLB(){
	if [ -f $stdErrFile ]; then rm $stdErrFile; fi
	if [ -f $stdOutFile ]; then rm $stdOutFile; fi
	exec 3>&-
	exec 4>&-

	exit 0
}
trap cleanupLB SIGINT #trap sigint signal for cleanup

# This function prints loading bar when progress (arg 1) = 0. Returns the current progress value
#Args: 1 - progress, 2 - interval, 3 - duration
loadingBarProcStillRunning(){
	local progress=$1
	local interval=$2
	local duration=$3

	if [ $progress -eq 0 ]; then
		printf "\r%s" "Loading .......................................[0%]"
	# When programming is running longer than ther preset duration
	elif [ $progress -ge 90 ]; then
		printf "\r%s" "Loading .......................................[90%]"
	# Just add to progress string
	else
		progressStr="$1" #converting to string for use in echo statement
		printf "\r%s" "Loading .......................................[$progress%]"
	fi
	
	if [ $progress -lt 90 ]; then
		progress=$(awk "BEGIN {print int($progress+$interval/$duration*100)}") #incrementing progress. Requires decimal math, so using awk cuz can't do it in pure bash
	fi

	return $progress
}

# This function prints loading bar that smooths the values when the function finished before the expected duration
#Args: 1 - progress
loadingBarProcFinishedEarly(){
	local progress=$1
	if [ $progress -le 50 ]; then
		printf "\r%s" "Loading .......................................[56%]"
		progress=55
		sleep 0.4
	fi
	if [ $progress -le 75 ]; then
		printf "\r%s" "Loading .......................................[83%]"
		progress=85
		sleep 0.3
	fi
	if [ $progress -le 90 ]; then
		printf "\r%s" "Loading .......................................[92%]"
		progress=100
		sleep 0.25
	fi

	# Final print statement
	printf "\r%s\n" "Loading .......................................[100%]"
	
}


# This function prints the loading bar (using the above functions) and terminates early if something is printed to stdout or stderr
#Args: 1 - pid, 2 - expected duration, [ 3 - "-n"], [ 3/4* - Pre-loading bar message]
#	3/4* means it could be the 3rd or 4th arg
loadingBar() {
	# Vars
	local errMsgBool=0
	local progress=0
	local interval=0.3
	duration=$2
	local timeoutDuration=20 #value in seconds
	local startTime=$(date +%s)
	local FF="${FUNCNAME[0]}"
	clearLineBool=1

	#Data Validation
	if [ -z $1 ]; then myPerrorSh $F_LOADINGBAR $FF "Invalid args; none were provided. At least 1 required"; fi

	# Parsing optional args
	#if arg 3 isn't empty and isn't the "-n" flag, then echo that arg since it represents the pre-loading bar msg
	if [ ! -z "$3" ] && [ ! "$3" == "-n" ]; then
		echo "$3"
	#if arg 3 isn't empty and the "-n" flag (latter implied by previous conditional and else if)
	elif [ ! -z "$3" ]; then
		clearLineBool=0
		#if arg 4 isn't empty, then echo that arg
		if [ ! -z "$4" ]; then echo "$4"; fi
	fi

	# While process is still running
	while [ -f /proc/$1/exe ] ; do
		#get elapsed time for time out 
		local elapsedTime=$(( $(date +%s) - startTime ))
	
		#the timeout is used to prevent an infinite loop for the loading bar
		if [ $elapsedTime -eq $timeoutDuration ]; then
			printf "\r\033[K"
			echo "===Loading Bar===	Exiting loading bar; process took too long" >&2
			errMsgBool=1
			break
		#if "stop" was printed to stderr, then end program
		#this is designed for when this func is called within the function is is supposed to be printing the loading bar
		#this scenario is for when a function doesn't lend itself well to running in th background. I call loadingBar() within in the background within the function and write to stdErr at the end of the function
		elif [[ $(cat $stdErrFile | tr -d '\0') =~ ^stop$ ]]; then
			break
		elif [[ ! $(cat $stdOutFile | tr -d '\0') =~ ^$ ]]; then #if something was printed to stdOut
			errMsgBool=$(expr $errMsgBool + 1)
			if [ $errMsgBool -eq 1 ]; then printf "\r\033[K"; fi
			cat $stdOutFile
			truncate -s 0 $stdOutFile
		elif [[ ! $(cat $stdErrFile | tr -d '\0') =~ ^$ ]]; then #if something was printed to stderr
			errMsgBool=$(expr $errMsgBool + 1)
			if [ $errMsgBool -eq 1 ]; then printf "\r\033[K"; fi
			cat $stdErrFile
			#if the loading bar is running in the background and "stop" is printed to standard error (denoting to end loading bar), then exit loop. For some reason
			if [ ! "$(cat $stdErrFile | tr -d '\0' | grep "^stop$")" == "" ]; then return; fi
			truncate -s 0 $stdErrFile
		elif [ $errMsgBool -eq 0 ]; then #if nothing was printed, keep printing loading bar
			sleep $interval
			loadingBarProcStillRunning $progress $interval $duration
			progress=$?
		fi

	done

	#if loading bar wasn't interrupted by stdout or stderr, then print last bit of loading bar
	if [ $errMsgBool -eq 0 ]; then loadingBarProcFinishedEarly $progress; fi

	# If clear line boolean is true
	if [ $clearLineBool -eq 1 ]; then
		# clears current line
		printf "\r\033[K"
		#if arg 3 was provided, then move up a line, clear it, move up another line, clear it
		# 	need to move up twice to cuz of '\n' in final print statement (the 100% one)
		#only need to check if arg 3 is empty cuz if 4 args provided, then the "-n" flag was used 
		if [ ! -z "$3" ]; then printf "\r\033[F\r\033[K\r\033[F\r\033[K"; fi
	fi

	# Revome the files and file descriptors created for stdout and stderr
	rm $stdErrFile
	rm $stdOutFile
	exec 3>&-
	exec 4>&-
}
