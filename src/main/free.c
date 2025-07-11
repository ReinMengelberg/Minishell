/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   free.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 14:41:27 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/06 16:48:44 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_commands_without_tokens(t_command *commands)
{
	t_command *temp;
	t_token	*arg_temp;

	while (commands)
	{
		temp = commands;
		commands = commands->next;
		while (temp->args)
		{
			arg_temp = temp->args;
			temp->args = temp->args->next;
			if (arg_temp->str)
				free(arg_temp->str);
			free(arg_temp);
		}
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
