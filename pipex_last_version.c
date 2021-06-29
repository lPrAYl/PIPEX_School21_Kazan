#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "pipex.h"
#include "get_next_line.h"

char	*ft_strjoin_sds(char const *s1, char delimeter, char const *s2)
{
	size_t		i;
	size_t		j;
	char		*dest;

	if (!s1 || !s2)
		return (NULL);
	dest = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 2));
	if (!dest)
		return (NULL);
	i = 0;
	while (i < ft_strlen(s1))
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = delimeter;
	i++;
	j = 0;
	while (j < ft_strlen(s2))
	{
		dest[i + j] = s2[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

void	**find_and_separation_path(int argc, char **envp)
{
	int		line;
	char	**chunk;

	line = 0;
	while (envp[line])
	{
		if(ft_strncmp("PATH=", envp[line], 5) == 0)
			break;
		++line;
	}
	chunk = ft_split(envp[line] + 5, ':');
	return (chunk);
}

void	do_execve(char **argv, char **envp, char **chunk, int i)
{
	int		j;
	char	**cmd;
	char	*path;

	j = 0;
	cmd = ft_split(argv[i + 2], ' ');
	while (chunk[j])
	{
		path = ft_strjoin_sds(chunk[j], '/', cmd[0]);
		execve(path, cmd, envp);
		++j;
		free(path);
	}
	perror("Couldn't not find program to execute");
	exit(EXIT_FAILURE);
}

void	creating_pipes(t_pipex *fdp)
{
	int	i;

	i = 0;
	while (i < fdp->num_cmd)
	{
		if(pipe(fdp[i].io) == -1)
		{
			perror("Error with creating pipe");
			exit(EXIT_FAILURE);
		}
		++i;
	}	
}

void	exec_process(int argc, char **argv, char **envp, t_pipex *fdp)
{
	int	i;
	int	pid;

	creating_pipes(fdp);
	i = 0;
	while (i < fdp->num_cmd - 1)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("Error with creating process");
			exit(EXIT_FAILURE);
		}
		if(pid == 0)
		{
			if (i == 0)
				creating_first_child_proccess(argv, fdp);
			if (i != 0)
				creating_child_proccess(argv, fdp);
		}
	}
}

int	main(int argc, char **argv, char **envp)
{
	int		num_cmd;
	t_pipex	*fdp;

	if(argc < 5)
	{
		write(1, "Not enough arguments!\n",  22);
		exit(EXIT_FAILURE);
	}
	fdp->chunk = find_and_separation_path(argc, envp);
	if(argc == 6 && !ft_strncmp(argv[1], "here_doc", 8))
	{
		fdp->num_cmd = argc - 4;
		exec_process(argc, argv, envp, fdp);
	}
	else
	{
		fdp->num_cmd = argc - 3;
		exec_process(argc, argv, envp, fdp);
	}
}

// typedef struct t_list
// {
	// int pp[2];
// } s_list 
// 
// s_list *fdp;
// 
// int fd[num-arg][2]
// fdp[0].pp[1]
// 
// fdp[0][1]
