/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 14:54:53 by rbagin        #+#    #+#                 */
/*   Updated: 2025/07/20 15:37:41 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ========================================================================== */
/*                              SYSTEM INCLUDES                              */
/* ========================================================================== */

# ifdef __linux__
#  include <linux/limits.h>
# elif __APPLE__
#  include <sys/syslimits.h>
# endif

# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

/* ========================================================================== */
/*                              LOCAL INCLUDES                               */
/* ========================================================================== */

# include "libft.h"

/* ========================================================================== */
/*                                  DEFINES                                  */
/* ========================================================================== */

# define HEREDOC_NAME "/tmp/.minishell_heredoc_"
# define MAX_COMMANDS 128
# define MAX_ARGS 64
# define PROMPT "armadillo $ "

/* ========================================================================== */
/*                              GLOBAL VARIABLES                             */
/* ========================================================================== */

extern volatile sig_atomic_t	g_signal_received;

/* ========================================================================== */
/*                                   ENUMS                                   */
/* ========================================================================== */

typedef enum e_state
{
	IN_CHILD,
	INTERACTIVE
}								t_state;

typedef enum e_tokentype
{
	HEREDOC,
	EMPTY,
	CMD,
	ARG,
	FILENAME,
	OUTPUT,
	APPEND,
	INPUT,
	PIPE,
	END,
	EXPANSION
}								t_tokentype;

typedef enum e_fd
{
	STDIN = 0,
	STDOUT = 1,
	STDERR = 2
}								t_fd;

typedef enum e_exitstatus
{
	SUCCESS = 0,
	FAILURE = 1,
	ERROR_NULL_POINTER = -1,
	ERROR_INVALID_INPUT = -2,
	ERROR_MEMORY_ALLOCATION = -3
}								t_exitstatus;

typedef enum e_quotestate
{
	NONE = 0,
	SINGLE = 1,
	DOUBLE = 2,
}								t_quotestate;

/* ========================================================================== */
/*                               STRUCTURES                                  */
/* ========================================================================== */

// Token Linked List
typedef struct s_token
{
	char						*str;
	t_tokentype					type;
	t_quotestate				quotestate;
	struct s_token				*next;
	struct s_token				*prev;
}								t_token;

// Environment Variables
typedef struct s_env
{
	char						*key;
	char						*value;
	struct s_env				*next;
	struct s_env				*prev;
}								t_env;

// Signals
typedef struct s_sig
{
	int							sigint;
	int							sigquit;
	t_exitstatus				exitstatus;
	pid_t						pid;
}								t_sig;

// Expansion voor env variables
typedef struct s_expansion
{
	char						*arg;
	int							oi;
	int							ni;
}								t_expansion;

typedef struct s_expand_context
{
	t_env						*env;
	t_exitstatus				status;
}								t_expand_context;

typedef struct s_command
{
	t_token						*cmd;
	t_token						*args;
	t_token						*input;
	t_token						*output;
	t_token						*input_list;
	t_token						*output_list;
	int							in_fd;
	int							out_fd;
	bool						is_piped;
	struct s_command			*next;
}								t_command;

typedef struct s_command_data
{
	int							cmd_count;
	int							cmd_index;
	bool						has_failed_redirections;
	bool						any_command_executed;
}								t_command_data;

typedef struct s_validator
{
	bool						valid;
	char						*msg;
}								t_validator;

// The Shell Struct
typedef struct s_shell
{
	int							status;
	t_env						*env;
	t_state						state;
	t_exitstatus				exit_status;
	t_command					*commands;
	t_token						*tokens;
	pid_t						*pids;
}								t_shell;

/* ========================================================================== */
/*                            FUNCTION DECLARATIONS                          */
/* ========================================================================== */

/* --------------------------------- MAIN ---------------------------------- */

// main/helpers
int								count_args(t_token *args);
void							print_tokens(t_token *tokens);

// main/free
void							free_commands_without_tokens(
									t_command *commands);
