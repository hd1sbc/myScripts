#!/bin/bash
# Defualt shebang is bash, but will be updated accordingly by the INSTALL.sh file

fileId=1
packageManagerCommand=""

userIsNotRoot () {
	# Variables
	currDir="$(pwd)" #for use in guiding the user on command to run
	if [[ $(which bash) =~ bin ]]; then  #if bash is default shell or installed on system, then use it
		shell="bash"
	else
		shell="$(echo $SHELL | sed 's/.*bin\///')" #getting just the executable name from the file path returned in "echo $SHELL"
	fi
	fileName=$(echo $0 | sed 's/.*\///g') #gets the current file path and removes everything except for the file name. Keep ECHO $0. I'm looking for file name, not shell

	echo "Current user lacks adequate privileges. Program is exiting"
	echo "Please run the following commands to install the additional programs: "
	echo "	sudo su -"
	echo "	$shell $currDir/$fileName"
	
	exit 1 #failed to run script. Exit value not specific
}

# This function determines which pacakage manager to use based on OS
packageManager () {
	distro=$(uname -a)
	distroLike=$(cat /etc/*-release | grep "^ID_LIKE=") #if an unknown system, this can help determine which major branch it's from. Uses the ID_LIKE field
	
	if [ $distro = "" ]; then
		echo "Distribution could not be found"
		echo "Exiting installation"
		exit 1
	elif [[ $distro =~ (Ubuntu|ubuntu) ]]; then
		packageManagerCommand="apt-get install"
	elif [[ $distro =~ (Kali|kali) ]]; then
		packageManagerCommand="apt-get install"
	elif [[ $distro =~ (fedora|Fedora|rhel|RHEL|redhat|Redhat|centos|CentOS) ]] || [[ $distroLike =~ (redhat|Redhat|rhel|RHEL) ]]; then
		if [ $(which dnf) = "" ]; then #if the updated package manager, DNF, is installed, use it instead of yum
			packageManagerCommand="dnf install"
		else
			packageManagerCommand="yum install"
		fi
	elif [[ $distro =~ void ]]; then
		packageManagerCommand="xbps-install -S"
	elif [[ $distro =~ (arch|Arch) ]] || [[ $distroLike =~ (arch|Arch) ]]; then
		packageManagerCommand="pacman -S"
	elif [[ $distroLike =~ (debian|Debian) ]]; then
		packageManagerCommand="apt-get install"
	elif [[ $distro =~ (freeBSD|FreeBSD) ]]; then
		packageManagerCommand="pkg install"
	elif [[ $distro =~ (openBSD|OpenBSD) ]]; then
		packageManagerCommand="pkg_add"
	else
		echo "Distribution not supported"
		echo "Exiting installation"
		exit 1
	fi
}


# This program converts the list of programs to install from listOfProgramsToInstall.txt to commands
programsToInstallToCommands () {
	packageManager

	i=0
	for row in $(cat ~/myScripts/dep/listOfProgramsToInstall.txt); do #based on the install script, we cloned the repo into the pwd of the user. So we should still be in that dir
		if [ ! $i -eq 0 ]; then #if not the first row because that row is the column names/header row
			package=$(echo $row | sed 's/,.*//g')
			system=$(echo $row | sed 's/.*,//g')
			#this isn't an exhaustive way to check if the package is on the system. It's just a quick check. I could grep the package manager, but it isn't worth it. Doesn't add much value
			if [[ ! $(which $package) =~ bin ]] ; then #if program doesn't already exist.
				if [[ $system =~ ^$ ]]; then #if system var is empty, then install it (empty means every system)
					eval sudo $packageManagerCommand $package
				elif [[ $(uname -a) =~ $system ]]; then #if on the right system, then install
					eval sudo $packageManagerCommand $package
				fi
			else
				echo "Package $package already exists, moving to next package"

			fi
		fi
		i=$(expr $i + 1) #incrementing i in posix-compatible manner
	done
	
}


installFromMyOtherRepos () {
	echo "FILLIN"
	#In this function, you just want to download and mark which repos have been downloaded
	#After completing the installation of the "base" repo (Linux Utils), then use the install script from the downloaded repos
	#you probably want to make this section interactive

}


main () {
	echo "Do you want to install additional packages? (Y/n)"
	#FILLIN need to add "more" option
	echo "If you're unsure, type "more" to see a full list of additional packages that will be installed"
	echo "Your answer: "
	read yn

	if [[ $yn =~ ^(y|Y)$ ]] || [ $yn = "" ]; then #if yes or no option
		#checking to make sure user is root. If not, tell them the commands to run and exit
		#FILLIN see if logged in as root is necessary. You don't want it to be. Long story why. Also make sure to fix "else" statement, which is just a filler "if" statement rn
		#if [ ! $(whoami) = "root" ]; then
		#	userIsNotRoot	
		#FILLIN fix if statement below. see the lines above for details
		if [ 1 -eq 1 ]; then #else #if user is root
			programsToInstallToCommands
			installFromMyOtherRepos
			
			#updating the shebang on updateShebang.sh cuz different shell. Finding new shell path, adding backslashes to make it compatible with sed, the replacing the first line with new shebang
			newShell="#\!$(echo $SHELL | sed 's/\//\\\//g')"; sed -i "1s/.*/$newShell/" updateShebang.sh
			~/myScripts/config/setup/updateShebang.sh setup #updates the shebang with the "setup" option
			$SHELL ~myScripts/config/setup/dirAndFileSetup.sh
		fi
	elif [[ $yn =~ ^(n|N)$ ]]; then
		~/myScripts/config/setup/updateShebang.sh setup #updates the shebang with the "setup" option
		$SHELL ~/myScripts/config/setup/dirAndFileSetup.sh
	else
		echo "Incorrect option chosen. Please select either 'y' or 'n'"
	fi
}

main
