# CD COMMANDS MANUAL - S #
- - -

### NAMES ###
- - -

s

### SYNOPSIS ###
- - -

s

### DESCRIPTION ###
- - -

A function that is works with the other components of the cd stack code. This function shows the stack of current, previous, and forward directories based on previous use of the cd command in the current shell session.

If any of these values are empty, the program will print so.

Values for each of the 3 parameters (current, previous, and forward directory) are printed on the same line, separated by a space.

The forward stack will be emptied everytime the cd command is used. If the "f" or "b" command are used, the forward stack will not automatically be emptied.

This program works with both bash and zsh.

### OPTIONS ###
- - -

none

### SEE ALSO ###
- - -

- b.md
- f.md

### ASSOCIATED SOURCE CODE DIRECTORIES AND/OR FILES ###
- - -

- src/cdStack/
