/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/21 13:24:51 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/03 12:50:04 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_commands(t_command *commands, t_shell *shell)
{
	int	exit_status;

	if (!commands)
		return (1);
	if (!check_commands(commands))
	{
		free_tokens(shell->tokens);
		return (free_commands(commands), 1);
	}
	if (!process_redirections(commands))
	{
		free_tokens(shell->tokens);
		free_commands(commands);
		return (1);
	}
	if (!setup_pipes(commands))
	{
		free_tokens(shell->tokens);
		free_commands(commands);
		return (1);
	}
	exit_status = run_command_pipeline(commands, shell->env);
	free_tokens(shell->tokens);
	free_commands(commands);
	return (exit_status);
}

int	run_command_pipeline(t_command *commands, t_env *env_list)
{
	int	cmd_count;
	pid_t	*pids;
	int		exit_status;
	int		cmd_index;
	t_command	*cmd;
	t_command *temp;

	cmd_count = count_commands(commands);
	exit_status = 0;
	cmd_index = 0;
	cmd = commands;
	pids = ft_calloc(cmd_count, sizeof(pid_t));
	if (!pids)
		return (1);
	while (cmd)
	{
		if (is_builtin(cmd->cmd->str))
		{
			ft_dprintf(2, "Not implemented yet\n");
		//	exit_status = execute_builtin(cmd, env_list);
		}
		else
		{
			pids[cmd_index] = fork();
			if (pids[cmd_index] == 0)
			{
				temp = commands;
				while (temp)
				{
					if (temp != cmd) // Skip current command's FDs
					{
						if (temp->in_fd > 2)
							close(temp->in_fd);
						if (temp->out_fd > 2)
							close(temp->out_fd);
					}
					temp = temp->next;
				}
				exit_status = setup_command_redirections(cmd);
				execute_external_command(cmd, env_list);
				exit(127);
			}
		}
		cmd_index++;
		cmd = cmd->next;
	}
	exit_status = wait_for_children(pids, cmd_count);
	free(pids);
	cmd = commands;
	while (cmd)
	{
		if (cmd->in_fd > 2)
			close(cmd->in_fd);
		if (cmd->out_fd > 2)
			close(cmd->out_fd);
		cmd = cmd->next;
	}
	return (exit_status);
}

int	setup_command_redirections(t_command *cmd)
{
	int	max_fd;
	int	i;

	if (cmd->in_fd != STDIN_FILENO)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			return(perror("dup2 failed for in_fd"), 1);
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			return(perror("dup2 failed for out_fd"), 1);
	}
	max_fd = sysconf(_SC_OPEN_MAX);
	i = 3;
	while (i < max_fd)
	{
		if (i != cmd->in_fd && i != cmd->out_fd)
			close(i);
		i++;
	}
}

int	execute_external_command(t_command *cmd, t_env *env_list)
{
	char	**args;
	char	**envp;
	char	path[PATH_MAX];

	args = tokens_to_args(cmd->cmd, cmd->args);
	if (!args)
	{
		exit(127);
	}
	envp = env_to_array(env_list);
	if (!envp)
		exit(127);
	if (find_command_path(args[0], envp, path))
	{
		execve(path, args, envp);
		perror("execve failed");
	}
	else
		ft_dprintf(2, "minishell: %s: command not found\n", args[0]);
	ft_free_array(args);
	ft_free_array(envp);
	exit(127);
}

int	wait_for_children(pid_t *pids, int count)
{
	int status;
	int exit_code = 0;

	for (int i = 0; i < count; i++)
	{
		if (pids[i] > 0) // Valid PID
		{
			if (waitpid(pids[i], &status, 0) == -1)
			{
				perror("waitpid failed");
				continue;
			}
			if (i == count - 1)
			{
				if (WIFEXITED(status))
					exit_code = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					exit_code = 128 + WTERMSIG(status);
				else
					exit_code = 1;
			}
		}
	}
	return exit_code;
}

char **env_to_array(t_env *env_list)
{
	int count = 0;
	t_env *current = env_list;

	while (current)
	{
		count++;
		current = current->next;
	}
	// Allocate array
	char **result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return NULL;
	current = env_list;
	int i = 0;
	while (current)
	{
		// Format: KEY=VALUE
		char *temp = ft_strjoin(current->key, "=");
		if (!temp)
		{
			ft_free_array(result);
			return NULL;
		}
		result[i] = ft_strjoin(temp, current->value);
		free(temp);
		if (!result[i])
		{
			ft_free_array(result);
			return NULL;
		}
		current = current->next;
		i++;
	}
	result[i] = NULL;
	return result;
}

bool is_builtin(char *cmd)
{
	if (!cmd)
		return false;

	return (ft_strcmp(cmd, "cd") == 0 ||
			ft_strcmp(cmd, "echo") == 0 ||
			ft_strcmp(cmd, "pwd") == 0 ||
			ft_strcmp(cmd, "export") == 0 ||
			ft_strcmp(cmd, "unset") == 0 ||
			ft_strcmp(cmd, "env") == 0 ||
			ft_strcmp(cmd, "exit") == 0);
}

int count_commands(t_command *commands)
{
	int count = 0;
	t_command *current = commands;

	while (current)
	{
		count++;
		current = current->next;
	}
	return count;
}

void free_commands(t_command *commands)
{
	t_command *temp;

	while (commands)
	{
		temp = commands;
		commands = commands->next;
		if (temp->in_fd > 2)
			close(temp->in_fd);
		if (temp->out_fd > 2)
			close(temp->out_fd);
		free(temp);
	}
}
