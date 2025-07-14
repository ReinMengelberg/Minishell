/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/08 15:09:48 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/14 13:56:38 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_env(t_command *cmd, t_shell *shell)
{
	(void)cmd;
	if (!shell || !shell->env)
	{
		printf("Environment is empty\n");
		return (SUCCESS);
	}
	print_env(shell->env);
	return (SUCCESS);
}
