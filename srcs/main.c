#include "../includes/pipex.h"

int	main(int argc, char **argv, char **env)
{
	int		i;
	t_pipex	*fdp;

	if (argc < 5)
		ft_perror("Not enough arguments!");
	i = 0;
	fdp = (t_pipex *)malloc(sizeof(t_pipex) * (argc - 4));
	if (!fdp)
		ft_perror("Couldn't to allocate memory");
	fdp->num_pipe = argc - 4;
	fdp->argc = argc;
	fdp->chunk = find_and_separation_path(env);
	i = process(argv, env, fdp, i);
	close(fdp[i].io[1]);
	if (dup2(fdp[i].io[0], 0) == -1)
		ft_perror("Couldn't read from the pipe");
	if (dup2(fdp->outfile, 1) == -1)
		ft_perror("Couldn't write to the file");
	free(fdp);
	do_execve(argv, env, fdp, i + 1);
	exit(EXIT_SUCCESS);
}
