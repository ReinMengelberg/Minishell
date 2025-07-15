/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser_utils.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/14 13:23:36 by rbagin        #+#    #+#                 */
/*   Updated: 2025/07/15 15:48:30 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_to_output_list(t_command *cmd, t_token *token)
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
	if (!cmd->output_list)
		cmd->output_list = new_token;
	else
	{
		current = cmd->output_list;
		while (current->next)
			current = current->next;
		current->next = new_token;
		new_token->prev = current;
	}
}

void	add_to_args(t_command *cmd, t_token *arg_token)
{
	t_token	*new_token;
	t_token	*current;

	new_token = create_token(arg_token->str, arg_token->type,
			arg_token->quotestate);
	if (!new_token)
		return ;
	if (!cmd->args)
		cmd->args = new_token;
	else
	{
		current = cmd->args;
		while (current->next)
			current = current->next;
		current->next = new_token;
		new_token->prev = current;
	}
}

bool	setup_pipes(t_command *commands)
{
	t_command	*current;
	int			pipe_fd[2];

	current = commands;
	while (current && current->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("setup_pipes: pipe creation failed");
			return (false);
		}
		if (current->out_fd == STDOUT_FILENO)
			current->out_fd = pipe_fd[1];
		else
			close(pipe_fd[1]);
		if (current->next->in_fd == STDIN_FILENO)
			current->next->in_fd = pipe_fd[0];
		else
			close(pipe_fd[0]);
		current->is_piped = true;
		current = current->next;
	}
	return (true);
}

bool	check_commands(t_command *cmd_head)
{
	t_command	*cmds;

	cmds = cmd_head;
	while (cmds)
	{
		if (!cmds->cmd)
			return (false);
		cmds = cmds->next;
	}
	return (true);
}

t_command	*create_new_command_node(t_command **cmd_head,
										t_command **current_cmd)
{
	t_command	*new_cmd;

	new_cmd = create_command();
	if (!new_cmd)
	{
		if (*cmd_head)
			free_commands(*cmd_head);
		return (NULL);
	}
	if (!*cmd_head)
		*cmd_head = new_cmd;
	else
	{
		(*current_cmd)->next = new_cmd;
		(*current_cmd)->is_piped = true;
	}
	*current_cmd = new_cmd;
	(*current_cmd)->in_fd = STDIN_FILENO;
	(*current_cmd)->out_fd = STDOUT_FILENO;
	return (new_cmd);
}
