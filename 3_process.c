#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

int	main(int argc, char **argv)
{
	int	id1, id2;
	int	fd[2];
	int	start;
	int	end;
	int	arrSize;
	int	i;
	int	sum;
	int	arr[] = { 1, 8, 3, 4, 1, 2 };

	arrSize = sizeof(arr) / sizeof(int);
	if (pipe(fd) == -1)
	{
		printf("An error occured with pipe\n");
		return (1);
	}
	id1 = fork();
	if (id1 != 0)
		id2 = fork();
	if (id1 == -1 || id2 == -1)
	{
		printf("An error occured with fork\n");
		return (2);
	}
	if (id1 != 0 && id2 != 0)
	{
		start = 0;
		end = arrSize / 3;
	}
	else
	{
		if (id1 == 0)
		{
			start = arrSize / 3;
			end = 2 * arrSize / 3;
		}
		else
		{
			start = 2 * arrSize / 3;
			end = arrSize;
		}
	}
	sum = 0;
	for (i = start; i < end; i++)
		sum += arr[i];
	printf("Calculated partial sum : %d\n", sum);
	if (id1 == 0 || id2 == 0)
	{
		close(fd[0]);
		write(fd[1], &sum, sizeof(sum));
		close(fd[1]);
	}
	else
	{
		int	sumFromChild1;
		int	sumFromChild2;
		close(fd[1]);
		read(fd[0], &sumFromChild1, sizeof(sumFromChild1));
		read(fd[0], &sumFromChild2, sizeof(sumFromChild2));
		int	totalSum = sum + sumFromChild1 + sumFromChild2;
		printf("Total sum is %d\n", totalSum);
		wait(NULL);
	}
	return (0);
}