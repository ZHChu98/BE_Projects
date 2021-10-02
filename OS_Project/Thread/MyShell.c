#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int parseCommand (char* cmd, char* cmd_array[]) {
	int count = 0;
	char *p;
	p = strtok(cmd, " ");

	while (p != 0) {
		cmd_array[count++] = p;
		p = strtok( NULL, " " );
	}

	cmd_array[count++] = NULL;
    return count;
}

int main()
{
	while(1) 
	{
		printf("myshell> ");

		char *cmd; // store the cmd
		cmd = malloc(sizeof(char)*100);
        
		fgets(cmd, 100, stdin);
		size_t len = strlen(cmd);
		if (len > 0 && cmd[len - 1] == '\n')
			cmd[len - 1] = '\0';

		if (strcmp(cmd, "exit" ) == 0)
			exit(0);

		char *cmd_array[10]; // to store each argument in the command string
        int count = parseCommand(cmd, cmd_array);

		int pid = fork();
		switch (pid) {
		case -1: 
			printf("Error: Failed to fork.\n"); 
			break;
		case 0: {
			if (strcmp(cmd_array[0], "./copy") == 0)
				execvp("./copy", cmd_array);
			else if (strcmp(cmd_array[0], "cp") == 0)
				execvp("/bin/cp", cmd_array);
			else if (strcmp(cmd_array[0], "rm") == 0)
				execvp("/bin/rm", cmd_array);
			else if (strcmp(cmd_array[0], "ls") == 0)
				execvp("/bin/ls", cmd_array);
			else
				printf("command \"%s\" not found.\n", cmd_array[0]);
		}
			exit(0);
		default:
			wait(0);
		}
		free(cmd);
	}      
	exit(0);
}
