#include "pipex.h"

static void	connect_pipe(int pipefd[2], int io)
{
	dup2(pipefd[io], io);
	close(pipefd[0]);
	close(pipefd[1]);
}

static void	cmd_init(const char *cmd, t_cmd *strt)
{
	char	**chunk;

	chunk = ft_split(cmd, ' ');
	strt->cmd[0] = ft_strjoin("/bin", chunk[0]);
	strt->cmd[1] = ft_strjoin("/usr/local/bin", chunk[0]);
	strt->cmd[2] = ft_strjoin("/usr/bin", chunk[0]);
	strt->cmd[3] = ft_strjoin("/usr/sbin", chunk[0]);
	strt->cmd[4] = ft_strjoin("/sbin", chunk[0]);
	strt->argv = (const char **)chunk;
}

static void		run_cmd(const char *cmd, t_cmd *cmd_arg)
{
	int			i;
	extern char	**environ;

	i = 0;
	cmd_init(cmd, cmd_arg);
	while (i < 5)
		execve(cmd_arg->cmd[i++], cmd_arg, environ);
	perror(cmd_arg->argv[0]);
}

int	init_input(t_input *input, int argc, char **argv)
{
	if(argc == 5)
	{
		input->f1 = argv[1];
		input->f2 = argv[4];
		input->cmd1 = argv[2];
		input->cmd2 = argv[3];
	}
	else if(argc == 6)
	{
		if(ft_strncmp(argv[1], "here_doc", 9))
			return (0);
		input->f2 = argv[5];
		input->cmd1 = argv[3];
		input->cmd2 = argv[4];
	}
	else
		return (0);
	return (1);
}

int	main(int argc, char **argv)
{
	int			pipefd[2];
	pid_t		pid;
	t_cmd		cmd;
	t_input		input;

	if(!init_input(&input, argc, argv))
		return (0);
	pipe(pipefd);
	pid = fork;
	if(pid > 0)
	{
		redirect_out(input.f2, argc);
		connect_pipe(pipefd, STDIN_FILENO);
		run_cmd(input.cmd2, &cmd);
	}
	else if(pid == 0)
	{
		if(argc == 6)
			heredoc(argv);
		else
			redirect_in(input.f1);
		connect_pipe(pipefd, STDOUT_FILENO);
		run_cmd(input.cmd1, &cmd); 
	}
	return (0);
}