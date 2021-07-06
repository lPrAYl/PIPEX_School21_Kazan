#include "includes/pipex.h"

int	main(int argc, char **argv)
{
	pid_t	pid;
	int		i;
	int		infile;
	int		outfile;
	int		pipex[5][2];
	
	char	*cat[3] = {"/bin/cat", "-e", NULL};

	i = 0;
	if(!ft_strncmp(argv[1], "here_doc", 8))
	{
		pipe(pipex[i]);
		pid = fork();
		if(!pid)
		{
			char	*buf;
			buf = "";
			close(pipex[i][0]);
			while(ft_strncmp(buf, argv[2], ft_strlen(argv[2])))
			{
				if(get_next_line(0, &buf) && ft_strncmp(buf, argv[2], ft_strlen(argv[2])))
				{
					write(pipex[i][1], buf, ft_strlen(buf));
					write(pipex[i][1], "\n", 1);
				}
			}
			return (0);
		}
		i++;
	}
	else
		infile = open("infile", O_RDONLY);
	outfile = open("outfile", O_RDWR | O_CREAT | O_TRUNC, 0777);
	pipe(pipex[i]);
	pid = fork();
	if(!pid)
	{
		if(!ft_strncmp(argv[1], "here_doc", 8))
		{
			close(pipex[i - 1][1]);
			dup2(pipex[i - 1][0], 0); // stdin
		}	
		else
			dup2(infile, 0);
		close(pipex[i][0]);
		dup2(pipex[i][1], 1);
		execve(cat[0], cat, NULL);
	}
	if(pid)
	{
		wait(NULL);
		if(!ft_strncmp(argv[1], "here_doc", 8))
			close(pipex[i - 1][1]); // для here_doc -1
		close(pipex[i][1]);
		while(i < 2)
		{
			i++;
			pipe(pipex[i]);
			pid = fork();
			if(!pid)
			{
				dup2(pipex[i - 1][0], 0);
				close(pipex[i][0]);
				dup2(pipex[i][1], 1);
				execve(cat[0], cat, NULL);
				return (0);
			}
			if(pid)
			{
				wait(NULL);
				if(!ft_strncmp(argv[1], "here_doc", 8))
					close(pipex[i - 1][1]); // для here_doc -1
				close(pipex[i][1]);
			}
		}
	}
	close(pipex[i][1]);
	dup2(pipex[i][0], 0);
	dup2(outfile, 1);
	execve(cat[0], cat, NULL);
	printf("!");
	return (0);
}