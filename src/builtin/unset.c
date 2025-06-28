/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   unset.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:57 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/28 13:37:38 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_unset(t_command *cmd, t_env *env_list)
{
    t_token *current_arg;
    int exit_status;

    if (!cmd || !env_list)
        return (ERROR_NULL_POINTER);
    if (!cmd->args)
        return (SUCCESS);
    
    exit_status = SUCCESS;
    current_arg = cmd->args;
    
    while (current_arg)
    {
        if (!is_valid_var_syntax(current_arg->str, false))
        {
            ft_dprintf(2, "unset: `%s': not a valid identifier\n", current_arg->str);
            exit_status = FAILURE;
        }
        else
        {
            // Remove the variable (ignore return value - unset succeeds even if var doesn't exist)
            remove_env_var(env_list, current_arg->str);
        }
        current_arg = current_arg->next;
    }
    return (exit_status);
}

