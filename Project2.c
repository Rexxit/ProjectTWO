/*
 * Project2.c
 *
 *  Created on: Feb 19, 2020
 *      Author: Wayne Cook
 */

// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>

#define MAXLTR 1000 // max number of letters to be supported
#define MAXCOMM 1000 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

int main()
{
    char inputString[MAXLTR], *parsedArgs[MAXCOMM];
    char* parsedArgsPiped[MAXCOMM];
    int execChecker = 0;

    while (1)
    {
        // print shell line
        printDir();
        // take input
        if (takeInput(inputString))
            continue;
        // process
        execChecker = processString(inputString, parsedArgs, parsedArgsPiped);
        /* execChecker returns zero if there is no command
         or it is a built in command,
        */

        // if 1 then it is a simple command
        if (execChecker == 1)
        {
            execArgs(parsedArgs);
        }//end if()

        // if 2 then it is including a pipe
        if (execChecker == 2)
        {
            execArgsPiped(parsedArgs, parsedArgsPiped);
        }//end if()
    }//end while()
    return 0;
}//end main()



// Function to take input
int takeInput(char* str)
{
    char* buffR;

    buffR = readline("\n>>> ");
    if (strlen(buffR) != 0)
    {
        add_history(buffR);
        strcpy(str, buffR);
        return 0;
    }//end if()
    else
    {
        return 1;
    }//end else()
}//end takeInput()


// Function to print the Current Directory
void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}//end printDir()

void environStrings (char* envirp)
{
	int i;
	for (i = 0; envirp[i] != NULL; i++)
	{
		printf("\n%s", envirp[i]);
	}//end for()
	getchar();
}//end environStrings()


void echoIt()
{
	int x = 0;
	int chars;

	while (x != EOF)
	{
		x = getChar();
		chars = getchar();
		putchar();
	}//end while()
}//end echoIt()


// Function where the system command is executed
void execArgs(char** parsed)
{
    // Forking a child
    pid_t pid = fork();

    if (pid == -1)
    {
        printf("\nFailed forking child..");
        return;
    }//end if()
    else if (pid == 0)
    {
        if (execvp(parsed[0], parsed) < 0)
        {
            printf("\nCould not execute command..");
        }//end if()
        exit(0);
    }//end else if()
    else
    {
        // waiting for child to terminate
        wait(NULL);
        return;
    }//end else()
}//end execArgs()


// Function where the piped system commands is executed
void execArgsPiped(char** parsed, char** parsedpipe)
{
    // 0 is read end, 1 is write end
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0)
    {
        printf("\nPipe could not be initialized");
        return;
    }//end if()
    p1 = fork();
    if (p1 < 0)
    {
        printf("\nCould not fork");
        return;
    }//end if()

    if (p1 == 0)
    {
        // Child 1 executing..
        // It only needs to write at the write end
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(parsed[0], parsed) < 0)
        {
            printf("\nCould not execute command 1..");
            exit(0);
        }//end if()
    }//end if()
    else
    {
        // Parent executing
        p2 = fork();

        if (p2 < 0)
        {
            printf("\nCould not fork");
            return;
        }//end if()

        // Child 2 executing..
        // It only needs to read at the read end
        if (p2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0)
            {
                printf("\nCould not execute command 2..");
                exit(0);
            }//end if()
        }//end if()
        else
        {
            // parent executing, waiting for two children
            wait(NULL);
            wait(NULL);
        }//end else()
    }//end else
}//end execArgsPiped()


// Help command builtin
void openHelp()
{
    puts("\n***HELP***"
        "\n-This bad shell.."
        "\nList of Commands supported:"
        "\n>cd"
        "\n>ls"
    	"\n>echo"
    	"\n>environ"
        "\n>exit"
        "\n>all other general commands available in UNIX shell"
        "\n>pipe handling"
        "\n>improper space handling");

    return;
}//end openHelp()


// Function to execute builtin commands
int ownCmdHandler(char** parsed)
{
    int i, NoOfOwnCmds = 5, switchOwnArg = 0;
    char* ListOfOwnCmds[NoOfOwnCmds];
    char* username;

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "echo";
    ListOfOwnCmds[3] = "environ";
    ListOfOwnCmds[4] = "help";
    ListOfOwnCmds[5] = "hello";

    for (i = 0; i < NoOfOwnCmds; i++)
    {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0)
        {
            switchOwnArg = i + 1;
            break;
        }//end if()
    }//end for()

    switch (switchOwnArg)
    {
    case 1:
        printf("\nGoodbye\n");
        exit(0);
    case 2:
        chdir(parsed[1]);
        return 1;
    case 3:
    	echoIt();
    	return 1;
    case 4:
    	environStrings();
    	return 1;
    case 5:
        openHelp();
        return 1;
    case 6:
        username = getenv("USER");
        printf("\nHello %s.\nMind that this is "
            "not a place to play around."
            "\nUse help to know more..\n",
            username);
        return 1;
    default:
        break;
    }//end switch()

    return 0;
}//end ownCmdHandler()

// function for finding pipe
int parsePipe(char* str, char** strpiped)
{
    int i;
    for (i = 0; i < 2; i++)
    {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
        {
            break;
        }//end if()
    }//end for()

    if (strpiped[1] == NULL)
    {
        return 0; // returns zero if no pipe is found.
    }//end if()
    else
    {
        return 1;
    }//end else()
}//end parsePipe()

// function for parsing command words
void parseSpace(char* str, char** parsed)
{
    int i;

    for (i = 0; i < MAXCOMM; i++)
    {
        parsed[i] = strsep(&str, " ");

        if (parsed[i] == NULL)
        {
            break;
        }//end if()
        if (strlen(parsed[i]) == 0)
        {
            i--;
        }//end if()
    }//end for()
}//end parseSpae()

int processString(char* str, char** parsed, char** parsedpipe)
{

    char* strpiped[2];
    int piped = 0;

    piped = parsePipe(str, strpiped);

    if (piped)
    {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);

    }//end if()
    else
    {

        parseSpace(str, parsed);
    }//end else()

    if (ownCmdHandler(parsed))
    {
        return 0;
    }//end if()
    else
    {
        return 1 + piped;
    }//end else()
}//end processString
