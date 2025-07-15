/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 15:25:08 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/15 15:26:30 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_child_process(int *pipe_fd, char *delimiter, t_shell *shell)
{
	sigset_t	orig_mask;
	sigset_t	block_mask;

	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGINT);
	sigaddset(&block_mask, SIGQUIT);
	sigprocmask(SIG_BLOCK, &block_mask, &orig_mask);
	close(pipe_fd[0]);
	sigprocmask(SIG_SETMASK, &orig_mask, NULL);
	set_state(shell, IN_HEREDOC);
	collect_heredoc_input(pipe_fd[1], delimiter);
	return (0);
}

int	handle_parent_process(int *pipe_fd, pid_t pid, t_shell *shell)
{
	int			status;
	sigset_t	orig_mask;
	sigset_t	block_mask;

	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGINT);
	sigaddset(&block_mask, SIGQUIT);
	sigprocmask(SIG_BLOCK, &block_mask, &orig_mask);
	close(pipe_fd[1]);
	waitpid(pid, &status, 0);
	sigprocmask(SIG_SETMASK, &orig_mask, NULL);
	if (WIFSIGNALED(status))
	{
		close(pipe_fd[0]);
		g_signal_received = WTERMSIG(status);
		shell->exit_status = 130;
		return (-1);
	}
	else if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}
