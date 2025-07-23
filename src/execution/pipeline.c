/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipeline.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 15:41:49 by rein          #+#    #+#                 */
/*   Updated: 2025/07/23 15:46:51 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_command_pipeline(t_command *commands, t_shell *shell)
{
	t_command_data	data;
	t_command		*cmd;
	int				exit_status;

	if (check_all_commands_empty(commands))
		return (0);
	data.cmd_count = count_commands(commands);
	data.has_failed_redirections = false;
	data.any_command_executed = false;
	data.cmd_index = 0;
	shell->pids = ft_calloc(data.cmd_count, sizeof(pid_t));
	if (!shell->pids)
		return (1);
	exit_status = 0;
	cmd = commands;
	while (cmd)
	{
		exit_status = process_single_command(cmd, shell, commands, &data);
		data.cmd_index++;
		cmd = cmd->next;
	}
	return (finalize_pipeline(shell, commands, &data, exit_status));
}

int	finalize_pipeline(t_shell *shell, t_command *commands,
		t_command_data *data, int exit_status)
{
	close_all_pipes(commands);
	if (!(data->cmd_count == 1 && is_builtin(commands->cmd->str)
			&& !commands->is_piped))
	{
		if (shell->status != 0)
			exit_status = wait_for_children(shell->pids, data->cmd_count);
		else
			wait_for_remain(shell->pids, data->cmd_count);
	}
	free(shell->pids);
	shell->pids = NULL;
	if (data->has_failed_redirections && !data->any_command_executed)
		return (1);
	return (exit_status);
}

void	close_unused_pipes(t_command *commands, t_command *current_cmd)
{
	t_command	*temp;

	temp = commands;
	while (temp)
	{
		if (temp != current_cmd)
		{
			if (temp->in_fd > 2)
				close(temp->in_fd);
			if (temp->out_fd > 2)
				close(temp->out_fd);
		}
		temp = temp->next;
	}
}

void	close_all_pipes(t_command *commands)
{
	t_command	*cmd;

	cmd = commands;
	while (cmd)
	{
		if (cmd->in_fd > 2)
			close(cmd->in_fd);
		if (cmd->out_fd > 2)
			close(cmd->out_fd);
		cmd = cmd->next;
	}
}
