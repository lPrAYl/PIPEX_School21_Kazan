#include "pipex.h"

int	redirect_in(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if(fd == -1)
	{
		perror(file);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	exit(EXIT_SUCCESS);
}

int	redirect_out(const char *file, int	argc)
{
	int	fd;

	if(argc == 6)
		fd = open(file, O_RDWR | O_CREAT | O_APPEND, 0644);
	else
		fd = open(file, O_RDWR | O_CREAT | O_TRUNC, 0644);
	if(fd == -1)
	{
		perror(file);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	exit(EXIT_SUCCESS);
}