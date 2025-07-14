/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirections.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/27 11:15:53 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/14 15:57:52 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cleanup_redirections(t_command *commands, int saved_fds[][2], int cmd_count)
{
	t_command *cmd = commands;
	int i = 0;

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

bool save_original_fds(t_command *commands, int saved_fds[][2], int *cmd_count)
{
	t_command *cmd = commands;
	*cmd_count = 0;

	while (cmd && *cmd_count < MAX_COMMANDS)
	{
		saved_fds[*cmd_count][0] = cmd->in_fd;
		saved_fds[*cmd_count][1] = cmd->out_fd;
		(*cmd_count)++;
		cmd = cmd->next;
	}
	return true;
}

// Handle input redirection (<)
static bool handle_all_inputs(t_command *cmd)
{
	t_token *current = cmd->input_list;

	while (current)
	{
		if (current->type == INPUT)
		{
			t_token *filename = current->next;
			if (!filename || filename->type != FILENAME)
			{
				cmd->in_fd = -2;
				return true;
			}
			if (cmd->in_fd > 2)
				close(cmd->in_fd);
			cmd->in_fd = open(filename->str, O_RDONLY);
			if (cmd->in_fd < 0)
			{
				ft_dprintf(2, "minishell: %s: %s\n", filename->str, strerror(errno));
				cmd->in_fd = -2;
				return true;
			}
		}
		if (current->next && current->next->type == FILENAME)
			current = current->next->next;
		else
			current = current->next;
	}
	return true;
}

static bool handle_all_outputs(t_command *cmd)
{
	t_token *current = cmd->output_list;
	
	while (current)
	{
		t_token *filename = current->next;
		if (!filename || filename->type != FILENAME)
		{
			cmd->out_fd = -2;
			return true;
		}
		if (cmd->out_fd > 2)
			close(cmd->out_fd);
		int flags;
		if (current->type == OUTPUT)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		else
			flags = O_WRONLY | O_CREAT | O_APPEND;
		cmd->out_fd = open(filename->str, flags, 0644);
		if (cmd->out_fd < 0)
		{
			ft_dprintf(2, "minishell: %s: %s\n", filename->str, strerror(errno));
			cmd->out_fd = -2;
			return true;
		}
		if (current->next && current->next->type == FILENAME)
			current = current->next->next;
		else
			current = current->next;
	}
	return true;
}

static bool handle_heredoc_redirect(t_command *commands, t_shell *shell)
{
	t_command *cmd = commands;
	t_token *delimiter;

	while (cmd)
	{
		if (cmd->input && cmd->input->type == HEREDOC)
		{
			delimiter = cmd->input->next;
			if (!delimiter || delimiter->type != FILENAME)
				return false;
			int fd = handle_heredoc(delimiter->str, shell);
			if (fd == -1)
				return false;
			cmd->in_fd = fd;
		}
		cmd = cmd->next;
	}
	return true;
}

bool process_redirections(t_command *commands, t_shell *shell)
{
	t_command *cmd;
	int saved_fds[MAX_COMMANDS][2];
	int cmd_count;
	int current_cmd;

	if (!save_original_fds(commands, saved_fds, &cmd_count))
		return (false);
	if (!handle_heredoc_redirect(commands, shell))
	{
		cleanup_redirections(commands, saved_fds, cmd_count);
		return false;
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
	return true;
}
