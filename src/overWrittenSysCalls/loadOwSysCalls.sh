#!/bin/bash

# GLOBAL VARS--------------------------------------------------------------------------------------
owSysCallsMkFileLoc="$(findPath -f 289)" #overWrittenSysCalls.mk
osSysCallsSharedObjLoc="$(findPath -f 216)" #liboverWrittenSysCalls.so
#--------------------------------------------------------------------------------------------------

unset LD_PRELOAD
make -f $owSysCallsMkFileLoc
if [ $? -eq 0 ]; then
	export LD_PRELOAD=$osSysCallsSharedObjLoc
else
	echo "Error make $owSysCallsMkFileLoc. Previous value at LD_PRELOAD was unset and nothing new was set. Exiting -1"
	exit -1
fi


