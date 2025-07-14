/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:48 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/14 15:26:16 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	isnum(char *str)
{
	int	i;

	if (!str || !*str)
		return (false);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

int	calc_exit(char *exit_arg)
{
	int	exit_code;

	exit_code = ft_atoi(exit_arg);
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code += 256;
	return (exit_code);
}

int	validate_exit_args(t_command *cmd, t_shell *shell)
{
	if (!isnum(cmd->args->str))
	{
		ft_dprintf(2, "minishell: exit: numeric argument required\n");
		return (2);
	}
	if (cmd->args->next)
	{
		ft_dprintf(2, "minishell: exit: too many arguments\n");
		shell->exit_status = 1;
		return (1);
	}
	return (0);
}

int	exec_exit(t_command *cmd, t_shell *shell)
{
	int		exit_code;
	int		validation_result;

	exit_code = 0;
	if (!cmd->args)
		exit_code = shell->exit_status;
	else
	{
		validation_result = validate_exit_args(cmd, shell);
		if (validation_result != 0)
			return (validation_result);
		exit_code = calc_exit(cmd->args->str);
	}
	shell->exit_status = exit_code;
	if (shell->state == INTERACTIVE)
		shell->status = 0;
	return (exit_code);
}
