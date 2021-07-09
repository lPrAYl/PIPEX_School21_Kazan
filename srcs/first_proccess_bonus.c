#include "../includes/pipex.h"

void	heredoc(char **argv, t_pipex *fdp)
{
	char	*buf;
	pid_t	pid;

	if (pipe(fdp[0].io) == -1)
		ft_perror("Error with creating pipe");
	pid = fork();
	if (pid == -1)
		ft_perror("Error with creating proccess");
	if (!pid)
	{
		buf = "";
		close(fdp[0].io[0]);
		while (ft_strcmp(argv[2], buf))
		{
			ft_putstr_fd("heredoc> ", 1);
			if (get_next_line(0, &buf) \
								&& ft_strcmp(argv[2], buf))
			{
				write(fdp[0].io[1], buf, ft_strlen(buf));
				write(fdp[0].io[1], "\n", 1);
			}
			free(buf);
		}
		exit(EXIT_SUCCESS);
	}	
}

void	first_child_proccess(char **argv, char **env, t_pipex *fdp, int i)
{
	if (!ft_strcmp(argv[1], "here_doc"))
	{
		close(fdp[i - 1].io[1]);
		if (dup2(fdp[i - 1].io[0], 0) == -1)
			ft_perror("Couldn't read from the pipe");
		close(fdp[i].io[0]);
		if (dup2(fdp[i].io[1], 1) == -1)
			ft_perror("Couldn't write to the pipe");
		do_execve(argv, env, fdp, i);
	}	
	else
	{
		if (dup2(fdp->infile, 0) == -1)
			ft_perror("Couldn't read from the file");
		close(fdp[i].io[0]);
		if (dup2(fdp[i].io[1], 1) == -1)
			ft_perror("Couldn't write to the pipe");
		do_execve(argv, env, fdp, i);
	}
}
