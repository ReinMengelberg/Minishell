/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cleanup.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 15:39:08 by rein          #+#    #+#                 */
/*   Updated: 2025/07/15 15:51:39 by rein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_token_list(t_token *tokens)
{
	t_token	*arg_temp;

	while (tokens)
	{
		arg_temp = tokens;
		tokens = tokens->next;
		if (arg_temp->str)
			free(arg_temp->str);
		free(arg_temp);
	}
}

static void	close_command_fds(t_command *cmd)
{
	if (cmd->in_fd > 2)
		close(cmd->in_fd);
	if (cmd->out_fd > 2)
		close(cmd->out_fd);
}

void	free_commands(t_command *commands)
{
	t_command	*temp;

	while (commands)
	{
		temp = commands;
		commands = commands->next;
		free_token_list(temp->args);
		free_token_list(temp->input_list);
		free_token_list(temp->output_list);
		close_command_fds(temp);
		free(temp);
	}
}
