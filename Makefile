CC = cc
CFLAGS = -Wall -Werror -Wextra -g3
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
		./src/input/token_utils.c \
		./src/input/tokenizer.c \
		./src/input/expander.c \
		./src/input/parser.c \
		./src/execution/execution.c \
		./src/execution/path.c \
		./src/execution/redirections.c \
		./src/env/env_init.c \
		./src/env/env_service.c \
		./src/main/main.c

OBJ_DIR = objs
OBJS = $(patsubst ./src/%.c,$(OBJ_DIR)/%.o,$(SRCS))

TEST_SRCS = ./src/testing/test_tokenizer_to_cmd.c \
			./src/input/tokenizer.c \
			./src/input/token_utils.c \
			./src/input/parser.c \
			./src/execution/redirections.c \
			./src/env/env_init.c

TEST_OBJS = $(patsubst ./src/%.c,$(OBJ_DIR)/%.o,$(TEST_SRCS))

INTEGRATION_SRCS =	./src/input/tokenizer.c \
					./src/input/token_utils.c \
					./src/input/parser.c \
					./src/execution/execution.c \
					./src/execution/redirections.c \
					./src/execution/path.c \
					./src/env/env_init.c \
					./src/env/env_service.c \
					./src/testing/test_integration.c \

INTEGRATION_OBJS = $(patsubst ./src/%.c,$(OBJ_DIR)/%.o,$(INTEGRATION_SRCS))

# Configure readline library based on OS
ifeq ($(shell uname), Darwin)
	BREW_PREFIX = $(shell brew --prefix 2>/dev/null || echo "/usr/local")
	INCLUDES += -I$(BREW_PREFIX)/include
	LIBS += -L$(BREW_PREFIX)/lib -lreadline
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

test: $(OBJ_DIR) $(TEST_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(TEST_OBJS) $(LIBFT) $(INCLUDES) $(LIBS) -o tokenizer_test
	@echo "Token test program built. Run with ./tokenizer_test"

integration_test: $(OBJ_DIR) $(INTEGRATION_OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(INTEGRATION_OBJS) $(LIBFT) $(INCLUDES) $(LIBS) -o integration_test
	@echo "Integration test program built. Run with ./integration_test"

clean:
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@echo "Clean complete"

re: fclean all

.PHONY: all clean fclean re
