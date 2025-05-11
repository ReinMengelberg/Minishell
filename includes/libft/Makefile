CC = cc
# Compiler flags
CFLAGS = -Wall -Werror -Wextra
#source files
SRC =	ft_atoi.c \
	ft_atol.c \
	ft_bzero.c \
	ft_calloc.c \
	ft_dprintf.c \
	ft_isalnum.c \
	ft_isalpha.c \
	ft_isascii.c \
	ft_isdigit.c \
	ft_isprint.c \
	ft_itoa.c \
	ft_memchr.c \
	ft_memcmp.c \
	ft_memcpy.c \
	ft_memmove.c \
	ft_memset.c \
	ft_putchar_fd.c \
	ft_putendl_fd.c \
	ft_putnbr_fd.c \
	ft_putstr_fd.c \
	ft_split.c \
	ft_strchr.c \
	ft_strcmp.c \
	ft_strdup.c \
	ft_striteri.c \
	ft_strjoin.c \
	ft_strlcat.c \
	ft_strlcpy.c \
	ft_strlen.c \
	ft_strmapi.c \
	ft_strncmp.c \
	ft_strnstr.c \
	ft_strrchr.c \
	ft_strtrim.c \
	ft_substr.c \
	ft_tolower.c \
	ft_toupper.c \
	get_next_line.c \
	get_next_line_utils.c \
	handle_char.c \
	handle_hex.c \
	handle_int.c \
	handle_ptr_address.c \
	handle_str.c \
	handle_int.c \
	ft_printf.c

OBJ = $(SRC:.c=.o)
NAME = libft.a
INC = libft.h

# Target for the library
all: $(NAME)

# Rule to create the library
$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

# Rule for creating object files
%.o: %.c $(INC)
	$(CC) $(CFLAGS) -c $< -o ${<:.c=.o}

# Clean rule to remove build artifacts
clean:
	rm -f $(OBJ)

# Full clean rule to remove all build artifacts
fclean: clean
	rm -f $(NAME)

# Rebuild rule to force a rebuild of the project
re: fclean all

# Phony targets
.PHONY: all clean fclean re
