#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include "../libft/libft.h"

typedef struct s_pipex
{
	int		argc;
	int		io[2];
	int		infile;
	int		outfile;
	int		num_pipe;
	char	**chunk;

}			t_pipex;

char	*ft_strjoin_sds(char const *s1, char delimeter, char const *s2);
char	**find_and_separation_path(char **envp);
void	do_execve(char **argv, char **envp, t_pipex *fdp, int i);
void	child_proccess(char **argv, char **env, t_pipex *fdp, int i);
void	parent_process(char **argv, t_pipex *fdp, int i);
void	heredoc(char **argv, t_pipex *fdp);
void	first_child_proccess(char **argv, char **env, t_pipex *fdp, int i);
void	ft_perror(char *str);
int	multiply_cmd(char **argv, char **env, t_pipex *fdp, int i);
int	reading(char **argv, t_pipex *fdp, int i);
int	process(char **argv, char **env, t_pipex *fdp, int i);
int	ft_strncmp_pipex(const char *s1, const char *s2, size_t n);

#endif