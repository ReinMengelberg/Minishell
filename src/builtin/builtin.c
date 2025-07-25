/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   builtin.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/08 13:46:21 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/14 13:54:51 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_builtin(t_command *cmd, t_shell *shell)
{
	if (!cmd || !cmd->cmd || !cmd->cmd->str)
		return (-1);
	if (strcmp(cmd->cmd->str, "exit") == 0)
		return (exec_exit(cmd, shell));
	else if (strcmp(cmd->cmd->str, "cd") == 0)
		return (exec_cd(cmd, shell));
	else if (strcmp(cmd->cmd->str, "pwd") == 0)
		return (exec_pwd(cmd, shell));
	else if (strcmp(cmd->cmd->str, "echo") == 0)
		return (exec_echo(cmd, shell));
	else if (strcmp(cmd->cmd->str, "env") == 0)
		return (exec_env(cmd, shell));
	else if (strcmp(cmd->cmd->str, "export") == 0)
		return (exec_export(cmd, shell));
	else if (strcmp(cmd->cmd->str, "unset") == 0)
		return (exec_unset(cmd, shell));
	return (1);
}

bool	is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	return (ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "echo") == 0
		|| ft_strcmp(cmd, "pwd") == 0
		|| ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0
		|| ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0);
}
