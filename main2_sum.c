#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

int	main(int argc, char **argv)
{
	int	arr[5];
	int	fd;
	int	i;

	fd = open("sum", O_RDONLY);
	if (fd == -1)
		return (1);
	for (i = 0; i < 5; i++)
	{
		if (read(fd, &arr[i], sizeof(int)) == -1)
			return (2);
		printf("Received %d\n", arr[i]);
	}
	close(fd);
	return (0);
}