#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

int	main(int argc, char **argv)
{
	int	arr[5];
	int	i;
	int	fd;
	int	sum;

	srand(time(NULL));
	for (i = 0; i < 5; i++)
	{
		arr[i] = rand() % 100;
		printf("Genereted : %d\n", arr[i]);
	}
	fd = open("sum", O_WRONLY);
	if (fd == -1)
		return (1);
	sum = 0;
	if (write(fd, arr, sizeof(int) * 5) == -1)
		return (2);
	close(fd);
	return (0);
}