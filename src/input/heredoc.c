/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 15:59:44 by rbagin        #+#    #+#                 */
/*   Updated: 2025/06/08 13:09:18 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Global flag for heredoc signal handling
volatile sig_atomic_t g_heredoc_signal = 0;

// Special signal handler for heredoc mode
static void heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_heredoc_signal = 1;
		write(STDERR_FILENO, "\n", 1);
		close(STDIN_FILENO);
	}
}

int handle_heredoc(char *delimiter)
{
	int pipe_fd[2];
	char *line;
	size_t delimiter_len;
	struct sigaction sa_old;
	struct sigaction sa_new;

	printf("Opening heredoc for delimiter: %s\n", delimiter);
	// Create pipe
	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}

	// Set up special signal handling for heredoc
	memset(&sa_new, 0, sizeof(sa_new));
	sa_new.sa_handler = heredoc_signal_handler;
	sigaction(SIGINT, &sa_new, &sa_old);

	g_heredoc_signal = 0;
	delimiter_len = ft_strlen(delimiter);

	// Read input until delimiter is found
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = readline("");

		// Check for signals or EOF
		if (g_heredoc_signal || !line)
		{
			if (line)
				free(line);
			close(pipe_fd[1]);
			close(pipe_fd[0]);
			sigaction(SIGINT, &sa_old, NULL);
			return (-1);
		}

		// Check for delimiter match
		if (ft_strncmp(line, delimiter, delimiter_len) == 0 &&
			line[delimiter_len] == '\0')
		{
			free(line);
			break;
		}

		// Write input to pipe
		ft_putendl_fd(line, pipe_fd[1]);
		free(line);
	}

	// Restore signal handling
	sigaction(SIGINT, &sa_old, NULL);

	// Close write end, return read end
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

// Process all heredocs in a command chain
bool process_heredocs(t_command *commands, t_shell *shell)
{
	t_command *cmd;
	t_token *token;
	int heredoc_fd;
	t_signalstate prev_state;

	prev_state = shell->sig_state;
	shell->sig_state = IN_HEREDOC;

	cmd = commands;
	while (cmd)
	{
		printf("Processing command: %s\n", cmd->cmd ? cmd->cmd->str : "NULL");
		token = cmd->input;
		while (token)
		{
			printf("  Checking token type %d: %s\n", token->type, token->str);
			if (token->type == HEREDOC && token->next)
			{
				printf("  Found heredoc with delimiter: %s\n", token->next->str);
				// Get delimiter from next token
				heredoc_fd = handle_heredoc(token->next->str);
				if (heredoc_fd == -1)
				{
					// Restore signal state before returning
					shell->sig_state = prev_state;
					return false;
				}

				// Store the heredoc fd as the command's input
				if (cmd->in_fd > 2)
					close(cmd->in_fd);
				cmd->in_fd = heredoc_fd;

				// Skip the delimiter token
				token = token->next;
			}
			if (token)
				token = token->next;
		}
		cmd = cmd->next;
	}

	// Restore signal state
	shell->sig_state = prev_state;
	return true;
}
