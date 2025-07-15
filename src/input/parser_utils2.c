/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils2.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/14 13:23:36 by rbagin        #+#    #+#                 */
/*   Updated: 2025/07/15 15:48:19 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->input_list = NULL;
	cmd->output_list = NULL;
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
	cmd->is_piped = false;
	cmd->next = NULL;
	return (cmd);
}

void	add_to_input_list(t_command *cmd, t_token *token)
{
	t_token	*new_token;
	t_token	*current;

	new_token = create_token(token->str, token->type, token->quotestate);
	if (!new_token)
		return ;
	if (token->next && token->next->type == FILENAME)
	{
		new_token->next = create_token(token->next->str, token->next->type,
				token->next->quotestate);
		if (new_token->next)
			new_token->next->prev = new_token;
	}
	if (!cmd->input_list)
		cmd->input_list = new_token;
	else
	{
		current = cmd->input_list;
		while (current->next)
			current = current->next;
		current->next = new_token;
		new_token->prev = current;
	}
}
