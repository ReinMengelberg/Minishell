/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execution_wait.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/20 15:21:00 by rbagin        #+#    #+#                 */
/*   Updated: 2025/07/20 15:21:19 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_signal_exit(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (130);
	}
	else if (sig == SIGQUIT)
	{
		write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		return (131);
	}
	else
		return (128 + sig);
}

static int	process_child_status(int status)
{
	int	sig;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		sig = WTERMSIG(status);
		return (handle_signal_exit(sig));
	}
	else
		return (1);
}

int	wait_for_children(pid_t *pids, int count)
{
	int	status;
	int	exit_code;
	int	last_cmd_index;

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
			exit_code = process_child_status(status);
		pids[last_cmd_index] = -1;
	}
	setup_signal_handler(handle_signal_interactive);
	signal(SIGQUIT, SIG_IGN);
	wait_for_remain(pids, count);
	return (exit_code);
}
