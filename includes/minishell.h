/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 14:54:53 by rbagin        #+#    #+#                 */
/*   Updated: 2025/07/06 11:26:39 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <errno.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_COMMANDS 128
#define MAX_ARGS 64
#define PROMPT "armadillo $ "

extern volatile sig_atomic_t g_signal_received;

typedef enum e_state
{
	IN_HEREDOC,
	IN_CHILD,
	INTERACTIVE
} t_state;

typedef enum e_tokentype
{
	HEREDOC,
	EMPTY,
	CMD,
	ARG,
	OUTPUT,
	APPEND,
	INPUT,
	PIPE,
	END,
	EXPANSION
} t_tokentype;

typedef enum e_fd
{
	STDIN = 0,
	STDOUT = 1,
	STDERR = 2
} t_fd;

typedef enum e_exitstatus
{
	SUCCESS = 0,
	FAILURE = 1,
	ERROR_NULL_POINTER = -1,
	ERROR_INVALID_INPUT = -2,
	ERROR_MEMORY_ALLOCATION = -3
} t_exitstatus;

typedef enum e_quote_state
{
	NONE = 0,
	SINGLE = 1,
	DOUBLE = 2,
} t_quote_state;

//ft_split in tokenizer.c
typedef struct s_split_result
{
    char **tokens;
    t_quote_state *quote_states;
    int count;
} t_split_result;

// Token Linked List
typedef struct s_token
{
	char *str;
	t_tokentype type;
	t_quote_state quote_state;
	struct s_token *next;
	struct s_token *prev;
} t_token;

// Environment Variables
typedef struct s_env
{
	char *key;
	char *value;
	struct s_env *next;
	struct s_env *prev;
} t_env;

// Signals
typedef struct s_sig
{
	int sigint;
	int sigquit;
	t_exitstatus exitstatus;
	pid_t pid;
} t_sig;

// Expansion voor env variables
typedef struct s_expansion
{
	char *arg; // New Argument
	int oi;	   // Old index
	int ni;	   // New index
} t_expansion;

typedef struct s_command
{
	t_token *cmd;
	t_token *args;
	t_token *input;
	t_token *output;
	int in_fd;
	int out_fd;
	bool is_piped;
	struct s_command *next;
} t_command;

typedef struct s_validator
{
	bool valid;
	char *msg;
} t_validator;

// The Shell Struct
typedef struct s_shell
{
	int status;
	t_env *env;
	t_state state;
	t_exitstatus exit_status;
	t_command *commands;
	t_token *tokens;
	pid_t *pids;
} t_shell;

// SIGNALS
void check_signals(t_shell *shell);
void setup_signal_handler(void (*handler)(int));
void set_state(t_shell *shell, t_state state);

// INPUT
t_token *create_token(char *str, t_tokentype type);
void add_token(t_token **tokens, t_token *new);
t_tokentype get_token_type(char *str);
t_token *tokenize(char *input);
int handle_heredoc(char *delimiter, t_shell *shell);
t_split_result	*ft_split_shell(char *input);
void ft_free_array(char **arr);
void free_tokens(t_token *tokens);

// PARSING
t_command *extract_commands(t_token *tokens);
t_command *create_command(void);
void add_to_args(t_command *cmd, t_token *arg_token);
bool process_redirections(t_command *commands, t_shell *shell);
void cleanup_redirections(t_command *commands, int saved_fds[][2], int cmd_count);
bool setup_pipes(t_command *commands);
char **tokens_to_args(t_token *cmd, t_token *args);
bool check_commands(t_command *cmd_head);
void cleanup_redirections(t_command *commands, int saved_fds[][2], int cmd_count);
bool find_command_path(char *cmd, char **env, char *path_buffer);

// EXECUTION
int execute_commands(t_command *commands, t_shell *shell);
int run_command_pipeline(t_command *commands, t_shell *shell);
void close_unused_pipes(t_command *commands, t_command *current_cmd);
void close_all_pipes(t_command *commands);
void setup_command_redirections(t_command *cmd);
int execute_external_command(t_command *cmd, t_env *env_list);
int wait_for_children(pid_t *pids, int count);
void wait_for_remain(pid_t *pids, int count);
char **env_to_array(t_env *env_list);
int count_commands(t_command *commands);
void free_commands(t_command *commands);

// builtin
bool		is_builtin(char *cmd);
int 		exec_builtin(t_command *cmd, t_shell *shell);
int 		exec_exit(t_command *cmd, t_shell *shell);
int			exec_pwd(t_command *cmd, t_shell *shell);
int			exec_cd(t_command *cmd, t_shell *shell);
int			exec_echo(t_command *cmd, t_shell *shell);
int 		exec_env(t_command *cmd, t_shell *shell);
int			exec_export(t_command *cmd, t_shell *shell);
int			exec_unset(t_command *cmd, t_shell *shell);

// env
t_env		*create_env(char **environ);
char		*env_get(t_env *head, const char *key);
void		print_env(t_env *head);
void		free_env(t_env *head);
bool		is_valid_var_syntax(const char *name, bool dollar);
int			update_env_var(t_env **env_head, const char *key, const char *value);
int			remove_env_var(t_env **env_head, const char *key);

// helpers
int count_args(t_token *args);

// free
void free_commands_without_tokens(t_command *commands);
void free_everything(t_shell *shell, bool on_exit);

// for testing
void print_tokens(t_token *tokens);

// expander
t_token *expand_tokens(t_token *token_head, t_env *env_head, t_exitstatus status);

#endif

// echo hello > temp1 > temp2 > temp3
