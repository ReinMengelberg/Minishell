/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 15:59:44 by rbagin        #+#    #+#                 */
/*   Updated: 2025/07/20 12:11:46 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int check_delimiter(char *line, char *delimiter)
{
    if (!line || !delimiter)
        return (0);
    return (ft_strcmp(line, delimiter) == 0);
}

void collect_heredoc_input(int fd, char *delimiter)
{
    char *line;
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_IGN);
    
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            ft_dprintf(2, "minishell: warning: here-document delimited by end-of-file (wanted `%s')\n", delimiter);
            break;
        }
        if (check_delimiter(line, delimiter))
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    close(fd);
    exit(0);
}

static int handle_child_process(int *pipe_fd, char *delimiter, t_shell *shell)
{
    (void)shell;  // Not needed in child
    
    close(pipe_fd[0]);  // Close read end
    collect_heredoc_input(pipe_fd[1], delimiter);
    return (0);  // Never reached due to exit() in collect_heredoc_input
}

static int handle_parent_process(int *pipe_fd, pid_t pid, t_shell *shell)
{
    int status;
    
    close(pipe_fd[1]);  // Close write end
    
    // Wait for child process
    if (waitpid(pid, &status, 0) == -1)
    {
        close(pipe_fd[0]);
        return (-1);
    }
    
    // Check if child was interrupted by signal
    if (WIFSIGNALED(status))
    {
        int sig = WTERMSIG(status);
        close(pipe_fd[0]);
        if (sig == SIGINT)
        {
            shell->exit_status = 130;
            // Don't set g_signal_received here - let the main loop handle signals normally
        }
        return (-1);
    }
    
    // Check exit status
    if (WIFEXITED(status))
    {
        int exit_code = WEXITSTATUS(status);
        if (exit_code != 0)
        {
            close(pipe_fd[0]);
            return (-1);
        }
    }
    
    return (pipe_fd[0]);  // Return read end of pipe
}

int handle_heredoc(char *delimiter, t_shell *shell)
{
    pid_t pid;
    int pipe_fd[2];
    
    if (!delimiter)
        return (-1);
        
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return (-1);
    }
    
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return (-1);
    }
    
    if (pid == 0)
        handle_child_process(pipe_fd, delimiter, shell);
    else
        return (handle_parent_process(pipe_fd, pid, shell));
    
    return (-1);  // Should never reach here
}
