# ProjectTWO

In this project, you will learn how to develop and implement a Linux/Unix shell. This will give you the opportunity to learn how child processes are created to perform large-grained work and how the parent process can follow up on a child process's work. This project is to be done on a Linux system and will require your programming in C.

1. The shell must support the following internal commands:
- **cd** <directory> - Change the current default directory to <directory>. If the <directory> argument is not present, report the current directory. If the directory does not exist an appropriate error should be reported. This command should also change the PWD environment variable.
- **clr** - Clear the screen.
- **dir** <directory> - List the contents of directory <directory>.
- **environ** - List all the environment strings.
- **echo** <comment> - Display <comment> on the display followed by a new line (multiple spaces/tabs may be reduced to a single space).
- **help** - Display the user manual using the more filter.
- **pause** - Pause operation of the shell until 'Enter' is pressed.
- **quit** - Quit the shell.
- The shell environment should contain shell=<pathname>/myshell where <pathname>/myshell is the full path for the shell executable(not a hardwired path back to your directory, but the one from which it was executed).
  
2. All other command line input is interpreted as program invocation, which should be done by the shell forking and execing the programs as its own child processes. The programs should be executed with an environment that contains the entry: parent=<pathname>/myshell where <pathname>/myshell is as described in the last point of 1 above.

3. The shell must be able to take its command line input from a file. That is, if the shell is invoked with a command line argument: "myshell batchfile"then batchfile is assumed to contain a set of command lines for the shell to process. When the end-of-file is reached, the shell should exit. Obviously, if the shell is invoked without a command line argument, it solicits input from the user via a prompt on the display. 
