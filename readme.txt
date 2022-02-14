Shell Ex4
Authored by Amit Cohen, ID : 315147330

==Description==

The program implements "mini-shell" for linux use in C language.
Following Exercises 1,2 and 3, building a shell and implementing the ctrl + Z command and the fg command.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
program DATABASE:
1. stringPar = string that entered by user.
2. argv = 2D array that contains the user's  command and its arguments.
3. fiestA = 2D array that contains the user's command and its arguments, when there is at least one pipe.
4. secondA = 2D array containing the user's command and arguments, when there is at least one pipe.
5. thirdA = 2D array containing the user's command and its arguments, when there is at least one pipe.
6.amountCommands = number of commands the user called in this session.
7. amountPipes = number of pipes the user typed in this session. (statistics-use-only).
8. pipeCount = used to determine how many commands to run in this loop (how many pipes were typed in).
9. wordCount = number of words in the last command typed in.
10. pidSon = saves the pid of the son we interrupted.
11. stat = Son process's status, to determine if stopped by Ctrl+Z.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
functions:
1.countWords-This function counts how many words there are in a string
2.buildCommandLine-This function builds the command line and prints it.
3.freeTheArray-This function releases the two-dimensional array.
4.build_argv-This function constructs the two-dimensional array and names it in the last member NULL.
5.findThePipe-This function goes over the string and counts how many pipes there are.
6.noPipes-This function basically executes the command to execvp when there are no pipes at all.
7.one_pipe-This function basically executes the command to execvp when there is one tube, puts the output of the first command into the input of the second command
8.two_pipes-This function basically executes the command to execvp when there are two tubes, inputs the output of the first command to the input of the second command and the output of the second command to the input of the third command.
9.findApostrophes-This function looks for apostrophe in a string.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
==Program Files==
ex4.c - contain the main and all the functions.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
==How to compile?==
    compile: gcc ex4.c -o ex4
    run: ./ex4
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
==Input==
Commands (up to 512 Characters).
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~;
==Output==
Executing the given command.