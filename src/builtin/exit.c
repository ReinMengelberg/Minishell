/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:48 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/22 21:22:53 by rein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_exit_args(t_command *cmd, t_shell *shell, bool *should_exit)
{
	*should_exit = true;
	if (!is_valid_exit_num(cmd->args->str))
	{
		ft_dprintf(2, "minishell: exit: %s: numeric argument required\n",
			cmd->args->str);
		shell->exit_status = 255;
		return (255);
	}
	if (cmd->args->next)
	{
		ft_dprintf(2, "minishell: exit: too many arguments\n");
		shell->exit_status = 1;
		*should_exit = false;
		return (1);
	}
	return (0);
}

static int	handle_exit_with_args(t_command *cmd, t_shell *shell)
{
	int		validation_result;
	bool	should_exit;
	int		exit_code;

	validation_result = validate_exit_args(cmd, shell, &should_exit);
	if (validation_result != 0)
	{
		if (should_exit)
		{
			shell->exit_status = validation_result;
			if (shell->state == INTERACTIVE)
				shell->status = 0;
			return (validation_result);
		}
		else
			return (validation_result);
	}
	exit_code = calc_exit(cmd->args->str);
	return (exit_code);
}

static void	set_shell_exit_state(t_shell *shell, int exit_code)
{
	shell->exit_status = exit_code;
	if (shell->state == INTERACTIVE)
		shell->status = 0;
}

int	exec_exit(t_command *cmd, t_shell *shell)
{
	int	exit_code;

	if (!cmd->args)
		exit_code = shell->exit_status;
	else
		exit_code = handle_exit_with_args(cmd, shell);
	set_shell_exit_state(shell, exit_code);
	return (exit_code);
}
