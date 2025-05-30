/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirections.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/27 11:15:53 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/05/27 12:42:15 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void cleanup_redirections(t_command *commands, int saved_fds[][2], int cmd_count)
{
	t_command *cmd = commands;
	int i = 0;

	while (cmd && i < cmd_count)
	{
		// Close any new file descriptors that were opened
		if (cmd->in_fd != saved_fds[i][0] && cmd->in_fd > 2)
		{
			close(cmd->in_fd);
			cmd->in_fd = saved_fds[i][0];
		}
		if (cmd->out_fd != saved_fds[i][1] && cmd->out_fd > 2)
		{
			close(cmd->out_fd);
			cmd->out_fd = saved_fds[i][1];
		}
		cmd = cmd->next;
		i++;
	}
}

static bool save_original_fds(t_command *commands, int saved_fds[][2], int *cmd_count)
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
static bool handle_input_redirection(t_command *cmd, t_command *commands,
									int saved_fds[][2], int current_cmd)
{
	t_token *filename = cmd->input->next;

	if (!filename)
	{
		cleanup_redirections(commands, saved_fds, current_cmd);
		return false;
	}
	cmd->in_fd = open(filename->str, O_RDONLY);
	if (cmd->in_fd < 0)
	{
		perror("INPUT open error");
		cleanup_redirections(commands, saved_fds, current_cmd);
		return false;
	}
	return true;
}

// Handle heredoc (<<) - placeholder for now
static bool handle_heredoc(t_command *cmd)
{
	(void)cmd; // Prevent unused parameter warning
	ft_dprintf(2, "HEREDOC has not been implemented yet\n");
	return true;
}

// Handle output redirection (>)
static bool handle_output_redirection(t_command *cmd, t_command *commands,
									int saved_fds[][2], int current_cmd)
{
	t_token *filename = cmd->output->next;

	if (!filename)
	{
		cleanup_redirections(commands, saved_fds, current_cmd);
		return false;
	}
	cmd->out_fd = open(filename->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->out_fd < 0)
	{
		perror("OUTPUT open error");
		cleanup_redirections(commands, saved_fds, current_cmd);
		return false;
	}
	return true;
}

// Handle append redirection (>>)
static bool handle_append_redirection(t_command *cmd, t_command *commands,
									int saved_fds[][2], int current_cmd)
{
	t_token *filename = cmd->output->next;

	if (!filename)
	{
		cleanup_redirections(commands, saved_fds, current_cmd);
		return false;
	}
	cmd->out_fd = open(filename->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->out_fd < 0)
	{
		perror("APPEND open error");
		cleanup_redirections(commands, saved_fds, current_cmd);
		return false;
	}
	return true;
}

bool process_redirections(t_command *commands)
{
	t_command *cmd;
	int saved_fds[MAX_COMMANDS][2];
	int cmd_count;
	int current_cmd;

	if (!save_original_fds(commands, saved_fds, &cmd_count))
		return false;
	cmd = commands;
	current_cmd = 0;
	while (cmd && current_cmd < cmd_count)
	{
		if (cmd->input)
		{
			if (cmd->input->type == INPUT)
			{
				if (!handle_input_redirection(cmd, commands, saved_fds, current_cmd))
					return false;
			}
			else if (cmd->input->type == HEREDOC)
			{
				if (!handle_heredoc(cmd))
					return false;
			}
		}
		if (cmd->output)
		{
			if (cmd->output->type == OUTPUT)
			{
				if (!handle_output_redirection(cmd, commands, saved_fds, current_cmd))
					return false;
			}
			else if (cmd->output->type == APPEND)
			{
				if (!handle_append_redirection(cmd, commands, saved_fds, current_cmd))
					return false;
			}
		}
		current_cmd++;
		cmd = cmd->next;
	}
	return true;
}
