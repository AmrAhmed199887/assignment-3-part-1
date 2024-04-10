
#include <stdlib.h>
#include <unistd.h>
#include "systemcalls.h"
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/wait.h>
/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

int ret;
ret= system(cmd);
if(WIFEXITED(ret))
{
	printf("Normal termination with exit status=%d\n",WEXITSTATUS (ret));
	return true;
}
if(WIFSIGNALED (ret))
{
	printf ("Killed by signal=%d%s\n",WTERMSIG (ret),WCOREDUMP (ret) ? " (dumped core)" : "");
	return false;
}
if (WIFSTOPPED (ret)) 
{ 
	printf ("Stopped by signal=%d\n",WSTOPSIG (ret)); 
	return false;
}
if (WIFCONTINUED (ret)) 
 printf ("Continued\n");

if(ret == -1)
{
	printf("something wronge happened\n");
	return false;
}

printf("undefined termination\n");
    return false;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
int status,ret;
pid_t pid;
bool check=false;
fflush(stdout);
pid= fork();
if(pid == -1)
{
	printf("can't fork the process");
        
	check=false;
}
else if(pid ==0)
{
	ret=execv(command[0],command+1);
	if(ret == -1)
	{
	perror("cant execv the command\n");
        // check=false;
	exit(EXIT_FAILURE);
	}
        
}
else if (pid >0)
{
        pid=wait(&status);
        printf("pid = %d\n",pid);
if(WIFEXITED(status))
{
        printf("Normal termination with exit status=%d\n",WEXITSTATUS (status));
        if(WEXITSTATUS (status)==EXIT_FAILURE)
        {
                check = false;
        }
        else if(EXIT_SUCCESS== WEXITSTATUS (status))
        {
                check = true;
        }
}
if(WIFSIGNALED (status))
{
        printf ("Killed by signal=%d%s\n",WTERMSIG (status),WCOREDUMP (status) ? " (dumped core)" : "");
        if(WEXITSTATUS (status)==EXIT_FAILURE)
        {
                check = false;
        }
        else if(EXIT_SUCCESS== WEXITSTATUS (status))
        {
                check = true;
        }
}
if (WIFSTOPPED (status))
{
        printf ("Stopped by signal=%d\n",WSTOPSIG (status));
        if(WEXITSTATUS (status)==EXIT_FAILURE)
        {
                check = false;
        }
        else if(EXIT_SUCCESS== WEXITSTATUS (status))
        {
                check = true;
        }
}
}



    va_end(args);

    return check;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

int status,fd,ret;
pid_t pid;
bool check=false;


fflush(stdout);
pid= fork();
if(pid == -1)
{
        perror("can't fork the process");
        return false;
}
else if(pid ==0)
{
        fd = open(outputfile,O_RDWR | O_CREAT , 0644);
        if(fd < 0)
        {
        printf("error at open function ");
        return false;
        }    
	if(dup2(fd,1) < 0)
	{
	        perror("error at dup2 phase");
	exit(EXIT_FAILURE);
	}
	close(fd);
        ret=execv(command[0],&command[1]);
        if(ret == -1)
        {
        perror("cant execv the command");
        exit(EXIT_FAILURE);
        }
}
else
{
pid=wait(&status);
if(WIFEXITED(status))
{
        printf("Normal termination with exit status=%d\n",WEXITSTATUS (status));
        if(WEXITSTATUS (status)==EXIT_FAILURE)
        {
                check = false;
        }
        else if(EXIT_SUCCESS== WEXITSTATUS (status))
        {
                check = true;
        }
}
if(WIFSIGNALED (status))
{
        printf ("Killed by signal=%d%s\n",WTERMSIG (status),WCOREDUMP (status) ? " (dumped core)" : "");
        if(WEXITSTATUS (status)==EXIT_FAILURE)
        {
                check = false;
        }
        else if(EXIT_SUCCESS== WEXITSTATUS (status))
        {
                check = true;
        }
}
if (WIFSTOPPED (status))
{
        printf ("Stopped by signal=%d\n",WSTOPSIG (status));
        if(WEXITSTATUS (status)==EXIT_FAILURE)
        {
                check = false;
        }
        else if(EXIT_SUCCESS== WEXITSTATUS (status))
        {
                check = true;
        }
}

}

    va_end(args);

    return check;
}
