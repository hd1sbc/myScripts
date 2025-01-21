#!/bin/bash

# This function moves the alias file to standard location and if the shell isn't bash, appends code to make the aliases usable on the system
aliasSetup () {
	#FILLIN need to fill in the id of the file in the line below
	bashAliasesLoc=$(~/myScripts/lib/base/findFilePathFromId 1) #gets location of bash_aliases. Using file path instead of executable cuz it hasn't been setup yet
	
	if [ ! -f ~/.bash_aliases ]; then #if .bash_aliases doesn't exist on system by default
		mv $bashAliasesLoc ~/.bash_aliases #move the current file to the appropriate dir and make it hidden
	else #if .bash_aliases does exist on system by default
		echo $bashAliasesLoc >> ~/.bash_aliases #append contents of bash_aliases to default/existing file
		rm $bashAliasesLoc #since .bash_aliases already exists, remove the one from the repo
	fi

	#if using zsh shell or aliases are found in zshrc, then use append bash_aliases related text to ~/.zshrc
	if [[ ! $(echo $SHELL) =~ bash ]] && [[ ! $(which bash) =~ bin ]]; then #if current shell isn't bash and bash doesn't exist on system
		newShell=$(echo $SHELL | sed 's/.*\///g') #getting the executable file name from the $SHELL path
		
		#finding shellrc file by looking for hidden files in home dir that include the shell name and are followed by "rc"
		#The rc is intentionally not in quotes
		#using the -o option to only match pattern, not whole line
		shellRcFileName=$(ls -la ~ | grep -o "\..*$newShell"rc)
		
		# Appending the following three lines to the .shellrc file (like .zshrc) so it will use the .bash_aliases file for reference of aliases
		echo "if [ -f ~/.bash_aliases ]; then" >> ~/$shellRcFileName
		echo "  . ~/.bash_aliases" >> ~/$shellRcFileName
		echo "fi" >> ~/$shellRcFileName
	fi
}


#TODO
#add that you need to export LIBRARY_PATH=/new/path/to/lib:$LIBRARY_PATH for gcc


# This function adds the compiled folder and subfolders
compiledFolderSetup () {
	# Ideally, none of these folders shouldn't already exist if the UNINSTALL.sh works correctly. However, it's easy to check, so might as well add it
	if [ ! -d ~/myScripts/compiledCode ]; then mkdir ~/myScripts/compiledCode; fi #if dir doesn't already exist, then make dir
	if [ ! -d ~/myScripts/compiledCode/compiledC ]; then mkdir ~/myScripts/compiledCode/compiledC; fi #if compiledC doesn't already exist, then make dir
	if [ ! -d ~/myScripts/compiledCode/compiledCPP ]; then mkdir ~/myScripts/compiledCode/compiledCPP; fi #if compiledC++ doesn't already exist, then make dir
	if [ ! -d ~/myScripts/compiledCode/compiledJava ]; then mkdir ~/myScripts/compiledCode/compiledJava; fi #if compiledJava doesn't already exist, then make dir
}

# This function sets up my terminator configuration preferences
terminatorConfigSetup () {
	#if terminator config file doesn't exist, it means the terminator package wasn't already installed. In that case, don't do anything
	if [ -f ~/.config/terminator/config ]; then #if terminator config file already exists
		rm ~/.config/terminator/config #remove existing config file
		#FILLIN need to fill in the id of the file in the line below
		terminatorConfigLoc=$(~/myScripts/lib/base/findFilePathFromId someNum) #gets location of terminatorConfig. Using file path instead of executable cuz it hasn't been setup yet
		
		#moving config file and changing name to just config so the terminator program can access it
		#"terminatorConfig" file name is used in the repo to differentiate various config files
		mv $terminatorConfigLoc ~/.config/terminator/config
	fi
}

# This function sets up my vim preferences
vimConfigSetup () { #my vim settings are basically just key mappings
	#editing /etc/vim/vimrc to use system vimrc default settings, but have user vimrc override those
	sudo sed -i 's/^" source $VIMRUNTIME\/defaults\.vim.*$/source $VIMRUNTIME/defaults.vim/g' /etc/vim/vimrc 

	if [ -f ~/.vimrc ]; then rm ~/.vimrc; fi #moving vimrc to the correct location
	mv ~/myScripts/linuxBoot/vimrc ~/.vimrc #moving file and making it hidden
}

# Main function is here just to make it easy to read and see how all the functions above come together
main () {
	aliasSetup
	compiledFolderSetup
	terminatorConfigSetup
	vimConfigSetup
}

main