void							free_everything(t_shell *shell, bool on_exit);

// main/signals
void							check_signals(t_shell *shell);
void							setup_signal_handler(void (*handler)(int));
void							set_state(t_shell *shell, t_state state);
void							handle_signal_interactive(int sig);
void							handle_signal_child(int sig);

/* --------------------------------- INPUT ---------------------------------- */

// Token management
t_token							*create_token(char *str, t_tokentype type,
									t_quotestate quote);
void							add_token(t_token **tokens, t_token *new);
void							free_tokens(t_token *tokens);
t_token							*tokenize(char *input);
t_token							*create_tokens_from_split(char **strings);

// Token analysis
t_tokentype						get_tokentype(char *str);
t_tokentype						set_tokentype(char *token_str);
t_quotestate					get_quotestate(char *str);
t_quotestate					set_quotestate(char *string);

// String processing
char							**ft_split_shell(char *input);
void							ft_free_array(char **arr);
char							*remove_quotes(char *str);
char							*extract_token(char *input, int *pos);
int								count_tokens(char *input);

// Quote handling
int								handle_quoted_token(char *input, int i,
									char quote);
void							update_quote_status(char input_char,
									char *quote);
void							handle_quote_char(char c, char *quote);

// Input validation
bool							check_input(char *input);
bool							is_unsupported_char(char c);
bool							is_unsupported_syntax(char *input, int i,
									char quote);
bool							is_unsupported_expansion(char *input, int i,
									char quote);
bool							validate_pipe_syntax(t_token *tokens);

// Token type processing
t_tokentype						process_token_type(t_tokentype type,
									bool *cmd_found, bool *next_redir_target,
									bool *after_redir_file);
void							reset_token_state(bool *cmd_found,
									bool *next_arg, bool *after_file);
t_tokentype						handle_cmd_arg_token(t_tokentype type,
									bool *cmd_found, bool *after_redir_file);

// Command extraction
t_command						*extract_commands(t_token *tokens);
t_command						*create_new_command_node(t_command **cmd_head,
									t_command **current_cmd);
t_command						*create_command(void);
void							add_to_args(t_command *cmd, t_token *arg_token);
void							add_to_input_list(t_command *cmd,
									t_token *token);
void							add_to_output_list(t_command *cmd,
									t_token *token);
char							**tokens_to_args(t_token *cmd, t_token *args);
bool							check_commands(t_command *cmd_head);

// Variable expansion
t_token							*expand_tokens(t_token *token_head,
									t_env *env_head, t_exitstatus status);
char							*expand_variable(char *str, int *i,
									t_expand_context *ctx);
int								count_tokens(char *input);
char							*extract_token(char *input, int *pos);
int								handle_quoted_token(char *input, int i,
									char quote);
bool							check_input(char *input);
t_token							*create_tokens_from_split(char **strings);
char							*remove_quotes(char *str);
t_tokentype						set_tokentype(char *token_str);
bool							validate_expansion_in_token(t_token *token);
t_quotestate					set_quotestate(char *string);
bool							validate_pipe_syntax(t_token *tokens);
bool							is_expansion(char *str);
bool							is_unsupported_char(char c);
bool							is_unsupported_syntax(char *input, int i,
									char quote);
bool							is_unsupported_expansion(char *input, int i,
									char quote);
void							update_quote_status(char input_char,
									char *quote);
void							handle_quote_char(char c, char *quote);
t_command						*create_command(void);
void							collect_heredoc_input(int fd, char *delimiter);
t_token							*expand_token(t_token *token, t_env *env,
									t_exitstatus status);
char							*expand_variable(char *str, int *i,
									t_expand_context *ctx);
bool							valid_expansions(t_token *token_head);
bool							validate_expansion_in_token(t_token *token);
bool							validate_variable_name(char *str, int *i);
bool							should_expand_variable(t_token *token);
bool							is_expansion(char *str);
char							*handle_exit_status(char *result,
									t_exitstatus status);
