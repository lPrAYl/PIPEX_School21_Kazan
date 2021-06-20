#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

// 2 processes
// 1) Child process should generate random numbers and send them to the parent
// 2) Parrent is going to sum all the numbers and print the result

int	main(int argc, char **argv)
{
	int	pid;
	int	fd[2];

	if (pipe(fd) == -1)
		return (2);
	pid = fork();
	if (pid == -1)
		return (1);

	if (pid == 0)
	{
		close(fd[0]);
		int	i;
		int	n;
		int	arr[10];
		srand(time(NULL));
		n = rand() % 10 + 1;
		printf("Generated : \n");
		for (i = 0; i < n; i++)
		{
			arr[i] = rand() % 11;
			printf("%d\n", arr[i]);
		}
		if (write(fd[1], &n, sizeof(int)) == -1)
			return (4);
		printf("Sent n = %d\n", n);
		if (write(fd[1], arr, sizeof(int) * n) == -1)
			return (3);
	}
	else
	{
		close(fd[1]);
		int	arr[10];
		int	n, i, sum =0;
		if (read(fd[0], &n, sizeof(int)) == -1)
			return (5);
		if (read(fd[0], arr, sizeof(int) * n) == -1)
			return (6);
		close(fd[0]);
		for (i = 0; i < n; i++)
			sum += arr[i];
		printf("Result is : %d", sum);
		wait(NULL);
	}
	return (0);
}