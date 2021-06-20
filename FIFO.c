#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

int	main(int argc, char **argv)
{
	int	fd;
	int	x;

	unlink("myfifo1");
	if (mkfifo("myfifo1", 0777) == -1)
	{
		if (errno != EEXIST)
		{
			printf("Could not create fifo file\n");
			return (1);
		}
	}
	printf("Openning...\n");
	fd = open("myfifo1", O_WRONLY);
	printf("Opened\n");
	x = 97;
	if (write(fd, &x, sizeof(x)) == -1)
		return (2);
	printf("Written\n");
	close(fd);
	printf("Closed\n");
	return (0);
}