/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 14:54:53 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/27 11:20:16 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <linux/limits.h>
# include <errno.h>
# include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_COMMANDS 128
typedef enum e_tokentype
{
	EMPTY,
	CMD,
	ARG,
	OUTPUT,
	APPEND,
	INPUT,
	PIPE,
	HEREDOC,
	END,
	EXPANSION
}	t_tokentype;

typedef enum e_fd
{
	STDIN = 0,
	STDOUT = 1,
	STDERR = 2
}	t_fd;

typedef enum e_exitstatus
{
	SUCCESS = 0,
	FAILURE = 1
}	t_exitstatus;

// Token Linked List
typedef struct s_token
{
	char			*str;
	t_tokentype		type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

// Environment Variables
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
	struct s_env	*prev;
}	t_env;

// Signals
typedef struct s_sig
{
	int				sigint;
	int				sigquit;
	t_exitstatus	exitstatus;
	pid_t			pid;
}	t_sig;

// Expansion voor env variables
typedef struct s_expansion
{
	char	*arg; // New Argument
	int		oi; // Old index
	int		ni; // New index
}	t_expansion;

typedef struct s_command {
	t_token				*cmd;
	t_token				*args;
	t_token				*input;
	t_token				*output;
	int					in_fd;
	int					out_fd;
	bool				is_piped;
	struct s_command	*next;
} t_command;

// The Shell Struct
typedef struct s_shell
{
	t_command	*command_head;
	t_token		*token_head;
	t_env		*env;
}	t_shell;

//INPUT
//tokenizer.c
t_token	*create_token(char *str, t_tokentype type);
void	add_token(t_token **tokens, t_token *new);
t_tokentype	get_token_type(char *str);
t_token	*tokenize(char *input);
//token_utils.c
char	**ft_split_shell(char *input);
void	ft_free_array(char **arr);
void	free_tokens(t_token *tokens);
//parser.c
t_command	*extract_commands(t_token *tokens);
t_command *create_command(void);
void add_to_args(t_command *cmd, t_token *arg_token);
bool	process_redirections(t_command *commands);
void cleanup_redirections(t_command *commands, int saved_fds[][2], int cmd_count);
bool	setup_pipes(t_command *commands);
char **tokens_to_args(t_token *cmd, t_token *args);
//redirections.c
bool process_redirections(t_command *commands);
void cleanup_redirections(t_command *commands, int saved_fds[][2], int cmd_count);
//path.c
bool find_command_path(char *cmd, char **env, char *path_buffer);
//execution.c
int execute_commands(t_command *commands, t_shell *shell);
int run_command_pipeline(t_command *commands, t_env *env_list);
void setup_command_redirections(t_command *cmd);
void execute_external_command(t_command *cmd, t_env *env_list);
void cleanup_commands(t_command *commands);
int wait_for_children(pid_t *pids, int count);
int count_commands(t_command *commands);
bool is_builtin(char *cmd);
char **env_to_array(t_env *env_list);
void free_commands(t_command *commands);
// env
t_env	*create_env(char **environ);
void	print_env(t_env *head);
void	free_env(t_env *head);

//for testing
void print_tokens(t_token *tokens);
#endif


// echo hello > temp1 > temp2 > temp3
