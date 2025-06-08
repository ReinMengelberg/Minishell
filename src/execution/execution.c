/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/21 13:24:51 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/08 11:08:11 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_commands(t_command *commands, t_shell *shell)
{
	if (!commands)
		return (1);
	if (!check_commands(commands))
	{
		free_tokens(shell->tokens);
		return (free_commands(commands), 1);
	}
	if (!process_heredocs(commands, shell))
	{
		free_tokens(shell->tokens);
		free_commands(commands);
		return (130);
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
	shell->exit_status = run_command_pipeline(commands, shell->env, shell->pids);
	free_tokens(shell->tokens);
	free_commands(commands);
	return (shell->exit_status);
}

int	run_command_pipeline(t_command *commands, t_env *env_list, pid_t *pids)
{
	int	cmd_count;
	int		exit_status;
	int		cmd_index;
	t_command	*cmd;

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
				close_unused_pipes(commands, cmd);
				setup_command_redirections(cmd);
				execute_external_command(cmd, env_list);
				exit(127);
			}
			else if (pids[cmd_index] < 0)
			{
				perror("fork failed");
				exit_status = 1;
			}
		}
		cmd_index++;
		cmd = cmd->next;
	}
	close_all_pipes(commands);
	exit_status = wait_for_children(pids, cmd_count);
	free(pids);
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

void	setup_command_redirections(t_command *cmd)
{
	int	max_fd;
	int	i;

	if (cmd->in_fd != STDIN_FILENO)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			return(perror("dup2 failed for in_fd"));
	}
	if (cmd->out_fd != STDOUT_FILENO)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			return(perror("dup2 failed for out_fd"));
	}
	max_fd = sysconf(_SC_OPEN_MAX);
	i = 3;
	while (i < max_fd)
	{
		if (i != cmd->in_fd && i != cmd->out_fd)
			close(i);
		i++;
	}
	return ;
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
	int	status;
	int	exit_code;
	int	last_cmd_index;

	exit_code = 0;
	last_cmd_index = count - 1;
	if (last_cmd_index >= 0 && pids[last_cmd_index] > 0)
	{
		if (waitpid(pids[last_cmd_index], &status, 0) == -1)
		{
			perror("waitpid failed for last command");
			exit_code = 1;
		}
		else
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = 128 + WTERMSIG(status);
			else
				exit_code = 1;
		}
		pids[last_cmd_index] = -1;
	}
	wait_for_remain(pids, count);
	return (exit_code);
}

void	wait_for_remain(pid_t *pids, int count)
{
	int	status;
	int	i;

	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
		{
			if (waitpid(pids[i], &status, 0) == -1)
				perror("waitpid failed");
		}
		i++;
	}
}

char **env_to_array(t_env *env_list)
{
	int count = 0;
	t_env *current;

	current = env_list;
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
