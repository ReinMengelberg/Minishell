/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   execute_builtin.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/08 13:46:21 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/06/08 14:22:34 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int execute_builtin(t_command *cmd, t_env *env_list)
{
    if (!cmd || !cmd->cmd || !cmd->cmd->str)
        return (-1);
        
    if (ft_strcmp(cmd->cmd->str, "cd") == 0)
        return execute_cd(cmd, env_list);
    else if (ft_strcmp(cmd->cmd->str, "pwd") == 0)
        return execute_pwd(cmd, env_list);
    // else if (ft_strcmp(cmd->cmd->str, "echo") == 0)
    //     return execute_echo(cmd, env_list);
    // else if (ft_strcmp(cmd->cmd->str, "export") == 0)
    //     return execute_export(cmd, env_list);
    // else if (ft_strcmp(cmd->cmd->str, "unset") == 0)
    //     return execute_unset(cmd, env_list);
    // else if (ft_strcmp(cmd->cmd->str, "env") == 0)
    //     return execute_env(cmd, env_list);
    // else if (ft_strcmp(cmd->cmd->str, "exit") == 0)
    //     return execute_exit(cmd, env_list);
    
    return (-1);
}
