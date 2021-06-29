#ifndef PIPEX_H
# define PIPEX_H

typedef struct s_pipex
{
	int	io[2];
	int	num_cmd;
	int	flag;
	char	**chunk;
}	t_pipex;


#endif