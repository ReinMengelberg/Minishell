CC = cc
CFLAGS = -Wall -Werror -Wextra
NAME = pipex
INCLUDES = -I./libft/includes
LIBFT = libft/libft.a

SRCS =	pipex_cmd.c \
		pipex_error.c \
		pipex_fds.c \
		pipex_heredoc.c \
		pipex_process.c \
		pipex_utils.c \
		main.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(INCLUDES) -o $(NAME)

$(LIBFT):
	make -C libft

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME)
	make -C libft fclean

re: fclean all
