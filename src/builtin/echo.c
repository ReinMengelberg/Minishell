/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:45 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/14 16:47:57 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to check if a string is exactly "-n"
static bool	is_n_flag(const char *str)
{
	int	i;

	if (!str || str[0] != '-')
		return (false);
	if (str[1] != 'n')
		return (false);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (i > 1);
}

int	exec_echo(t_command *cmd, t_shell *shell)
{
	t_token	*current_arg;
	bool	newline;

	newline = true;
	(void)shell;
	if (!cmd)
		return (ERROR_NULL_POINTER);
	current_arg = cmd->args;
	while (current_arg && is_n_flag(current_arg->str))
	{
		newline = false;
		current_arg = current_arg->next;
	}
	while (current_arg && current_arg->type == ARG)
	{
		printf("%s", current_arg->str);
		current_arg = current_arg->next;
		if (current_arg && current_arg->type == ARG)
			printf(" ");
	}
	if (newline)
		printf("\n");
	return (SUCCESS);
}
