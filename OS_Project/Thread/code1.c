#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() 
{
	int finish;
	int record = fork();
	
	if (record > 0)
	{
		printf("I(%d) am the parent process, I have a child process: %d.\n", getpid(), record);
		int status = 0;
		finish = wait(&status);
		printf("The child process %d is done.\n", finish);
	}
	
	else if (record == 0)
	{
		printf("I(%d) am the child process.\n", getpid());
		printf("The child process %d returns.\n", getpid());
	}
	
	else printf("fork error\n");	
}
