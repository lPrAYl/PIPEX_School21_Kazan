#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "libft_done/libft.h"
#include <errno.h>
#include <string.h>	
#include <stdio.h>

int	main(int argc, char **argv, char **envp)
{
	int		i;
	int		j;
	int		infile;
	int		count;
	int		num_argc;
	char	*path;
	char	**dv;
	char	**cmd1;

	count = 0;
	num_argc = argc - 3;		//	number of process
	int	pid[num_argc + 1];		//	number of children plus 1 parent
	if (argc < 5)
	{
		perror("Not enough arguments!\n");
		return (1);
	}
	printf("hello!");
	while (envp[count])
	{
		if ((ft_strncmp("PATH=", envp[count], 5)) == 0)
			break;
		count++;
	}
	//	creating pipes
	int	fdp[num_argc][2];

	i = 0;
	while (i < num_argc - 1)
	{
		if (pipe(fdp[i]) == -1)
		{
			perror("Error with creating pipe!\n");
			return (2);
		}
		++i;
	}
	i = 0;
	//	creating process
	while (i < num_argc - 1)
	{
		pid[i] = fork();
		if (pid[i] == -1)
		{
			perror("Error with creating process!\n");
			return (3);
		}
		if (pid[i] == 0)
		{
			if (i == 0)
			{
				infile = open(argv[1], O_RDONLY);
				if (infile < 0)
				{
					perror(argv[1]);
					exit(2);
				}
				if (dup2(infile, STDIN_FILENO) < 0)
					perror("Couldn't read from the file!\n");
			}
			if (i != 0)
			{
				close(fdp[i - 1][1]);
				if (dup2(fdp[i - 1][0], STDIN_FILENO) < 0)
					perror("Couldn't read from the pipe!\n");
				close(fdp[i - 1][0]);
			}
			if (dup2(fdp[i][1], STDOUT_FILENO) < 0)
				perror("Could't write to the pipe!\n");
			close(fdp[i][0]);
			close(fdp[i][1]);
			path = envp[count] + 5;
			dv = ft_split(path, ':');
			cmd1 = ft_split(argv[2 + i], ' ');
			j = 0;
			char	*str;
			while (dv[j])
			{
				str = ft_strjoin(dv[j], "/");
				str = ft_strjoin(str, cmd1[0]);
				execve(str, cmd1, envp);
				++j;
			}
			perror("child");
			return (0);
		}
		else
		{
			wait(NULL);
			close(fdp[i][1]);
		}
		i++;
	}
	int fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0774);
	dup2(fd, STDOUT_FILENO);
	dup2(fdp[num_argc - 2][0], STDIN_FILENO);
	close(fdp[num_argc - 2][1]);
	close(fdp[num_argc - 2][0]);
	path = envp[count] + 5;
	dv = ft_split(path, ':');
	cmd1 = ft_split(argv[2 + i], ' ');
	j = 0;
	char	*str;
	while (dv[j])
	{
		str = ft_strjoin(dv[j], "/");
		str = ft_strjoin(str, cmd1[0]);
		execve(str, cmd1, envp);
		++j;
	}
	perror("parent");
}