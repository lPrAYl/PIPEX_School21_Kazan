#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "libft_done/libft.h"
#include "gnl/get_next_line.h"

#include <stdio.h>


int	main(int argc, char **argv, char **envp)
{
	int		i;
	int		j;
	int		infile;
	int		outfile;
	int		num_cmd;

	char	**chunk;
	char	**cmd;
	char	*path;
	char	*buffer;


	//	проверка количества аргументов

	if(argc != 6 && !ft_strncmp(argv[1], "here_doc", 8))
	{
		write(1, "Not enough arguments!\n", 22);
		exit(EXIT_FAILURE);
	}

	//	парсинг переменной окружения

	i = 0;
	while(envp[i])
	{
		if(ft_strnstr(envp[i], "PATH=", 5))
			break ;
		i++;
	}
	chunk = ft_split(envp[i] + 5, ':');		//	почистить память

	//	creating pipes

	num_cmd = argc - 4;
	int	pids[num_cmd + 1];
	int	pipes_fd[num_cmd][2];

	i = 0;
	while (i < num_cmd)
	{
		if(pipe(pipes_fd[i]) == -1)
		{
			perror("Error with creating pipe");
			exit(EXIT_FAILURE);
		}
		++i;
	}

	//	creating process

	i = 0;
	while (i < num_cmd - 1)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("Error with creating process");
			exit(EXIT_FAILURE);
		}
		if(pids[i] == 0)
		{
			j = 0;
			while (j < num_cmd - 1)
			{
				if (i != j)
					close(pipes_fd[j][1]);
				if (i - 1 != j)
					close(pipes_fd[j][0]);
				++j;
			}
			if(i == 0)
			{
				while(ft_strncmp(buffer, argv[2], ft_strlen(argv[2])))
				{
					if(get_next_line(0, &buffer) && ft_strncmp(buffer, argv[2], ft_strlen(argv[2])))
					{
						write(pipes_fd[i][1], buffer, ft_strlen(buffer));
						write(pipes_fd[i][1], "\n", 1);
					}
					free(buffer);
				}
        	}
			if (i != 0)
			{
				if(dup2(pipes_fd[i - 1][0], STDIN_FILENO) == -1)
				{
					perror("Couldn't read from the pipe");
					exit(EXIT_FAILURE);
				}
				if(dup2(pipes_fd[i][1], STDOUT_FILENO) == -1)
				{
					perror("Couldn't write to the pipe");
					exit(EXIT_FAILURE);
				}
				close(pipes_fd[i][0]);
				close(pipes_fd[i + 1][1]);
				j = 0;
				cmd = ft_split(argv[i + 2], ' ');
				while (chunk[j])
				{
					path = ft_strjoin(chunk[j], "/");	//	изменить strjoin
					path = ft_strjoin(path, cmd[0]);	//	почистить память
					execve(path, cmd, envp);
					++j;
				}
				perror("Couldn't not find program to execute");
			}
		}
		else
		{
			wait(NULL);
			close(pipes_fd[i][1]);
		}
		++i;
	}

	j = 0;
	//while (j < num_cmd - 1)
	//{
	//	if (j != num_cmd - 2)
	//		close(pipes_fd[j][1]);
	//	if (j != num_cmd -2)
	//		close(pipes_fd[j][0]);
	//	++j;
	//}
	outfile = open(argv[argc - 1], O_CREAT | O_WRONLY | O_APPEND, 0777);
	if(outfile == -1)
	{
		perror("Not possible to open the file for writting");
		exit(EXIT_FAILURE);
	}
	if(dup2(outfile, STDOUT_FILENO) == -1)
	{
		perror("Couldnt't write to the file");
		exit(EXIT_FAILURE);
	}
	if(dup2(pipes_fd[num_cmd - 2][0], STDIN_FILENO) == -1)
	{
		perror("Couldn't read from the pipe");
		exit(EXIT_FAILURE);	
	}
	write(1, "hi\n", 3);
	write(2, "ha\n", 3);
	close(pipes_fd[num_cmd - 2][0]);
	close(pipes_fd[num_cmd - 2][1]);
	j = 0;
	cmd = ft_split(argv[i + 2], ' ');
	while (chunk[j])
	{
		path = ft_strjoin(chunk[j], "/");	//	изменить strjoin
		path = ft_strjoin(path, cmd[0]);	//	почистить память
		execve(path, cmd, envp);
		++j;
	}
	perror("Couldn't not find program to execute");
}

