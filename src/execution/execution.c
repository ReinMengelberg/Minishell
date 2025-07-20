/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/21 13:24:51 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/20 10:20:23 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_commands(t_command *commands, t_shell *shell)
{
	int	saved_fds[MAX_COMMANDS][2];
	int	command_count;

	if (!commands)
		return (1);
	if (!check_commands(commands))
		return (free_everything(shell, false), 1);
	command_count = count_commands(commands);
	save_original_fds(commands, saved_fds, &command_count);
	process_redirections(commands, shell);
	if (!setup_pipes(commands))
	{
		cleanup_redirections(commands, saved_fds, command_count);
		return (free_everything(shell, false), 1);
	}
	if (shell->pids)
	{
		free(shell->pids);
		shell->pids = NULL;
	}
	shell->exit_status = run_command_pipeline(commands, shell);
	cleanup_redirections(commands, saved_fds, command_count);
	return (shell->exit_status);
}

bool	check_all_commands_empty(t_command *commands)
{
	t_command	*temp_cmd;

	temp_cmd = commands;
	while (temp_cmd)
	{
		if (temp_cmd->cmd && temp_cmd->cmd->str
			&& temp_cmd->cmd->str[0] != '\0')
			return (false);
		temp_cmd = temp_cmd->next;
	}
	return (true);
}

static int	execute_single_builtin(t_command *cmd, t_shell *shell)
{
	int	stdin_save;
	int	stdout_save;
	int	exit_status;

	if (cmd->in_fd == -2 || cmd->out_fd == -2)
		return (1);
	stdin_save = dup(STDIN_FILENO);
	stdout_save = dup(STDOUT_FILENO);
	if (cmd->in_fd != STDIN_FILENO && cmd->in_fd >= 0)
		dup2(cmd->in_fd, STDIN_FILENO);
	if (cmd->out_fd != STDOUT_FILENO && cmd->out_fd >= 0)
		dup2(cmd->out_fd, STDOUT_FILENO);
	exit_status = exec_builtin(cmd, shell);
	dup2(stdin_save, STDIN_FILENO);
	dup2(stdout_save, STDOUT_FILENO);
	close(stdin_save);
	close(stdout_save);
	return (exit_status);
}

static int	fork_and_execute(t_command *cmd, t_shell *shell,
		t_command *commands, int cmd_index)
{
	shell->pids[cmd_index] = fork();
	if (shell->pids[cmd_index] == 0)
	{
		close_unused_pipes(commands, cmd);
		setup_command_redirections(cmd);
		set_state(shell, IN_CHILD);
		if (is_builtin(cmd->cmd->str))
			exit(exec_builtin(cmd, shell));
		else
		{
			execute_external_command(cmd, shell->env);
			exit(127);
		}
	}
	else if (shell->pids[cmd_index] < 0)
	{
		perror("fork failed");
		if (shell->status != 0)
			return (1);
	}
	return (0);
}

int	process_single_command(t_command *cmd, t_shell *shell,
		t_command *commands, t_command_data *data)
{
	if (cmd->in_fd == -2 || cmd->out_fd == -2)
	{
		data->has_failed_redirections = true;
		return (0);
	}
	if (!cmd->cmd || !cmd->cmd->str || cmd->cmd->str[0] == '\0')
		return (0);
	data->any_command_executed = true;
	if (is_builtin(cmd->cmd->str) && data->cmd_count == 1 && !cmd->is_piped)
		return (execute_single_builtin(cmd, shell));
	else
		return (fork_and_execute(cmd, shell, commands, data->cmd_index));
}
