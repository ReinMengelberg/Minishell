/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/21 13:24:51 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/14 19:09:58 by rmengelb      ########   odam.nl         */
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

int	run_command_pipeline(t_command *commands, t_shell *shell)
{
    int		cmd_count;
    int		exit_status;
    int		cmd_index;
    t_command	*cmd;
    bool	has_failed_redirections;
	bool	any_command_executed;

    cmd_count = count_commands(commands);
    exit_status = 0;
    cmd_index = 0;
    cmd = commands;
	any_command_executed =false;
    has_failed_redirections = false;
    
    // Check if all commands are empty
    t_command *temp_cmd = commands;
    bool all_empty = true;
    while (temp_cmd)
    {
        if (temp_cmd->cmd && temp_cmd->cmd->str && temp_cmd->cmd->str[0] != '\0')
        {
            all_empty = false;
            break;
        }
        temp_cmd = temp_cmd->next;
    }
    
    // If all commands are empty, return 0
    if (all_empty)
        return (0);
    
    shell->pids = ft_calloc(cmd_count, sizeof(pid_t));
    if (!shell->pids)
        return (1);
    while (cmd)
    {
        if (cmd->in_fd == -2 || cmd->out_fd == -2)
        {
            has_failed_redirections = true;
            cmd_index++;
            cmd = cmd->next;
            continue;
        }
        
        // Skip empty commands
        if (!cmd->cmd || !cmd->cmd->str || cmd->cmd->str[0] == '\0')
        {
            cmd_index++;
            cmd = cmd->next;
            continue;
        }
        any_command_executed = true;
        if (is_builtin(cmd->cmd->str) && cmd_count == 1 && !cmd->is_piped)
        {
            // Check for failed redirections before executing builtin
            if (cmd->in_fd == -2 || cmd->out_fd == -2)
            {
                cmd_index++;
                cmd = cmd->next;
                continue;
            }
            
            int stdin_save = dup(STDIN_FILENO);
            int stdout_save = dup(STDOUT_FILENO);

            // Set up redirections for builtin
            if (cmd->in_fd != STDIN_FILENO && cmd->in_fd >= 0)
                dup2(cmd->in_fd, STDIN_FILENO);
            if (cmd->out_fd != STDOUT_FILENO && cmd->out_fd >= 0)
                dup2(cmd->out_fd, STDOUT_FILENO);
            exit_status = exec_builtin(cmd, shell);
            // Restore original stdin/stdout
            dup2(stdin_save, STDIN_FILENO);
            dup2(stdout_save, STDOUT_FILENO);
            close(stdin_save);
            close(stdout_save);
        }
        else
        {
            shell->pids[cmd_index] = fork();
            if (shell->pids[cmd_index] == 0)
            {
                close_unused_pipes(commands, cmd);
                setup_command_redirections(cmd);
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
                // Don't override exit code if shell is exiting
                if (shell->status != 0)
                    exit_status = 1;
            }
        }
        cmd_index++;
        cmd = cmd->next;
    }
    close_all_pipes(commands);
    if (!(cmd_count == 1 && is_builtin(commands->cmd->str) && !commands->is_piped))
    {
        if (shell->status != 0)
            exit_status = wait_for_children(shell->pids, cmd_count);
        else
            wait_for_remain(shell->pids, cmd_count);
    }
    free(shell->pids);
    shell->pids = NULL;
	if (has_failed_redirections && !any_command_executed)
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

void	setup_command_redirections(t_command *cmd)
{
	int	max_fd;
	int	i;

	if (cmd->in_fd != STDIN_FILENO && cmd->in_fd >= 0)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			return(perror("dup2 failed for in_fd"));
	}
	if (cmd->out_fd != STDOUT_FILENO && cmd->out_fd >=0)
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
}

int	execute_external_command(t_command *cmd, t_env *env_list)
{
    char	**args;
    char	**envp;
    char	path[PATH_MAX];
    struct stat	path_stat;

    args = tokens_to_args(cmd->cmd, cmd->args);
    if (!args)
        exit(127);
    envp = env_to_array(env_list);
    if (!envp)
    {
        ft_free_array(args);
        exit(127);
    }
    
    // Check if it's an absolute or relative path first
    if (strchr(args[0], '/'))
    {
        if (stat(args[0], &path_stat) == 0)
        {
            if (S_ISDIR(path_stat.st_mode))
            {
                ft_dprintf(2, "minishell: %s: Is a directory\n", args[0]);
                ft_free_array(args);
                ft_free_array(envp);
                exit(126);
            }
            if (access(args[0], X_OK) != 0)
            {
                ft_dprintf(2, "minishell: %s: Permission denied\n", args[0]);
                ft_free_array(args);
                ft_free_array(envp);
                exit(126);
            }
            execve(args[0], args, envp);
            ft_dprintf(2, "minishell: %s: %s\n", args[0], strerror(errno));
        }
        else
        {
            ft_dprintf(2, "minishell: %s: No such file or directory\n", args[0]);
            ft_free_array(args);
            ft_free_array(envp);
            exit(127);
        }
    }
    else if (find_command_path(args[0], envp, path))
    {
        // Check if the path is a directory
        if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
        {
            ft_dprintf(2, "minishell: %s: Is a directory\n", args[0]);
            ft_free_array(args);
            ft_free_array(envp);
            exit(126);
        }
        execve(path, args, envp);
        ft_dprintf(2, "minishell: %s: %s\n", args[0], strerror(errno));
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
	return (wait_for_remain(pids, count), exit_code);
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
    t_token *arg_temp;

    while (commands)
    {
        temp = commands;
        commands = commands->next;
        while (temp->args)
        {
            arg_temp = temp->args;
            temp->args = temp->args->next;
            if (arg_temp->str)
                free(arg_temp->str);
            free(arg_temp);
        }
		while (temp->input_list)
        {
            arg_temp = temp->input_list;
            temp->input_list = temp->input_list->next;
            if (arg_temp->str)
                free(arg_temp->str);
            free(arg_temp);
        }
        while (temp->output_list)
        {
            arg_temp = temp->output_list;
            temp->output_list = temp->output_list->next;
            if (arg_temp->str)
                free(arg_temp->str);
            free(arg_temp);
        }
        if (temp->in_fd > 2)
            close(temp->in_fd);
        if (temp->out_fd > 2)
            close(temp->out_fd);
        free(temp);
    }
}
