/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   minishell.h                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 14:54:53 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/18 13:12:14 by ravi-bagin    ########   odam.nl         */
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
# include <limits.h>
# include <errno.h>
# include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

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

// The Shell Struct
typedef struct s_shell
{
	t_token	*tokens;
	t_env	*env;
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

// env
t_env	*create_env(char **environ);
void	print_env(t_env *head);

//for testing
void print_tokens(t_token *tokens);
#endif
