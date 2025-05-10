#ifndef MINISHELL_H
# define MINISHELL_H

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

typedef enum t_filedescriptor {
    STDIN;
    STDOUT;
    STDERR;
}

typedef enum

tydef struct s_token {
    char	        *str;
    int             type;
    t_tokentype	    type;
    struct s_token	*next;
    struct s_token	*prev;
} t_token

#endif