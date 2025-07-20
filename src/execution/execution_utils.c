/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 15:38:30 by rein          #+#    #+#                 */
/*   Updated: 2025/07/20 10:24:22 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_command_redirections(t_command *cmd)
{
	int	max_fd;
	int	i;

	if (cmd->in_fd != STDIN_FILENO && cmd->in_fd >= 0)
	{
		if (dup2(cmd->in_fd, STDIN_FILENO) == -1)
			return (perror("dup2 failed for in_fd"));
	}
	if (cmd->out_fd != STDOUT_FILENO && cmd->out_fd >= 0)
	{
		if (dup2(cmd->out_fd, STDOUT_FILENO) == -1)
			return (perror("dup2 failed for out_fd"));
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

static int	handle_absolute_path(char **args, char **envp)
{
	struct stat	path_stat;

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
		ft_dprintf(2, "minishell: %s: No such file or directory\n", args[0]);
	return (127);
}

static int	handle_relative_path(char **args, char **envp, char *path)
{
	struct stat	path_stat;

	if (find_command_path(args[0], envp, path))
	{
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
	return (127);
}

int	execute_external_command(t_command *cmd, t_env *env_list)
{
	char	**args;
	char	**envp;
	char	path[PATH_MAX];

	args = tokens_to_args(cmd->cmd, cmd->args);
	if (!args)
		exit(127);
	envp = env_to_array(env_list);
	if (!envp)
	{
		ft_free_array(args);
		exit(127);
	}
	if (strchr(args[0], '/'))
		handle_absolute_path(args, envp);
	else
		handle_relative_path(args, envp, path);
	ft_free_array(args);
	ft_free_array(envp);
	exit(127);
}

int	wait_for_children(pid_t *pids, int count)
{
    int	status;
    int	exit_code;
    int	last_cmd_index;
    int	sig;

    // Temporarily ignore signals in parent while waiting
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
    
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
            {
                sig = WTERMSIG(status);
                if (sig == SIGINT)
                {
                    write(STDOUT_FILENO, "\n", 1);  // Print newline for ^C
                    exit_code = 130;
                }
                else if (sig == SIGQUIT)
                {
                    write(STDERR_FILENO, "Quit (core dumped)\n", 19);
                    exit_code = 131;
                }
                else
                    exit_code = 128 + sig;
            }
            else
                exit_code = 1;
        }
        pids[last_cmd_index] = -1;
    }
    
    // Restore interactive signal handling
    setup_signal_handler(handle_signal_interactive);
    signal(SIGQUIT, SIG_IGN);
    
    return (wait_for_remain(pids, count), exit_code);
}
