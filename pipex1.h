#ifndef PIPEX1_H
# define PIPEX1_H

typedef struct s_pipex
{
	int	flag;
	int	io[2];
	int	num_cmd;
	char	**chunk;


}		t_pipex;


#endif