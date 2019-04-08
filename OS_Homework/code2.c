#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
	int pid[2];
	int finish;
	pid[0] = fork();
	pid[1] = fork();

	if (pid[1] > 0) 
	{
		printf("I(%d) am the parent process, I have two child processes: %d and %d.\n", getpid(), pid[0], pid[1]);
   		finish = wait(0);
   		printf("The child process %d is done.\n", finish);
	} 

	else if (pid[1] == 0) 
	{
   		printf("I(%d) am the child process.\n", getpid());
   		printf("The child process %d returns.\n", getpid());
	} 

	else printf ("fork error\n");
}


