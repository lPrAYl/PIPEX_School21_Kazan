#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>

int	main(int argc, char **argv)
{
	int	pid;
	int	error;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		int	file = open("pingResults.txt", O_WRONLY | O_CREAT, 0777);
		if (file == -1)
			return (2);
		dup2(file, STDOUT_FILENO);
		close(file);
	
		error = execlp("ping", "ping", "-c", "1", "google.com", NULL);
		if (error == -1)
		{
			printf("Could not find program to execute!\n");
			return (0);
		}
	}
	else
	{
		int	wstatus;
		wait(&wstatus);
		if (WIFEXITED(wstatus))
		{
			int	statusCode = WEXITSTATUS(wstatus);
			if (statusCode == 0)
				printf("Success!\n");
			else
				printf("Failure with\n");
		}
		printf("Some post processing goes here!\n");
	}
	return (0);
}