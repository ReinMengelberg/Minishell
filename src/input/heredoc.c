/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 15:59:44 by rbagin        #+#    #+#                 */
/*   Updated: 2025/07/20 14:10:37 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define HEREDOC_NAME "/tmp/.minishell_heredoc_"

static int check_delimiter(char *line, char *delimiter)
{
    if (!line || !delimiter)
        return (0);
    return (ft_strcmp(line, delimiter) == 0);
}

static char *get_heredoc_name(void)
{
    static int i = 0;
    char *name;
    char *number;
    
    number = ft_itoa(i);
    if (!number)
        return (NULL);
    name = ft_strjoin(HEREDOC_NAME, number);
    free(number);
    i++;
    return (name);
}

static char *get_delim(char *delim, int *quotes)
{
    int len;
    
    len = ft_strlen(delim) - 1;
    if ((delim[0] == '\"' && delim[len] == '\"')
        || (delim[0] == '\'' && delim[len] == '\''))
    {
        *quotes = 1;
        return (ft_strtrim(delim, "\'\""));
    }
    *quotes = 0;
    return (ft_strdup(delim));
}

static void heredoc_sigint_handler(int sig)
{
    (void)sig;
    exit(130);
}

static int fill_heredoc(char *delimiter, int fd, t_shell *shell, int quotes)
{
    char *line;
    char *expanded_line;
    t_expand_context ctx;
    int i;
    
    // Set up expansion context
    ctx.env = shell->env;
    ctx.status = shell->exit_status;
    
    // Set up custom signal handling for heredoc
    signal(SIGINT, heredoc_sigint_handler);  // Custom handler
    signal(SIGQUIT, SIG_IGN);
    
    while (1)
    {
        line = readline("> ");
        if (!line)  // EOF 
        {
            // EOF (Ctrl+D) reached
            ft_dprintf(2, "minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
            break;
        }
        
        if (check_delimiter(line, delimiter))
        {
            free(line);
            break;
        }
        
        // Handle variable expansion if not quoted delimiter
        if (!quotes && ft_strchr(line, '$'))
        {
            expanded_line = ft_strdup("");
            i = 0;
            while (line[i])
            {
                if (line[i] == '$' && line[i + 1] != '\0')
                    expanded_line = process_dollar_expansion(expanded_line, line, &i, &ctx);
                else
                {
                    char temp_str[2];
                    char *temp;
                    temp_str[0] = line[i];
                    temp_str[1] = '\0';
                    temp = ft_strjoin(expanded_line, temp_str);
                    free(expanded_line);
                    expanded_line = temp;
                    i++;
                }
            }
            write(fd, expanded_line, ft_strlen(expanded_line));
            free(expanded_line);
        }
        else
        {
            write(fd, line, ft_strlen(line));
        }
        
        write(fd, "\n", 1);
        free(line);
    }
    
    return (0);
}

int handle_heredoc(char *delimiter, t_shell *shell)
{
    pid_t pid;
    char *filename;
    char *clean_delimiter;
    int quotes;
    int status;
    int fd;
    
    if (!delimiter)
        return (-1);
    
    filename = get_heredoc_name();
    if (!filename)
        return (-1);
    
    clean_delimiter = get_delim(delimiter, &quotes);
    if (!clean_delimiter)
    {
        free(filename);
        return (-1);
    }
    
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        free(filename);
        free(clean_delimiter);
        return (-1);
    }
    
    if (pid == 0)
    {
        // Child process
        fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd == -1)
            exit(1);
        if (fill_heredoc(clean_delimiter, fd, shell, quotes) == -1)
            exit(130);  // Signal interrupted
        close(fd);
        exit(0);
    }
    else
    {
        // Parent process
        waitpid(pid, &status, 0);
        free(clean_delimiter);
        
        // Check if child was interrupted
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {
            unlink(filename);
            free(filename);
            shell->exit_status = 130;
            return (-1);
        }
        
        // Check if child exited with error
        if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
        {
            unlink(filename);
            free(filename);
            return (-1);
        }
        
        // Open file for reading
        fd = open(filename, O_RDONLY);
        if (fd == -1)
        {
            unlink(filename);
            free(filename);
            return (-1);
        }
        
        // Note: filename memory is leaked here - you should store it for cleanup
        return (fd);
    }
}
