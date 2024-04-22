# Minimalistic-Shell

The Simple Minimalistic Shell is a program designed for launching other programs within a Linux environment. It provides users with a textual interface to enter commands, which are then executed by the shell. The project implements both interactive and batch modes of execution.

## Basic Functionalities

### Interactive Mode
  -Users can enter commands interactively.
  -Each command line starts with a prompt displaying the current working directory and ends with the % character.
  -Commands are executed in child processes, and the output is displayed within the shell.
  -The shell waits for new commands after executing each one.
  -Typing "quit" exits the shell.
  
### Batch Mode

  -Users can write a text file containing multiple commands, each on a distinct line.
  -The shell executes commands sequentially from the file.
  -Commands are executed one by one until the end of the file or the "quit" command is encountered.
  
## Command Types

  -Simple Command: Executes a single program with its specified arguments.
  -Compound Command: Contains special operations such as ;, &, &&, and || for sequential or parallel execution of commands or | pipe for output redirection.

  ### Examples of Compound Commands:
  
    * cmd1 ; cmd2 ; cmd3
    * cmd1 && cmd2
    * cmd1 | cmd2
    * cmd1 && cmd2 ; cmd3 || cmd4 ; cmd5
    
### Implementation Details
  - The project is developed under a Linux environment.
  - It does not use the system() function and relies on system calls such as open(), fork(), exec(), and wait().
