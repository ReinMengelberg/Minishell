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

enum e_tokentype {
    EMPTY;
    CMD;
    ARG;
    TRUNC;
    APPEND;
    INPUT;
    PIPE;
    END;
};

enum e_fd {
    STDIN = 0;
    STDOUT = 1;
    STDERR = 2;
}

enum e_exitstatus {
    SUCCESS = 0;
    FAILURE = 1;
}

// Token Linked List
typedef struct s_token {
    char	        *str;
    e_tokentype	    type;
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
    int          sigint;
    int          sigquit;
    e_exitstatus exitstatus;
    pid_t        pid;
} t_sig;

// Expansion voor env variables
typedef struct t_expansion {
    char    *arg; // New Argument
    int     oi; // Old index
    int     ni; // New index
} t_expansion;

// The Shell Struct
typedef struct t_shell {
    t_token    *tokens;
    t_env      *env
} s_shell;

#endif