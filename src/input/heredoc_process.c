/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_process.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/20 15:14:41 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/20 15:18:58 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	write_heredoc_line(char *line, int fd, t_expand_context *ctx,
		int quotes)
{
	char	*expanded_line;

	if (!quotes)
	{
		expanded_line = expand_heredoc_line(line, ctx);
		write(fd, expanded_line, ft_strlen(expanded_line));
		free(expanded_line);
	}
	else
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}

int	read_heredoc_input(char *delimiter, int fd, t_shell *shell, int quotes)
{
	char				*line;
	t_expand_context	ctx;

	ctx.env = shell->env;
	ctx.status = shell->exit_status;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_dprintf(2,
				"minishell: warning: here-document delimited by "
				"end-of-file (wanted `%s')\n",
				delimiter);
			break ;
		}
		if (check_delimiter(line, delimiter))
		{
			free(line);
			break ;
		}
		write_heredoc_line(line, fd, &ctx, quotes);
		free(line);
	}
	return (0);
}

int	fill_heredoc(char *delimiter, int fd, t_shell *shell, int quotes)
{
	signal(SIGINT, heredoc_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	return (read_heredoc_input(delimiter, fd, shell, quotes));
}

int	setup_heredoc_child(char *filename, char *clean_delimiter, t_shell *shell,
		int quotes)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		exit(1);
	if (fill_heredoc(clean_delimiter, fd, shell, quotes) == -1)
		exit(130);
	close(fd);
	exit(0);
}

int	handle_heredoc_signals(pid_t pid, int *status, char *filename,
		t_shell *shell)
{
	waitpid(pid, status, 0);
	set_state(shell, shell->state);
	if (WIFSIGNALED(*status) && WTERMSIG(*status) == SIGINT)
	{
		unlink(filename);
		shell->exit_status = 130;
		return (-1);
	}
	if (WIFEXITED(*status) && WEXITSTATUS(*status) == 130)
	{
		unlink(filename);
		shell->exit_status = 130;
		return (-1);
	}
	if (WIFEXITED(*status) && WEXITSTATUS(*status) != 0)
	{
		unlink(filename);
		return (-1);
	}
	return (0);
}
