#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
stackMax=15
currDir=""
cdBackStack=()
cdForwardStack=()
backStackCount=0
forwardStackCount=0

# Error Message Constant Vars
F_CDSTACK="${BASH_SOURCE[0]}"
#--------------------------------------------------------------------------------------------------

source $sharedScripts

# This function overwrites the "cd" command to add forward and backwards abilities
cd(){
	builtin cd "$@" #run the original cd command
	if [ $? -eq 0 ]; then #if no errors

		# Checking num elements in array to prevent overflow
		if [ $backStackCount -gt $stackMax ]; then
			unset cdBackStack[$stackMax]
		else
			backStackCount=$(expr $backStackCount + 1)
		fi

		# Updating stacks
		if [ ! "$currDir" == "" ]; then
			cdBackStack=("$currDir" "${cdBackStack[@]}")
		fi
		# Reseting forward stack
		unset cdForwardStack
		forwardStackCount=0
		currDir="$(pwd)"
	fi
}

# This function displays the current back stack, forward stack, and curr dir
s(){
	echo -n "Back Stack: "
	if [ ${#cdBackStack[@]} -eq 0 ]; then
		echo "empty"
	else
		echo "${cdBackStack[@]}"
	fi

	echo -n "Forward Stack: "
	if [ ${#cdForwardStack[@]} -eq 0 ]; then
		echo "empty"
	else
		echo "${cdForwardStack[@]}"
	fi

	echo -n "Current Directory: "
	if [ "$currDir" == "" ]; then
		echo "empty (no cd change yet)"
	else
		echo "$currDir"
	fi
}

# This function takes the user to previous dir
b(){
	# Vars
	local FF="${FUNCNAME[0]}"

	if [ ! -z "${cdBackStack[0]}" ]; then #if back stack isn't empty
		backDir="${cdBackStack[0]}" #dir to move to (1st element in back stack)
		builtin cd "$backDir"
		if [ $? -eq 0 ]; then
			# Checking num elements in forward stack to prevent buffer overflow
			if [ $forwardStackCount -gt $stackMax ]; then
				unset cdForwardStack[$stackMax]
			else
				forwardStackCount=$(expr $forwardStackCount + 1)
			fi

			# Updating stacks
			cdBackStack=("${cdBackStack[@]:1}") #popping 1st element from back stack
			cdForwardStack=("$currDir" "${cdForwardStack[@]}") #pushing popped element from back stack to forward stack
			currDir="$backDir"
		fi
	else #if back stack is empty
		myPerrorSh $F_CDSTACK $FF "cd backwards stack is empty; no directory to move to. Returning -1"
		return -1
	fi
}

# This function takes user to forward dir
f(){
	# Vars
	local FF="${FUNCNAME[0]}"

	if [ ! -z "${cdForwardStack[0]}" ]; then #if forward stack isn't empty
		forwardDir="${cdForwardStack[0]}" #dir to move to (1st element in forward stack)
		builtin cd "$forwardDir"
		if [ $? -eq 0 ]; then
			# Checking num elements in back stack to prevent buffer overflow
			if [ $backStackCount -gt $stackMax ]; then
				unset cdBackStack[$stackMax]
			else
				backStackCount=$(expr $backStackCount + 1)
			fi
		
			cdBackStack=("$currDir" "${cdBackStack[@]}") #pushing popped element from forward stack to back stack
			cdForwardStack=("${cdForwardStack[@]:1}") #popping 1st element from back stack
			currDir="$forwardDir"
		fi
	else #if forward stack is empty
		myPerrorSh $F_CDSTACK $FF "cd forwards stack is empty; no directory to move to. Returning -1"
		return -1
	fi
}
