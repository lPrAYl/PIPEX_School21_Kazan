#include "pipex.h"

void	create_child_process(int pipefd, char **argv, char **env)
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if(fd == -1)
	{
		close(pipefd);
		perror(argv[1]);
		ft_free_all();
		exit(EXIT_FAILURE);
	}
	else
	{
		dup2(pipefd, STDOUT_FILENO);
		close(pipefd);
		dup2(fd, STDIN_FILENO);
		close(fd);
		do_execve(argv[2], env);
	}
}

void	parent_process(int pipefd, char **argv, char **env)
{
	int	fd;

	wait(NULL);
	fd = open(argv[4], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if(fd == -1)
	{
		close(pipefd);
		perror(argv[4]);
		ft_free_all();
		exit(EXIT_FAILURE);
	}
	else
	{
		dup2(pipefd, STDIN_FILENO);
		close(pipefd);
		dup2(fd, STDOUT_FILENO);
		close(fd);
		do_execve(argv[3], env);
	}
}

#ifdef BONUS
int	main(int argc, char **argv, char **env)
{

}

#else
int	main(int argc, char **argv, char **env)
{
	int	pipefd[2];

	if(argc != 5)
	{
		perror("Incorrect arguments amount")
	}

}