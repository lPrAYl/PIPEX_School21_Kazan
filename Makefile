NAME		= pipex

BONUS		= pipex_bonus

SRC			= 	main.c	exec.c	first_proccess.c	create_proccess.c	utils.c

SRC_B		=	main_bonus.c	exec_bonus.c	first_proccess_bonus.c	create_proccess_bonus.c	utils_bonus.c

SRCDIR		= ./srcs/

SRCS		= $(addprefix $(SRCDIR), $(SRC))

SRCS_B		= $(addprefix $(SRCDIR), $(SRC_B))

OBJS		= $(SRCS:.c=.o)

OBJS_B		= $(SRCS_B:.c=.o)

INCDIR		= ./includes/

LIBFT		= ./libft/libft.a

LIBFLAGS	= -L ./libft -lft

FLAGS		= -Wall -Werror -Wextra

BONUSFLAGS	= -D BONUS=1

HDR			= pipex.h

INCDIR		= ./includes/

HEADER		= $(addprefix $(INCDIR), $(HDR))

%.o:		%.c $(HEADER)
			gcc $(FLAGS) -I $(INCDIR) -c $< -o $@

all:		$(NAME)

$(NAME) :	$(HEADER) $(LIBFT) $(OBJS)
		gcc $(FLAGS) -I $(INCDIR) -c srcs/main.c -o srcs/main.o
		gcc $(FLAGS) -I $(INCDIR) $(OBJS) $(LIBFLAGS) -o $(NAME)

$(BONUS) :	$(HEADER) $(LIBFT) $(OBJS_B)
		gcc $(FLAGS) -I $(INCDIR) -c srcs/main_bonus.c -o srcs/main_bonus.o
		gcc $(FLAGS) -I $(INCDIR) $(OBJS_B) $(LIBFLAGS) -o $(BONUS)

$(LIBFT) :
		@$(MAKE) -C ./libft/

bonus :		$(BONUS)

clean :
		rm -rf $(OBJS) $(OBJS_B)
		@$(MAKE) clean -C ./libft/

fclean :
		rm -rf $(OBJS) $(OBJS_B) $(NAME) $(BONUS)
		@$(MAKE) fclean -C ./libft/

re :		fclean all

.PHONY:	all clean fclean re bonus