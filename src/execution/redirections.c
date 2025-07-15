/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirections.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/27 11:15:53 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/15 15:28:50 by rein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static bool	process_output_token(t_command *cmd, t_token *current)
{
	t_token	*filename;
	int		flags;

	filename = current->next;
	if (!filename || filename->type != FILENAME)
	{
		cmd->out_fd = -2;
		return (true);
	}
	if (current->type == OUTPUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	if (open_output_file(filename, cmd, flags) == -1)
		return (true);
	return (false);
}

static bool	handle_all_inputs(t_command *cmd)
{
	t_token	*current;

	current = cmd->input_list;
	while (current)
	{
		if (process_input_token(cmd, current))
			return (true);
		if (current->next && current->next->type == FILENAME)
			current = current->next->next;
		else
			current = current->next;
	}
	return (true);
}

static bool	handle_all_outputs(t_command *cmd)
{
	t_token	*current;

	current = cmd->output_list;
	while (current)
	{
		if (process_output_token(cmd, current))
			return (true);
		if (current->next && current->next->type == FILENAME)
			current = current->next->next;
		else
			current = current->next;
	}
	return (true);
}

bool	process_redirections(t_command *commands, t_shell *shell)
{
	t_command	*cmd;
	int			saved_fds[MAX_COMMANDS][2];
	int			cmd_count;
	int			current_cmd;

	if (!save_original_fds(commands, saved_fds, &cmd_count))
		return (false);
	if (!handle_heredoc_redirect(commands, shell))
	{
		cleanup_redirections(commands, saved_fds, cmd_count);
		return (false);
	}
	cmd = commands;
	current_cmd = 0;
	while (cmd && current_cmd < cmd_count)
	{
		if (cmd->input_list && !(cmd->input && cmd->input->type == HEREDOC))
			handle_all_inputs(cmd);
		if (cmd->output_list)
			handle_all_outputs(cmd);
		current_cmd++;
		cmd = cmd->next;
	}
	return (true);
}
