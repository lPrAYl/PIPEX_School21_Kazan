#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "../libft/libft.h"

typedef struct s_pipex
{
	int	io[2];
	char	**chunk;
}			t_pipex;

//typedef struct	s_cmd
//{
//	const char	*cmd[5];
//	const char	**argv;
//	const char 	**envp;
//}				t_cmd;
//
//typedef struct	s_input
//{
//	const char	*f1;
//	const char	*f2;
//	const char	*cmd1;
//	const char	*cmd2;
//}				t_input;
//
//int		redirect_in(const char *file);
//int		redirect_out(const char *file, int argc);
//void	heredoc(char **argv);

#endif