char							*handle_variable_expansion(char *result,
									char *original, int *i, t_env *env);
char							*process_dollar_expansion(char *result,
									char *original, int *i,
									t_expand_context *ctx);

// Heredoc handling
int								handle_heredoc(char *delimiter, t_shell *shell);
char							*get_heredoc_name(void);
char							*get_delim(char *delim, int *quotes);
void							heredoc_sigint_handler(int sig);
char							*expand_heredoc_line(char *line,
									t_expand_context *ctx);
void							write_heredoc_line(char *line, int fd,
									t_expand_context *ctx, int quotes);
int								read_heredoc_input(char *delimiter, int fd,
									t_shell *shell, int quotes);
int								fill_heredoc(char *delimiter, int fd,
									t_shell *shell, int quotes);
int								setup_heredoc_child(char *filename,
									char *clean_delimiter, t_shell *shell,
									int quotes);
int								handle_heredoc_signals(pid_t pid, int *status,
									char *filename, t_shell *shell);
int								open_heredoc_file(char *filename);
int								create_heredoc_process(char *filename,
									char *clean_delimiter, t_shell *shell,
									int quotes);
int								check_delimiter(char *line, char *delimiter);

/* -------------------------------- EXECUTION ------------------------------- */

// Main execution functions
int								execute_commands(t_command *commands,
									t_shell *shell);
int								run_command_pipeline(t_command *commands,
									t_shell *shell);
void							free_commands(t_command *commands);
int								count_commands(t_command *commands);
char							**env_to_array(t_env *env_list);

// Process management functions
int								wait_for_children(pid_t *pids, int count);
void							wait_for_remain(pid_t *pids, int count);
int								finalize_pipeline(t_shell *shell,
									t_command *commands, t_command_data *data,
									int exit_status);

// Command execution functions
bool							check_all_commands_empty(t_command *commands);
int								process_single_command(t_command *cmd,
									t_shell *shell, t_command *commands,
									t_command_data *data);
int								execute_external_command(t_command *cmd,
									t_env *env_list);
void							setup_command_redirections(t_command *cmd);
bool							find_command_path(char *cmd, char **env,
									char *path_buffer);

// Pipe and file descriptor management
void							close_unused_pipes(t_command *commands,
									t_command *current_cmd);
void							close_all_pipes(t_command *commands);
bool							setup_pipes(t_command *commands);

// Redirections
bool							process_redirections(t_command *commands,
									t_shell *shell);
void							cleanup_redirections(t_command *commands,
									int saved_fds[][2], int cmd_count);
bool							save_original_fds(t_command *commands,
									int saved_fds[][2], int *cmd_count);
int								open_input_file(t_token *filename,
									t_command *cmd);
int								open_output_file(t_token *filename,
									t_command *cmd, int flags);
bool							handle_all_inputs(t_command *cmd,
									t_shell *shell);
bool							process_single_input(t_command *cmd,
									t_token *current, t_shell *shell);
bool							handle_all_outputs(t_command *cmd);

// execution/builtin
bool							is_builtin(char *cmd);
int								exec_builtin(t_command *cmd, t_shell *shell);
int								exec_exit(t_command *cmd, t_shell *shell);
int								exec_pwd(t_command *cmd, t_shell *shell);
int								exec_cd(t_command *cmd, t_shell *shell);
int								exec_echo(t_command *cmd, t_shell *shell);
int								exec_env(t_command *cmd, t_shell *shell);
int								exec_export(t_command *cmd, t_shell *shell);
int								exec_unset(t_command *cmd, t_shell *shell);

/* ----------------------------- ENVIRONMENT ------------------------------- */

t_env							*create_env(char **environ);
char							*env_get(t_env *head, const char *key);
void							print_env(t_env *head);
void							free_env(t_env *head);
bool							is_valid_var_syntax(const char *name,
									bool dollar);
int								update_env_var(t_env **env_head,
									const char *key, const char *value);
int								remove_env_var(t_env **env_head,
									const char *key);

#endif
