#include <stdio.h>		//	убрать
#include <errno.h>		//	обработка ошибок errno
#include <string.h>		//	для strerror
#include "libft_done/libft.h"
#include <fcntl.h>		//	для open


#define	NUM_PROCESS 2
//	функция access

int	main(int argc, char **argv, char **env)
{
	int		i;
	int		n;
	char	**chunk;
	char	**cmd;
	char	*path;

	//if (argc < 5)
	//{
	//	printf("%s\n", strerror(errno));	//	нужен ли здесь strerror???
	//	return (1);							//	почитать про exit
	//}

	/*>>>>>>>>>>>>>>>>>>>>>>>парсинг переменной окружения<<<<<<<<<<<<<<<*/
	i = 0;
	while (env[i])
	{
		if (ft_strnstr(env[i], "PATH=", 5))
		{
			n = i;
			break;
		}
		i++;
	}
	chunk = ft_split(env[n] + 5, ':');		//	почистить память

	/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

	

	
	int	pids[NUM_PROCESS];
	int	pipes[NUM_PROCESS + 1][2];


	

	//printf("%s\n", cmd[0]);
	i = 0;
	while (i < NUM_PROCESS + 1)							//	количество процессов + 1
	{
		if (pipe(pipes[i]) == -1)
		{
			printf("Error with creating pipe\n");
			return (1);								//	обработать ошибку
		}
		i++;
	}
	i = 0;
	while (i < NUM_PROCESS)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			printf("Error with creating process\n");
			return (1);								//	обработать ошибку
		}
		if (pids[i] == 0)
		{
			int	j = 0;
			while (j < NUM_PROCESS)
			{
				if (i != j)
					close(pipes[j][0]);
				if (i + 1 != j)
					close(pipes[j][1]);
				j++;
			}		
			//dup2(pipes[i][0], STDIN_FILENO);
			//close(pipes[i][0]);
			//pipes[i + 1][1] = open("outfile", O_WRONLY | O_TRUNC | O_CREAT, 0777);
			//dup2(pipes[i + 1][1], STDOUT_FILENO);
			//close(pipes[i + 1][1]);
			//dup2(outfile, STDOUT_FILENO);
			int	k = 0;
			cmd = ft_split(argv[i + 1], ' ');			//	почистить память	//	получение команды из аргумента строки	//	тут нужно только cmd[0]
			//printf("%s\n", cmd[0]);
			while (chunk[k])
			{
				path = ft_strjoin(chunk[k], "/");	//	почистить память
				path = ft_strjoin(path, cmd[0]);	//	изменить strjoin
				//printf("%s\n", path);
				//printf("%s\n", argv[i]);
				execve(path, &argv[i + 1], env);
				k++;
			}			
		}
		//printf("%d\n", i);
		i++;
	}
	for (int j = 0; j < NUM_PROCESS; j++)
	{
		if (j != NUM_PROCESS)
			close(pipes[j][0]);
		if (j != 0)
			close(pipes[j][1]);
	}

	int infile = open("infile", O_RDONLY, 0777);
	dup2(infile, pipes[0][1]);
	close(infile);
	int outfile = open("outfile", O_WRONLY | O_TRUNC, 0777);
	dup2(outfile, pipes[NUM_PROCESS][0]);
	close(outfile);
	close(pipes[0][1]);
	close(pipes[NUM_PROCESS][0]);
	for (i = 0; i < NUM_PROCESS; i++)
	{
		waitpid(pids[i], NULL, 0);
	}
	execlp("wc", "wc", NULL);
}
