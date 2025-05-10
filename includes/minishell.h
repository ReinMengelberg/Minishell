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


// Filedescriptor definitions
# define STDIN 0
# define STDOUT 1
# define STDERR 2

typedef enum t_tokentype {
    EMPTY;
    CMD;
    ARG;
    TRUNC;
    APPEND;
    INPUT;
    PIPE;
    END;
};

// Token Linked List
typedef struct s_token {
    char	        *str;
    t_tokentype	    type;
    struct s_token	*next;
    struct s_token	*prev;
} t_token

// Environment Variables
typedef struct s_env {
    char	*key;
    char	*value;
} t_env

// Signals
typedef struct s_sig {
    int      sigint;
    int      sigquit;
    pid_t    pid;
} t_sig;

typedef struct t_shell {
    t_token    *tokens;
    t_env      *env
} s_shell;


#endif