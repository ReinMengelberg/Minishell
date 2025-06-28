/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:45 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/28 10:52:07 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to check if a string is exactly "-n"
static bool is_n_flag(const char *str)
{
	return (str && strcmp(str, "-n") == 0);
}

int exec_echo(t_command *cmd, t_env *env_list)
{
	t_token	*current_arg;
	bool	newline;

	newline = true;
	(void)env_list;
	if (!cmd)
		return (ERROR_NULL_POINTER);
	current_arg = cmd->args;
	if (current_arg && is_n_flag(current_arg->str))
	{
		newline = false;
		current_arg = current_arg->next;
	}
	while (current_arg)
	{
		printf("%s", current_arg->str);
		current_arg = current_arg->next;
	}
	if (newline)
		printf("\n");
	return (SUCCESS);
}

