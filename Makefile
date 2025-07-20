CC = cc
CFLAGS =-g3
NAME = minishell
LIBFT_DIR = ./includes/libft
LIBFT = $(LIBFT_DIR)/libft.a
INCLUDES = -I./includes -I$(LIBFT_DIR)
LIBS =

SRCS =	./src/builtin/cd.c \
		./src/builtin/echo.c \
		./src/builtin/exit.c \
		./src/builtin/export.c \
		./src/builtin/pwd.c \
		./src/builtin/unset.c \
		./src/builtin/env.c \
		./src/builtin/builtin.c \
		./src/input/token_utils.c \
		./src/input/token_utils2.c \
		./src/input/token_utils3.c \
		./src/input/tokenizer.c \
		./src/input/tokenizer_creation.c \
		./src/input/tokenizer_types.c \
		./src/input/tokenizer_utils.c \
		./src/input/tokenizer_validation.c \
		./src/input/expander.c \
		./src/input/expander_utils.c \
		./src/input/expander_utils2.c \
		./src/input/heredoc.c \
		./src/input/parser_utils.c \
		./src/input/parser_utils2.c \
		./src/input/parser.c \
		./src/execution/execution.c \
		./src/execution/execution_utils.c \
		./src/execution/pipeline.c \
		./src/execution/command_utils.c \
		./src/execution/cleanup.c \
		./src/execution/path.c \
		./src/execution/redirections.c \
		./src/execution/redirections_utils.c \
		./src/env/env_init.c \
		./src/env/env_service.c \
		./src/env/env_utils.c \
		./src/main/main.c \
		./src/main/signalhandlers.c \
		./src/main/signalsetup.c \
		./src/main/free.c \
		./src/main/helpers.c \

OBJ_DIR = objs
OBJS = $(patsubst ./src/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Configure readline library based on OS
ifeq ($(shell uname), Darwin)
    BREW_PREFIX = $(shell brew --prefix 2>/dev/null || echo "/opt/homebrew")
    READLINE_PREFIX = $(BREW_PREFIX)/opt/readline
    INCLUDES += -I$(READLINE_PREFIX)/include
    LIBS += -L$(READLINE_PREFIX)/lib -lreadline
    $(info Using BREW_PREFIX: $(BREW_PREFIX))
    $(info Using READLINE_PREFIX: $(READLINE_PREFIX))
    $(info Using LIBS: $(LIBS))
else
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
