/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   redirections_handlers.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/20 15:00:00 by rbagin        #+#    #+#                 */
/*   Updated: 2025/07/20 15:10:21 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	process_output_token(t_command *cmd, t_token *current)
{
	t_token	*filename;
	int		flags;

	filename = current->next;
	if (!filename || filename->type != FILENAME)
	{
		cmd->out_fd = -2;
		return (true);
	}
	if (current->type == OUTPUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	if (open_output_file(filename, cmd, flags) == -1)
		return (true);
	return (false);
}

bool	handle_all_inputs(t_command *cmd, t_shell *shell)
{
	t_token	*current;

	current = cmd->input_list;
	while (current)
	{
		if (process_single_input(cmd, current, shell))
			return (true);
		if (current->next && current->next->type == FILENAME)
			current = current->next->next;
		else
			current = current->next;
	}
	return (false);
}

bool	handle_all_outputs(t_command *cmd)
{
	t_token	*current;

	current = cmd->output_list;
	while (current)
	{
		if (process_output_token(cmd, current))
			return (true);
		if (current->next && current->next->type == FILENAME)
			current = current->next->next;
		else
			current = current->next;
	}
	return (false);
}
