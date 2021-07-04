#include "includes/pipex.h"

int	main(int argc, char **argv)
{
	pid_t	pid;
	int		infile;
	int		outfile;
	int		pipex[2];
	int		pipex2[2];
	int		pipex3[2];

	char	*cat[3] = {"/bin/cat", "-e", NULL};

	//pipe(pipex3);
	//pid = fork();
	//if(!pid)
	//{
	//	char	*buf;
	//	buf = "";
	//	close(pipex3[0]);
	//	while(ft_strncmp(buf, argv[2], ft_strlen(argv[2])))
	//	{
	//		if(get_next_line(0, &buf) && ft_strncmp(buf, argv[2], ft_strlen(argv[2])))
	//		{
	//			write(pipex3[1], buf, ft_strlen(buf));
	//			write(pipex3[1], "\n", 1);
	//		}
	//	}
	//	return (0);
	//}
	infile = open("infile", O_RDONLY);
	outfile = open("outfile", O_RDWR | O_CREAT | O_TRUNC, 0777);
	pipe(pipex);
	//close(pipex3[1]);
	pid = fork();
	if(!pid)
	{
		//dup2(pipex3[0], 0); // stdin
		dup2(infile, 0);
		close(pipex[0]);
		dup2(pipex[1], 1);
		execve(cat[0], cat, NULL);
	}
	//if(pid)
	//{
	//	wait(NULL);
	//	close(pipex[1]);
	//	pipe(pipex2);
	//	pid = fork();
	//	if(!pid)
	//	{
	//		dup2(pipex[0], 0);
	//		close(pipex2[0]);
	//		dup2(pipex2[1], 1);
	//		execve(cat[0], cat, NULL);
	//		return (0);
	//	}
	//	if(pid)
	//	{
	//		wait(NULL);
	//		close(pipex2[1]);
	//		pipe(pipex);
	//		pid = fork();
	//		if(!pid)
	//		{
	//			dup2(pipex2[0], 0);
	//			close(pipex[0]);
	//			dup2(pipex[1], 1);
	//			execve(cat[0], cat, NULL);
	//			return (0);
	//		}
	//	}
		close(pipex[1]);
		dup2(pipex[0], 0);
		dup2(outfile, 1);
		execve(cat[0], cat, NULL);
	//}
	printf("!");
	return (0);
}