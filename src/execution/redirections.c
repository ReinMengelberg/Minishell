/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirections.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/27 11:15:53 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/20 15:24:20 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	process_heredoc_token(t_command *cmd, t_token *current,
		t_shell *shell)
{
	t_token	*delimiter;
	int		fd;

	delimiter = current->next;
	if (!delimiter || delimiter->type != FILENAME)
	{
		cmd->in_fd = -2;
		return (true);
	}
	if (cmd->in_fd > 2)
		close(cmd->in_fd);
	fd = handle_heredoc(delimiter->str, shell);
	if (fd == -1)
	{
		cmd->in_fd = -2;
		return (true);
	}
	cmd->in_fd = fd;
	return (false);
}

static bool	process_input_token(t_command *cmd, t_token *current)
{
	t_token	*filename;

	if (current->type == INPUT)
	{
		filename = current->next;
		if (!filename || filename->type != FILENAME)
		{
			cmd->in_fd = -2;
			return (true);
		}
		if (open_input_file(filename, cmd) == -1)
			return (true);
	}
	return (false);
}

bool	process_single_input(t_command *cmd, t_token *current,
		t_shell *shell)
{
	if (current->type == INPUT)
		return (process_input_token(cmd, current));
	else if (current->type == HEREDOC)
		return (process_heredoc_token(cmd, current, shell));
	return (false);
}

bool	process_redirections(t_command *commands, t_shell *shell)
{
	t_command	*cmd;
	int			saved_fds[MAX_COMMANDS][2];
	int			cmd_count;
	int			current_cmd;

	if (!save_original_fds(commands, saved_fds, &cmd_count))
		return (false);
	cmd = commands;
	current_cmd = 0;
	while (cmd && current_cmd < cmd_count)
	{
		if (cmd->input_list)
			handle_all_inputs(cmd, shell);
		if (cmd->output_list)
			handle_all_outputs(cmd);
		current_cmd++;
		cmd = cmd->next;
	}
	return (true);
}
