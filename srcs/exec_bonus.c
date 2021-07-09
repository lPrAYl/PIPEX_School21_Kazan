#include "../includes/pipex.h"

char	**find_and_separation_path(char **envp)
{
	int		line;
	char	**chunk;

	line = 0;
	while (envp[line])
	{
		if (ft_strncmp("PATH=", envp[line], 5) == 0)
			break ;
		++line;
	}
	chunk = ft_split(envp[line] + 5, ':');
	return (chunk);
}

void	do_execve(char **argv, char **envp, t_pipex *fdp, int i)
{
	int		j;
	char	**cmd;
	char	*path;

	j = 0;
	cmd = ft_split(argv[i + 2], ' ');
	while (fdp->chunk[j])
	{
		path = ft_strjoin_sds(fdp->chunk[j], '/', cmd[0]);
		execve(path, cmd, envp);
		++j;
		free(path);
	}
	free(cmd);
	free(fdp);
	ft_perror("Couldn't not find program to execute");
}
