/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_error.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/30 13:50:00 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/09 15:19:50 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	free_everything(t_pipex *pipex)
{
	t_command	*cmd_head;
	t_command	*temp;

	cmd_head = pipex->cmds;
	while (cmd_head)
	{
		temp = cmd_head;
		cmd_head = cmd_head->next;
		free_array(temp->args);
		if (temp->infile_path)
			free(temp->infile_path);
		free(temp);
	}
	if (pipex->pipe_fd)
		free(pipex->pipe_fd);
}

/*	Prints error message to stderr
	Cleans up resources (frees arrays)
	Exits with failure status*/
void	error_exit(char *message, t_pipex *pipex, int exit_num)
{
	ft_dprintf(2, "%s\n", message);
	if (pipex)
		free_everything(pipex);
	exit(exit_num);
}

/*	Frees the array used for command arguments and environment variables*/
void	free_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
