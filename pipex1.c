#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int	main(int argc, char **argv)
{
	int	fd[2];
	int	infile;
	int	outfile;
	int pid1;
	int pid2;


	if (pipe(fd) < 0)
		return (1);		//	обработать ошибку
	//infile = open("infile", O_RDONLY, 0777);
	pid1 = fork();
	if (pid1 < 0)
		return (2);		//	обработать ошибку
	if (pid1 == 0)
	{
		close(fd[0]);
		infile = open("infile", O_RDONLY, 0777);
		dup2(infile, STDIN_FILENO);
		close(infile);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		
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
		
		outfile = open("outfile", O_RDWR, 0777);
		wait(NULL);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		dup2(outfile, STDOUT_FILENO);
		close(outfile);
		execlp("wc", "wc", NULL);
	}

	close(fd[0]);
	close(fd[1]);
	//waitpid(pid1, NULL, 0);
	//waitpid(pid2, NULL, 0);
	return (0);
}
