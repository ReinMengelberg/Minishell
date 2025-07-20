/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirections_utils.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 15:19:28 by rein          #+#    #+#                 */
/*   Updated: 2025/07/20 13:13:16 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_redirections(t_command *commands, int saved_fds[][2],
		int cmd_count)
{
	t_command	*cmd;
	int			i;

	cmd = commands;
	i = 0;
	while (cmd && i < cmd_count)
	{
		if (cmd->in_fd > 2)
			close(cmd->in_fd);
		if (cmd->out_fd > 2)
			close(cmd->out_fd);
		cmd->in_fd = saved_fds[i][0];
		cmd->out_fd = saved_fds[i][1];
		cmd = cmd->next;
		i++;
	}
}

bool	save_original_fds(t_command *commands, int saved_fds[][2],
		int *cmd_count)
{
	t_command	*cmd;

	cmd = commands;
	*cmd_count = 0;
	while (cmd && *cmd_count < MAX_COMMANDS)
	{
		saved_fds[*cmd_count][0] = cmd->in_fd;
		saved_fds[*cmd_count][1] = cmd->out_fd;
		(*cmd_count)++;
		cmd = cmd->next;
	}
	return (true);
}

int	open_input_file(t_token *filename, t_command *cmd)
{
	int	fd;

	if (cmd->in_fd > 2)
		close(cmd->in_fd);
	fd = open(filename->str, O_RDONLY);
	if (fd < 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", filename->str,
			strerror(errno));
		cmd->in_fd = -2;
		return (-1);
	}
	cmd->in_fd = fd;
	return (0);
}

int	open_output_file(t_token *filename, t_command *cmd, int flags)
{
	int	fd;

	if (cmd->out_fd > 2)
		close(cmd->out_fd);
	fd = open(filename->str, flags, 0644);
	if (fd < 0)
	{
		ft_dprintf(2, "minishell: %s: %s\n", filename->str,
			strerror(errno));
		cmd->out_fd = -2;
		return (-1);
	}
	cmd->out_fd = fd;
	return (0);
}

bool	handle_heredoc_redirect(t_command *commands, t_shell *shell)
{
	t_command	*cmd;
	t_token		*delimiter;
	int			fd;

	cmd = commands;
	while (cmd)
	{
		if (cmd->input && cmd->input->type == HEREDOC)
		{
			delimiter = cmd->input->next;
			if (!delimiter || delimiter->type != FILENAME)
				return (false);
			fd = handle_heredoc(delimiter->str, shell);
			if (fd == -1)
				return (false);
			cmd->in_fd = fd;
		}
		cmd = cmd->next;
	}
	return (true);
}
