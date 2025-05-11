CC = cc
CFLAGS = -Wall -Werror -Wextra
NAME = minishell
LIBFT_DIR = ./includes/libft
LIBFT = $(LIBFT_DIR)/libft.a
INCLUDES = -I./includes -I$(LIBFT_DIR)
LIBS =

SRCS =	./src/main/main.c \
		./src/builtin/cd.c \
		./src/builtin/echo.c \
		./src/builtin/exit.c \
		./src/builtin/export.c \
		./src/builtin/pwd.c \
		./src/builtin/unset.c

OBJ_DIR = objs
OBJS = $(patsubst ./src/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Configure readline library based on OS
ifeq ($(shell uname), Darwin)
    # Check if using Homebrew
    BREW_PREFIX = $(shell brew --prefix 2>/dev/null || echo "/usr/local")
    # Add readline include and lib paths
    INCLUDES += -I$(BREW_PREFIX)/include
    LIBS += -L$(BREW_PREFIX)/lib -lreadline
else
    # Linux case
    LIBS += -lreadline
endif

all: $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)/main
	@mkdir -p $(OBJ_DIR)/builtin

$(NAME): $(OBJ_DIR) $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(INCLUDES) $(LIBS) -o $(NAME)
	@cat ./includes/armadillo.txt

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(OBJ_DIR)/%.o: ./src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "Clean complete"

re: fclean all

.PHONY: all clean fclean re