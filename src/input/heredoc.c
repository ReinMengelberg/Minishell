/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 15:59:44 by rbagin        #+#    #+#                 */
/*   Updated: 2025/07/19 11:34:21 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_delimiter(char *line, char *delimiter, size_t delimiter_len)
{
	if (ft_strncmp(line, delimiter, delimiter_len) == 0
		&& line[delimiter_len] == '\0')
		return (1);
	return (0);
}

static int	handle_readline_input(char *line, int fd)
{
    if (!line)
    {
        close(fd);
        if (g_signal_received == SIGINT)
            exit(130);
        exit(1);
    }
    return (0);
}

static void	write_line_to_fd(int fd, char *line)
{
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

void	collect_heredoc_input(int fd, char *delimiter)
{
    char	*line;
    size_t	delimiter_len;

    delimiter_len = ft_strlen(delimiter);
    g_signal_received = 0;
    while (1)
    {
        line = readline("> ");
        
        // Handle readline returning NULL (EOF or signal)
        if (handle_readline_input(line, fd) != 0)
            break;
        
        if (check_delimiter(line, delimiter, delimiter_len))
        {
            free(line);
            break ;
        }
        write_line_to_fd(fd, line);
        free(line);
    }
    close(fd);
    exit(0);
}

int	handle_heredoc(char *delimiter, t_shell *shell)
{
	pid_t	pid;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return (-1);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (-1);
	}
	if (pid == 0)
		handle_child_process(pipe_fd, delimiter, shell);
	return (handle_parent_process(pipe_fd, pid, shell));
}
