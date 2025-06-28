/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 15:59:44 by rbagin        #+#    #+#                 */
<<<<<<< HEAD
/*   Updated: 2025/06/28 11:12:57 by rmengelb      ########   odam.nl         */
=======
/*   Updated: 2025/06/24 12:17:55 by rbagin        ########   odam.nl         */
>>>>>>> origin/main
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Global flag for heredoc signal handling
volatile sig_atomic_t	g_heredoc_signal = 0;

// Special signal handler for heredoc mode
static void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_heredoc_signal = 1;
		write(STDERR_FILENO, "\n", 1);
		close(STDIN_FILENO);
	}
}

static int	setup_heredoc_pipe(struct sigaction *sa_new,
								struct sigaction *sa_old)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (-1);
	}
	memset(sa_new, 0, sizeof(*sa_new));
	sa_new->sa_handler = heredoc_signal_handler;
	sigaction(SIGINT, sa_new, sa_old);
	g_heredoc_signal = 0;
	return (0);
}

static int	read_heredoc_line(int *pipe_fd, char *delimiter,
								size_t delimiter_len, struct sigaction *sa_old)
{
	char	*line;

	write(STDOUT_FILENO, "> ", 2);
	line = readline("");
	if (g_heredoc_signal || !line)
	{
		if (line)
			free(line);
		close(pipe_fd[1]);
		close(pipe_fd[0]);
		sigaction(SIGINT, sa_old, NULL);
		return (-1);
	}
	if (ft_strncmp(line, delimiter, delimiter_len) == 0
		&& line[delimiter_len] == '\0')
	{
		free(line);
		return (1);
	}
	ft_putendl_fd(line, pipe_fd[1]);
	free(line);
	return (0);
}

int	handle_heredoc(char *delimiter)
{
	int					pipe_fd[2];
	struct sigaction	sa_old;
	struct sigaction	sa_new;
	size_t				delimiter_len;
	int					result;

	if (pipe(pipe_fd) == -1)
		return (-1);
	if (setup_heredoc_pipe(&sa_new, &sa_old) == -1)
		return (-1);
	delimiter_len = ft_strlen(delimiter);
	while (1)
	{
		result = read_heredoc_line(pipe_fd, delimiter, delimiter_len, &sa_old);
		if (result == -1)
			return (-1);
		if (result == 1)
			break ;
	}
	sigaction(SIGINT, &sa_old, NULL);
	close(pipe_fd[1]);
	return (pipe_fd[0]);
}

static bool	process_heredoc_token(t_token *token, t_command *cmd,
								t_shell *shell, t_state *prev_state)
{
	int	heredoc_fd;

	if (token->type == HEREDOC && token->next)
	{
		heredoc_fd = handle_heredoc(token->next->str);
		if (heredoc_fd == -1)
		{
			shell->state = *prev_state;
			return (false);
		}
		if (cmd->in_fd > 2)
			close(cmd->in_fd);
		cmd->in_fd = heredoc_fd;
		return (true);
	}
	return (false);
}

static bool	process_command_tokens(t_command *cmd, t_shell *shell,
									t_state *prev_state)
{
	t_token	*token;
	bool	skip_token;

	token = cmd->input;
	while (token)
	{
		skip_token = false;
		if (token->type == HEREDOC && token->next)
		{
			if (!process_heredoc_token(token, cmd, shell, prev_state))
				return (false);
			token = token->next;
			skip_token = true;
		}
		if (token && !skip_token)
			token = token->next;
	}
	return (true);
}

bool	process_heredocs(t_command *commands, t_shell *shell)
{
	t_command	*cmd;
	t_state	prev_state;

	prev_state = shell->state;
	shell->state = IN_HEREDOC;
	cmd = commands;
	while (cmd)
	{
		if (!process_command_tokens(cmd, shell, &prev_state))
			return (false);
		cmd = cmd->next;
	}
	shell->state = prev_state;
	return (true);
}
