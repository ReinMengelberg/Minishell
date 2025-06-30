/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_new.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 15:59:44 by rbagin        #+#    #+#                 */
/*   Updated: 2025/06/30 13:11:27 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// This function will run in the child process and collect heredoc input
static void	collect_heredoc_input(int fd, char *delimiter)
{
	char	*line;
	size_t	delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	// Need to explicitly reset this at the start of each heredoc
	g_signal_received = 0;
	while (1)
	{
		// Use a proper prompt with readline instead of writing directly
		line = readline("> ");
		// Check for CTRL+D (EOF) first
		if (!line)
		{
			write(STDOUT_FILENO, "\n", 1);
			close(fd);
			exit(1);
		}
		// Check for CTRL+C separately - heredoc handler sets this flag
		if (g_signal_received == SIGINT)
		{
			free(line);
			close(fd);
			exit(130); // Standard SIGINT exit code
		}
		// Check for delimiter match
		if (ft_strncmp(line, delimiter, delimiter_len) == 0 &&
			line[delimiter_len] == '\0')
		{
			free(line);
			break ;
		}
		// Write line to pipe with newline
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	exit(0);
}

// Updated to take shell parameter instead of get_shell_instance()
int	handle_heredoc(char *delimiter, t_shell *shell)
{
	pid_t	pid;
	int		pipe_fd[2];
	int		status;

	sigset_t orig_mask, block_mask;
	// Create pipe for communication
	if (pipe(pipe_fd) == -1)
		return (-1);
	// Block signals in parent
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGINT);
	sigaddset(&block_mask, SIGQUIT);
	sigprocmask(SIG_BLOCK, &block_mask, &orig_mask);
	pid = fork();
	if (pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		sigprocmask(SIG_SETMASK, &orig_mask, NULL);
		return (-1);
	}
	if (pid == 0)
	{
		// Child process
		close(pipe_fd[0]);
		// Unblock signals in child process
		sigprocmask(SIG_SETMASK, &orig_mask, NULL);
		set_state(shell, IN_HEREDOC);
		collect_heredoc_input(pipe_fd[1], delimiter);
	}
	// Parent process
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	// Restore original signal mask
	sigprocmask(SIG_SETMASK, &orig_mask, NULL);
	// Check if heredoc was interrupted
	if (WIFSIGNALED(status))
	{
		close(pipe_fd[0]);
		g_signal_received = WTERMSIG(status);
		shell->exit_status = 130;
		return (-1);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}
