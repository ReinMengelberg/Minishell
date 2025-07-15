/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 14:41:27 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/15 15:55:40 by rein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	free_token_list(t_token **list)
{
	t_token	*temp;

	while (*list)
	{
		temp = *list;
		*list = (*list)->next;
		if (temp->str)
			free(temp->str);
		free(temp);
	}
}

void	free_commands_without_tokens(t_command *commands)
{
	t_command	*temp;

	while (commands)
	{
		temp = commands;
		commands = commands->next;
		free_token_list(&temp->args);
		free_token_list(&temp->input_list);
		free_token_list(&temp->output_list);
		if (temp->in_fd > 2)
			close(temp->in_fd);
		if (temp->out_fd > 2)
			close(temp->out_fd);
		free(temp);
	}
}

void	free_everything(t_shell *shell, bool on_exit)
{
	if (!shell)
		return ;
	if (shell->commands)
	{
		free_commands_without_tokens(shell->commands);
		shell->commands = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->pids)
	{
		free(shell->pids);
		shell->pids = NULL;
	}
	if (shell->env && on_exit)
	{
		free_env(shell->env);
		shell->env = NULL;
	}
}
