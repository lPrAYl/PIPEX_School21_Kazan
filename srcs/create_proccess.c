#include "../includes/pipex.h"

void	child_proccess(char **argv, char **env, t_pipex *fdp, int i)
{
	if (dup2(fdp[i - 1].io[0], 0) == -1)
		ft_perror("Couldn't read from the pipe");
	close(fdp[i].io[0]);
	if (dup2(fdp[i].io[1], 1) == -1)
		ft_perror("Couldn't write to the pipe");
	free(fdp);
	do_execve(argv, env, fdp, i);
}

void	parent_process(char **argv, t_pipex *fdp, int i)
{
	wait(NULL);
	if (!ft_strncmp(argv[1], "here_doc", 8))
		close(fdp[i - 1].io[1]);
	close(fdp[i].io[1]);
}

int	multiply_cmd(char **argv, char **env, t_pipex *fdp, int i)
{
	pid_t	pid;

	parent_process(argv, fdp, i);
	while (i < fdp->num_pipe - 1)
	{
		i++;
		if (pipe(fdp[i].io) == -1)
			ft_perror("Error with creating pipe");
		pid = fork();
		if (pid == -1)
			ft_perror("Error with creating proccess");
		if (!pid)
			child_proccess(argv, env, fdp, i);
		if (pid)
			parent_process(argv, fdp, i);
	}
	return (i);
}

int	reading(char **argv, t_pipex *fdp, int i)
{
	if (!ft_strncmp(argv[1], "here_doc", 8))
	{		
		heredoc(argv, fdp);
		fdp->outfile = open(argv[fdp->argc - 1], \
								O_RDWR | O_CREAT | O_APPEND, 0777);
		i++;
	}
	else
	{
		fdp->infile = open(argv[1], O_RDONLY);
		if (fdp->infile == -1)
			ft_perror(argv[1]);
		fdp->outfile = open(argv[fdp->argc - 1], \
								O_RDWR | O_CREAT | O_TRUNC, 0777);
	}
	return (i);
}

int	process(char **argv, char **env, t_pipex *fdp, int i)
{
	pid_t	pid;

	i = reading(argv, fdp, i);
	if (fdp->outfile == -1)
		ft_perror(argv[fdp->argc - 1]);
	if (pipe(fdp[i].io) == -1)
		ft_perror("Error with creating pipe");
	pid = fork();
	if (pid == -1)
		ft_perror("Error with creating proccess");
	if (!pid)
		first_child_proccess(argv, env, fdp, i);
	if (pid)
		i = multiply_cmd(argv, env, fdp, i);
	return (i);
}