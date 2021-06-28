#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    int     pipefd[2];
    int     status;
    int     done;
    pid_t   cpid;
	int		infile;
	int		outfile;

    done = 0;
    pipe(pipefd);
	cpid = fork();
	if (cpid == 0)
	{
		//infile = open("infile", O_RDONLY, 0644);
		//dup2(infile, STDIN_FILENO);
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		execlp("cat", "cat", NULL);
	}
	cpid = fork();
	if (cpid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		outfile = open("outfile", O_WRONLY | O_TRUNC, 0644);
		dup2(outfile, STDOUT_FILENO);
		execlp("wc", "wc", NULL);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(-1, &status, 0);
	waitpid(-1, &status, 0);
	return (0);
}