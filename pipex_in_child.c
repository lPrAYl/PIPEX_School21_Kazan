#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int	main(int argc, char **argv)
{
	int	fd[2][2];
	int	infile;
	int	outfile;
	int pid1;
	int pid2;

	//if (pipe(fd) < 0)
	//	return (1);		//	обработать ошибку
	pid1 = fork();
	if (pid1 < 0)
		return (2);		//	обработать ошибку
	if (pid1 == 0)
	{
		close(fd[0][1]);
		close(fd[0][0]);
		dup2(fd[0][1], STDIN_FILENO);
		dup2(fd[1][0], STDOUT_FILENO);
		close(fd[1][0]);
		close(fd[1][1]);
		//if (infile < 0)
		//	return (3);		// обработать ошибку
		execlp("cat", "cat", NULL);
	}
	//pid2 = fork();
	//if (pid2 < 0)
	//	return (4);		//	обработать ошибку
	//if (pid2 == 0)
	else
	{
		fd[0][0] = open("infile", O_RDONLY, 0777);
		dup2(fd[0][0], STDOUT_FILENO);
		wait(NULL);
		close(fd[0][0]);
		fd[0][1] = open("outfile", O_WRONLY, 0777);
		dup2(fd[0][1], STDIN_FILENO);
		close(fd[0][1]);

		close(fd[1][0]);
		close(fd[1][1]);
		execlp("wc", "wc", NULL);
	}

	//close(fd[0]);
	//close(fd[1]);
	//waitpid(pid1, NULL, 0);
	//waitpid(pid2, NULL, 0);
	return (0);
}