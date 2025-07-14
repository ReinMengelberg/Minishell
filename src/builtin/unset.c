/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:57 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/14 15:34:36 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_unset(t_command *cmd, t_shell *shell) // Pass whole shell
{
	t_token	*current_arg;
	int		exit_status;

	if (!cmd || !shell || !shell->env)
		return (ERROR_NULL_POINTER);
	if (!cmd->args)
		return (SUCCESS);
	exit_status = SUCCESS;
	current_arg = cmd->args;
	while (current_arg)
	{
		if (!is_valid_var_syntax(current_arg->str, false))
		{
			ft_dprintf(2, "unset: `%s': not a valid identifier\n",
				current_arg->str);
			exit_status = FAILURE;
		}
		else
			remove_env_var(&shell->env, current_arg->str);
		current_arg = current_arg->next;
	}
	return (exit_status);
}
