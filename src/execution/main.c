/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/05 19:48:58 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/14 14:55:45 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_pipex(t_pipex *pipex, char *argv[], int argc)
{
	t_command	*cmd_head;
	int			i;

	i = 2;
	pipex->count_cmds = argc - 3;
	pipex->cmds = allocate_cmds(pipex->count_cmds);
	if (!pipex->cmds)
		error_exit("Memory allocation failed", NULL, 1);
	cmd_head = pipex->cmds;
	while (cmd_head)
	{
		if (argv[i] && argv[i][0] != '\0')
			cmd_head->args = ft_split(argv[i], ' ');
		else
			cmd_head->args = ft_calloc(sizeof(char *), 1);
		if (!cmd_head->args)
			error_exit("ft_split failed", pipex, 1);
		cmd_head = cmd_head->next;
		i++;
	}
	pipex->pipe_fd = ft_calloc(sizeof(int) * pipex->count_cmds * 2, 1);
	if (!pipex->pipe_fd)
		error_exit("pipe creation failed", pipex, 1);
}

/*	Waits for both child processes to complete
	Cleans up file descriptors
	Returns the exit status of the second command*/
static int	wait_and_cleanup(t_pipex *pipex)
{
	t_command	*cmd_head;
	int			status;
	int			exit_code;

	cmd_head = pipex->cmds;
	exit_code = 0;
	while (cmd_head)
	{
		if (cmd_head->pid > 0)
		{
			waitpid(cmd_head->pid, &status, 0);
			if (cmd_head->next == NULL && cmd_head->pid > 0)
			{
				pipex->last_error = status;
				if (WIFEXITED(status))
					exit_code = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					exit_code = 128 + WTERMSIG(status);
				else
					exit_code = 1;
			}
		}
		cmd_head = cmd_head->next;
	}
	return (exit_code);
}

int	main(int argc, char *argv[], char **envp)
{
	t_pipex		pipex;
	int			exit_status;
	t_command	*last_cmd;

	if (argc < 5)
		error_exit("Usage: ./pipex infile cmd1 cmd2 ... outfile", NULL, 1);
	pipex.envp = envp;
	if (is_here_doc(argv[1]))
		setup_heredoc(&pipex, argv, argc);
	else
	{
		init_pipex(&pipex, argv, argc);
		get_infile_outfile(&pipex, argv, argc);
	}
	last_cmd = pipex.cmds;
	check_file_fds(&pipex);
	create_pipes(pipex.pipe_fd, pipex.count_cmds, &pipex);
	execution(&pipex);
	exit_status = wait_and_cleanup(&pipex);
	while (last_cmd->next)
		last_cmd = last_cmd->next;
	if (last_cmd->outfile_fd < 0)
		exit_status = 1;
	free_everything(&pipex);
	return (exit_status);
}
