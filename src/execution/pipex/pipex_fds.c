/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_fds.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 14:55:18 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/14 15:11:39 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	create_pipes(int *fd, int count, t_pipex *pipex)
{
	int		i;

	i = 0;
	while (i < count - 1)
	{
		if (pipe(&fd[i * 2]) == -1)
			error_exit("Pipe creation failed", pipex, 1);
		i++;
	}
}

void	check_file_fds(t_pipex *pipex)
{
	t_command	*cmd_head;
	int			i;

	i = 0;
	cmd_head = pipex->cmds;
	while (cmd_head)
	{
		if (i == 0 && cmd_head->infile_fd < 0)
			ft_dprintf(2, "pipex: %s: No such file or directory\n",
				cmd_head->infile_path);
		else if (i == pipex->count_cmds - 1 && cmd_head->outfile_fd < 0)
			ft_dprintf(2, "Cannot open output file");
		cmd_head = cmd_head->next;
		i++;
	}
}

void	close_unused_pipes(t_pipex *pipex, int cmd_index)
{
	int	i;

	i = 0;
	while (i < (pipex->count_cmds - 1) * 2)
	{
		if ((cmd_index > 0 && i == (cmd_index - 1) * 2)
			|| (cmd_index < pipex->count_cmds - 1 && i == cmd_index * 2 + 1))
		{
			i++;
			continue ;
		}
		close(pipex->pipe_fd[i++]);
	}
}

void	dup2_fds(t_pipex *pipex, t_command *cmd, int cmd_index)
{
	close_unused_pipes(pipex, cmd_index);
	if (cmd_index == 0)
	{
		if (dup2(cmd->infile_fd, STDIN_FILENO) == -1)
			error_exit("dup2 failed for infile", pipex, 1);
	}
	else
	{
		if (dup2(pipex->pipe_fd[(cmd_index - 1) * 2], STDIN_FILENO) == -1)
			error_exit("dup2 failed for pipe read", pipex, 1);
	}
	if (cmd_index == pipex->count_cmds - 1)
	{
		if (dup2(cmd->outfile_fd, STDOUT_FILENO) == -1)
			error_exit("dup2 failed for outfile", pipex, 1);
	}
	else
	{
		if (dup2(pipex->pipe_fd[cmd_index * 2 + 1], STDOUT_FILENO) == -1)
			error_exit("dup2 failed for pipe write", pipex, 1);
	}
	close_remaining_fds(pipex, cmd, cmd_index);
}

void	close_remaining_fds(t_pipex *pipex, t_command *cmd, int cmd_index)
{
	int	i;

	if (cmd_index == 0 && cmd->infile_fd > 0)
		close(cmd->infile_fd);
	if (cmd_index == pipex->count_cmds - 1 && cmd->outfile_fd > 0)
		close(cmd->outfile_fd);
	i = 0;
	while (i < (pipex->count_cmds - 1) * 2)
		close (pipex->pipe_fd[i++]);
}
