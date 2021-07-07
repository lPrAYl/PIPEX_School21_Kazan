NAME		= pipex

BONUS		= pipex_bonus

SRC			= 	main.c	exec.c	first_proccess.c	create_proccess.c	utils.c

SRCDIR		= ./srcs/

SRCS		= $(addprefix $(SRCDIR), $(SRC))

OBJS		= $(SRCS:.c=.o)

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

$(BONUS) :	$(HEADER) $(LIBFT) $(OBJS)
		gcc $(FLAGS) $(BONUSFLAGS) -I $(INCDIR) -c srcs/main.c -o srcs/main.o
		gcc $(FLAGS) -I $(INCDIR) $(OBJS) $(LIBFLAGS) -o $(BONUS)

$(LIBFT) :
		@$(MAKE) -C ./libft/

bonus :		$(BONUS)

clean :
		rm -rf $(OBJS)
		@$(MAKE) clean -C ./libft/

fclean :
		rm -rf $(OBJS) $(NAME) $(BONUS)
		@$(MAKE) fclean -C ./libft/

re :		fclean all

.PHONY:	all clean fclean re bonus