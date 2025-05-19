/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_process.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/30 13:57:58 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/14 15:07:23 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	copy_direct_path(char *cmd, char *final_path)
{
	int	j;

	j = -1;
	while (cmd[++j] != '\0')
		final_path[j] = cmd[j];
	final_path[j] = '\0';
}

int	open_input_file(char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		ft_dprintf(2, "Error opening input file: %s: %s\n", file,
			strerror(errno));
	}
	return (fd);
}

/*	Opens the input or the output file
	It also sets the permission of the output file to 0644(user can read/write,
	group and others can read)
	mode == 1 - Write mode(output file)*/
int	open_output_file(char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_dprintf(2, "Error opening output file: %s:%s\n", file,
			strerror(errno));
		return (fd);
	}
	return (fd);
}

void	get_infile_outfile(t_pipex *pipex, char **argv, int argc)
{
	t_command	*cmd_head;

	cmd_head = pipex->cmds;
	if (cmd_head)
	{
		cmd_head->infile_path = ft_strdup(argv[1]);
		if (!cmd_head->infile_path)
			error_exit("Memory allocation failed", pipex, 1);
		cmd_head->infile_fd = open_input_file(argv[1]);
		while (cmd_head->next != NULL)
			cmd_head = cmd_head->next;
		cmd_head->outfile_fd = open_output_file(argv[argc - 1]);
	}
}
