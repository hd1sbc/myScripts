# LOADING BAR SHELL MANUAL #
- - -

### NAMES ###
- - -

loadingBar

### SYNOPSIS ###
- - -

loadingBar \<pid\> \<expected duration\> \[-n\] \[pre-loading bar message\]

### DESCRIPTION ###
- - -

This function prints a loading bar with increasing values of percentage complete (based on pre-set expected duration) when running bash scripts.

It can be run in the background and stopped by echo'ing stop to standard error.

The loading bar will automatically close itself (since it is run in the background) after 30 seconds.

If the loading bar percentage complete reaches 90% before the program is finished, it will hold the value of 90% until the program is finished.

Whenever the program finished, regardless of the percentage complete, a graceful percentage completed routine will run to smooth out the percent complete values. For example, if "stop" is echo'ed to standard error while the percentage complete is only 50%, the program will change the percentage complete to 75%, wait 0.3 seconds, then switch to 90% complete, wait 0.25 seconds, then print 100%.

The loading bar line will be cleared automatically. If the "-n" option is used, the preloading bar message line will also be cleared.

### OPTIONS ###
- - -

-n      :- clear pre-loading bar line


### EXAMPLES ###
- - -

loadingBar 12345 10         :- will print a loading bar with incrementing percentages of ~3% every 1/3 of a second

loadingBar 12345 10 -n      :- will print a loading bar with incrementing percentages of ~3% every 1/3 of a second, but will not remove the printed loading bar text after it has been completed

loadingBar 12345 10 \"hi\"  :- wil print \"hi\" followed by a new line with a loading bar with incrementing percentages of ~3% every 1/3 of a second

loadingBar 12345 10 \"hi\" -n :- wil print \"hi\" followed by a new line with a loading bar with incrementing percentages of ~3% every 1/3 of a second, but will not remove either the loading bar line or the pre-loading bar message after the function has been closed

### NOTES ###
- - -

- Remember to echo \"stop\" when you want the loading bar to close

### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- lib/sharedShellScripts/loadingBar.sh
