/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_cmd.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/02 18:10:40 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/14 15:09:45 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static t_command	*create_cmd_node(void)
{
	t_command	*node;

	node = (t_command *)ft_calloc(sizeof(t_command), 1);
	return (node);
}

static void	free_cmd_list(t_command *head)
{
	t_command	*temp;

	while (head)
	{
		temp = head->next;
		free (head);
		head = NULL;
		head = temp;
	}
}

//LinkedList allocation
t_command	*allocate_cmds(int count)
{
	int			i;
	t_command	*head;
	t_command	*temp;
	t_command	*cmd;

	cmd = create_cmd_node();
	if (cmd == NULL)
		return (NULL);
	i = 1;
	head = cmd;
	while (i < count)
	{
		temp = create_cmd_node();
		if (temp == NULL)
		{
			free_cmd_list(head);
			return (NULL);
		}
		cmd->next = temp;
		cmd = cmd->next;
		i++;
	}
	return (head);
}

/*	Parses command string into arguments
	Verifies command validity
	Finds command path in PATH
	Executes command with execve()*/
void	execute_cmd(t_command *cmd, t_pipex *pipex)
{
	char	**args;
	char	path[PATH_MAX];

	args = cmd->args;
	if (!args || !args[0] || args[0][0] == '\0')
		error_exit("Empty command", pipex, 127);
	if (find_path(args[0], pipex->envp, path) == 1)
		error_exit("Command not found", pipex, 127);
	execve(path, args, pipex->envp);
	error_exit("Execve failed", pipex, 1);
}

/*	Handles all children
First child: redirects stdin to input file and stdout to pipe write-end
Last child: redirects stdin to pipe read-end and stdout to output file
Executes appropriate command
Handles error cases (file not found)*/
void	execution(t_pipex *pipex)
{
	int			i;
	t_command	*cmd_head;

	cmd_head = pipex->cmds;
	i = 0;
	while (cmd_head)
	{
		cmd_head->pid = fork();
		if (cmd_head->pid == -1)
			error_exit("Pid failed", pipex, 1);
		else if (cmd_head->pid == 0)
		{
			dup2_fds(pipex, cmd_head, i);
			execute_cmd(cmd_head, pipex);
		}
		i++;
		cmd_head = cmd_head->next;
	}
	i = 0;
	while (i < (pipex->count_cmds - 1) * 2)
	{
		close (pipex->pipe_fd[i]);
		i++;
	}
}
