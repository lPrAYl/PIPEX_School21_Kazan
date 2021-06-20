#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


int	x = 0;

void	handle_sigusr1(int sig)
{
	if (x == 0)
		printf("(HINT) Rememmber that multiplication is repetitive addititon!\n");
}

int	main(int argc, char **argv)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		sleep(1);
		kill(getpid(), SIGUSR1);
	}
	else
	{
		struct sigaction	sa;

		sa.sa_flags = SA_RESTART;
		sa.sa_handler = &handle_sigusr1;
		sigaction(SIGUSR1, &sa, NULL);
		printf("What is the result of 3 x 5 : ");
		scanf("%d", &x);
		if (x == 15)
			printf("Right!");
		else
			printf("Wrong!");
		wait(NULL);
	}
	return (0);
}