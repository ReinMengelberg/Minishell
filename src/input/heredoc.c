/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 15:59:44 by rbagin        #+#    #+#                 */
/*   Updated: 2025/07/22 10:42:19 by rein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_heredoc_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		unlink(filename);
		return (-1);
	}
	return (fd);
}

int	create_heredoc_process(char *filename, char *clean_delimiter,
		t_shell *shell, int quotes)
{
	pid_t	pid;
	int		status;
	int		fd;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		set_state(shell, shell->state);
		return (-1);
	}
	if (pid == 0)
		setup_heredoc_child(filename, clean_delimiter, shell, quotes);
	if (handle_heredoc_signals(pid, &status, filename, shell) == -1)
		return (-1);
	fd = open_heredoc_file(filename);
	return (fd);
}

int	handle_heredoc(t_token *delimiter, t_shell *shell)
{
	char	*filename;
	int		fd;

	if (!delimiter)
		return (-1);
	filename = get_heredoc_name();
	if (!filename)
		return (-1);
	fd = create_heredoc_process(filename, delimiter->str,
			shell, delimiter->quotestate);
	free(filename);
	return (fd);
}
