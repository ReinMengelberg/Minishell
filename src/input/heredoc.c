/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 15:59:44 by rbagin        #+#    #+#                 */
/*   Updated: 2025/07/20 13:38:28 by rmengelb      ########   odam.nl         */
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

static void cleanup_readline_after_interrupt(void)
{
    // Clear readline's internal line buffer
    rl_replace_line("", 0);
    
    // Reset readline to a clean state
    rl_on_new_line();
    
    // Reset readline's "done" flag
    rl_done = 0;
    
    // Force readline to refresh/redisplay
    rl_redisplay();
}

static int fill_heredoc(char *delimiter, int fd, t_shell *shell, int quotes)
{
    char *line;
    
    // Set signal handling for heredoc input
    signal(SIGINT, SIG_DFL);  // Default behavior (terminate)
    signal(SIGQUIT, SIG_IGN); // Ignore SIGQUIT
    
    while (1)
    {
        line = readline("> ");
        if (!line)
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
            // Add your variable expansion function here
            // line = expand_variables(line, shell);
        }
        
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    
    return (0);
}

static int handle_child_process(char *filename, char *delimiter, t_shell *shell, int quotes)
{
    int fd;
    
    fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("open");
        exit(1);
    }
    
    fill_heredoc(delimiter, fd, shell, quotes);
    close(fd);
    exit(0);
}

static int handle_parent_process(pid_t pid, t_shell *shell, char *filename)
{
    int status;
    int fd;

    (void)shell;  // Will be used for variable expansion
    
    // Wait for child process
    if (waitpid(pid, &status, 0) == -1)
    {
        unlink(filename);
        return (-1);
    }
    
    // Check if child was interrupted by signal
    if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);
        unlink(filename);
        
        if (sig == SIGINT)
        {
            shell->exit_status = 130;
            
            // CRITICAL: Clean up readline state after interrupt
            cleanup_readline_after_interrupt();
            
            // Print newline to match bash behavior
            write(STDOUT_FILENO, "\n", 1);
            
            // Set global signal state
            g_signal_received = SIGINT;
        }
        return (-1);
    }
    
    // Check exit status
    if (WIFEXITED(status))
    {
        int exit_code = WEXITSTATUS(status);
        if (exit_code != 0)
        {
            unlink(filename);  // Clean up temp file
            return (-1);
        }
    }
    
    // Open the temp file for reading
    fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror("open");
        unlink(filename);  // Clean up temp file
        return (-1);
    }
    
    return (fd);
}

int handle_heredoc(char *delimiter, t_shell *shell)
{
    pid_t pid;
    char *filename;
    char *clean_delimiter;
    int quotes;
    int result_fd;
    
    if (!delimiter)
        return (-1);
    
    // Generate unique temp filename
    filename = get_heredoc_name();
    if (!filename)
        return (-1);
    
    // Clean delimiter and check for quotes
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
        // Child process - collect input and write to temp file
        handle_child_process(filename, clean_delimiter, shell, quotes);
        // Never returns due to exit() in handle_child_process
    }
    else
    {
        // Parent process - wait for child and open temp file
        result_fd = handle_parent_process(pid, shell, filename);
        
        // Note: Don't unlink the file here if successful!
        // The file needs to persist until the command is executed
        // You should unlink it after the command finishes executing
        
        free(clean_delimiter);
        
        if (result_fd != -1)
        {
            // Store filename in shell structure for later cleanup
            // You might want to add this to your shell/command structure
            // shell->heredoc_files = add_to_list(shell->heredoc_files, filename);
        }
        else
        {
            free(filename);
        }
        
        return (result_fd);
    }
    
    // Should never reach here
    free(filename);
    free(clean_delimiter);
    return (-1);
}
