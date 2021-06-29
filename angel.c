#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include "pipex.h"
#include "get_next_line.h"

//void	do_execve(char **envp, char **argv, int i, int count)
//{
//	char *path;
//	char **dv;
//	char **cmd1;
//	char *str;
//	int		j;
//	j = 0;
//	path = envp[count] + 5;
//	dv = ft_split(path, ':');
//	cmd1 = ft_split(argv[2 + i], ' ');
//	while (dv[j])
//	{
//		str = ft_strjoin3(dv[j], "/", cmd1[0]);
//		execve(str, cmd1, envp);
//		j++;
//		//doesn't free when is executed
//		free(str);
//	}
//}

//int find_path(char **envp, int argc)
//{
//	int	count;
//
//	count = 0;
//	if (argc < 5)
//	{
//		perror("Not enough arguments");
//		return (-1);
//	}
//	while (envp[count])
//	{
//		if ((ft_strncmp("PATH=", envp[count], 5)) == 0)
//			break;
//		count++;
//	}
//	return (count);
//}

int exec_process(int argc, char **argv, int count, char **envp, int num_argc, t_arg *fdp)
{
	int pid;
	int	i;
	int fd;

	i = 0;

	//creating pipes

	while (i < num_argc - 1)
	{
		if (pipe(fdp[i].pp) == -1)
		{
			perror("Error with creating pipe\n");
			return (1);
		}
		i++;
	}
	i = 0;

	//creating process
	while (i < num_argc - 1)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("Error with creating process\n");
			return (2);
		}
		if (pid == 0)
		{
			int	fd;
			char *buf;
		//child process
			//first pipe
			if (i == 0)
			{
				//limiter
				if (fdp->flag == 1)
				{
					buf = NULL;
					close(fdp[i].pp[0]);
					while(ft_strncmp(buf, argv[2], ft_strlen(argv[2])))
					{
						if(get_next_line(0, &buf) && ft_strncmp(buf, argv[2], ft_strlen(argv[2])))
						{
							write(fdp[i].pp[1], buf, ft_strlen(buf));
							write(fdp[i].pp[1], "\n", 1);
						}
						free(buf);
					}
				}
				//multiple pipes
				else
				{
					fd = open(argv[1], O_RDONLY);
					if (fd < 0)
					{
						perror(argv[1]);
						exit(2);
					}
					if (dup2(fd, STDIN) < 0)
						perror("Couldn't read from the file");
				}
			}
			//next pipes
			if (i != 0)
			{
				close(fdp[i - 1].pp[1]);
				if (dup2(fdp[i - 1].pp[0], STDIN) < 0)
					perror("Couldn't read from the pipe1");
				close(fdp[i - 1].pp[0]);
			}
			if (dup2(fdp[i].pp[1], STDOUT) < 0)
				perror("Couldn't write to the pipe1");
			close(fdp[i].pp[0]);
			close(fdp[i].pp[1]);

			//execute
			if (fdp->flag == 1 && i > 0)
			{
				do_execve(envp, argv, i + 1, count);
				perror("child");
			}
			else if (fdp->flag != 1)
			{
				do_execve(envp, argv, i, count);
				perror("child");
			}
			return (3);
		}
		else
		{
			wait(0);
			close((fdp[i].pp[1]));
			if (i)
				close(fdp[i - 1].pp[0]);
		}
		i++;
	}
	//parent process
	if (fdp->flag == 1)
		fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 00774);
	else
		fd = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 00774);
	if (fd < 0)
	{
		perror(argv[4]);
		exit(2);
	}
	if (dup2(fd, STDOUT) < 0)
		perror("Couldn't read from the file");
	if (dup2(fdp[num_argc - 2].pp[0], STDIN) < 0)
		perror("Couldn't read from the pipe2");
	close(fdp[num_argc - 2].pp[1]);
	close(fdp[num_argc - 2].pp[0]);
	do_execve(envp, argv, i, count);
	perror("parent");
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	int		i;
	int		count;
	t_arg	*fdp;
	int		num_argc;

	num_argc = argc - 3;
	fdp =  (t_arg *)malloc(sizeof (t_arg) * argc);
	i = 0;
	count = find_path(envp, argc);
	if (count == -1)
		return (1);
	if (argc < 5)
	{
		perror("Not enough arguments");
		exit(EXIT_FAILURE);
	}
	if (!ft_strncmp(argv[1], "here_doc", 8))
	{
		if (argc < 6)
		{
			perror("Not enough arguments");
			exit(EXIT_FAILURE);
		}
		fdp->flag = 1;
		num_argc = argc - 4;
	}
	if (exec_process(argc, argv, count, envp, num_argc, fdp))
		return (2);
	//segfault ./a.out
	//seeking in current directory
	//initialize fds
	//посмотри права доступа функцией access()

}