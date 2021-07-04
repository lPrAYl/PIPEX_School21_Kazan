#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

#include "pipex1.h"
#include "gnl/get_next_line.h"
#include "libft/libft.h"

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

char	**find_and_separation_path(char **envp)
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

int	do_execve(char **argv, char **envp, t_pipex *fdp, int i)
{
	int		j;
	char	**cmd;
	char	*path;

	j = 0;
	cmd = ft_split(argv[i + 2], ' ');
	if (i == 1)
		write(2, "uoc\n", 4);
	while (fdp->chunk[j])
	{
		path = ft_strjoin_sds(fdp->chunk[j], '/', cmd[0]);
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
	while (i < fdp->num_cmd - 1)
	{
		if(pipe(fdp[i].io) == -1)
		{
			perror("Error with creating pipe");
			exit(EXIT_FAILURE);
		}
		++i;
	}	
}

void	creating_first_child_proccess(char **argv, t_pipex *fdp)
{
	char	*buffer;
	int		infile;

	if(fdp->flag == 1)
	{
		buffer = "\0";
		close(fdp[0].io[0]);
		while (ft_strncmp(buffer, argv[2], ft_strlen(argv[2])))
		{
			if(get_next_line(0, &buffer) && ft_strncmp(buffer, argv[2], ft_strlen(argv[2])))
			{
				write(fdp[0].io[1], buffer, ft_strlen(buffer));
				write(fdp[0].io[1], "\n", 1);
			}
			free(buffer);
		}
		//dup2(fdp[0].io[1], STDOUT_FILENO);
	}
	else
	{
		infile = open(argv[1], O_RDONLY);
		if(infile < 0)
		{
			perror(argv[1]);
			exit(EXIT_FAILURE);
		}
		if(dup2(infile, STDIN_FILENO) == -1)
		{
			perror("Couldn't read from the file");
			exit(EXIT_FAILURE);
		}
	}
}

void	creating_child_proccess(char **argv, t_pipex *fdp, int i)
{
		close(fdp[i - 1].io[1]);
		if(dup2(fdp[i - 1].io[0], STDIN_FILENO) == -1)
		{
			perror("Couldn't read 555 from the pipe");
			exit(EXIT_FAILURE);
		}
		close(fdp[i - 1].io[0]);
}

void	parent_proccess(int argc, char **argv, char **envp, t_pipex *fdp, int i)
{
	int	outfile;

	if(fdp->flag == 1)
		outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0774);
	else
		outfile = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0774);
	if (outfile == -1)
	{
		perror(argv[argc - 1]);
		exit(EXIT_FAILURE);
	}
	if(dup2(outfile, STDOUT_FILENO) == -1)
	{
		perror("Couldn't read from the file");
		exit(EXIT_FAILURE);
	}
	if(dup2(fdp[fdp->num_cmd - 2].io[0], STDIN_FILENO) == -1)
	{
		perror("Couldn't read from the pipe");
		exit(EXIT_FAILURE);
	}
	
	close(fdp[fdp->num_cmd - 2].io[1]);
	close(fdp[fdp->num_cmd - 2].io[0]);
	do_execve(argv, envp, fdp, i);	
}

void	exec_process(int argc, char **argv, char **envp, t_pipex *fdp)
{
	int		i;
	pid_t	pid;

	creating_pipes(fdp);
	i = 0;
	while (i < fdp->num_cmd)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("Error with creating process");
			exit(EXIT_FAILURE);
		}
		if(pid == 0)
		{
			if(i == 0)
				creating_first_child_proccess(argv, fdp);
			if(i != 0)
				creating_child_proccess(argv, fdp, i);
			if(dup2(fdp[i].io[1], STDOUT_FILENO) == -1)
			{
				perror("Couldn't write 555 to the pipe");
				exit(EXIT_FAILURE);
			}
			close(fdp[i].io[0]);
			close(fdp[i].io[1]);
			if (fdp->flag == 1)
				write(2, "456cou\n", 7);
			if(fdp->flag == 1 && i > 0)
			{
				fdp->flag = 0;
				write(2, "cou\n", 4);
				do_execve(argv, envp, fdp, i + 1);
			} 
			else if (fdp->flag != 1)
			{
				do_execve(argv, envp, fdp, i);
			}
		}
		else
		{
			write(2, "CUV\n", 4);
			wait(NULL);
			close(fdp[i].io[1]);
			if(i)
				close(fdp[i - 1].io[0]);
		}
		++i;
	}
	parent_proccess(argc, argv, envp, fdp, i);
}

int	main(int argc, char **argv, char **envp)
{
	int		num_cmd;
	t_pipex	*fdp;

	fdp = (t_pipex *)malloc(sizeof(t_pipex) * argc);
	if(argc < 5)
	{
		write(1, "Not enough arguments!\n",  22);
		exit(EXIT_FAILURE);
	}
	fdp->chunk = find_and_separation_path(envp);
	if(argc == 6 && !ft_strncmp(argv[1], "here_doc", 8))
	{
		fdp->flag = 1;
		fdp->num_cmd = argc - 4;
		write(2, "MAC\n", 4);
		exec_process(argc, argv, envp, fdp);
	}
	else
	{
		fdp->num_cmd = argc - 3;
		exec_process(argc, argv, envp, fdp);
	}
}
