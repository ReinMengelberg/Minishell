/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/08 15:09:48 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/06/28 16:57:44 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_env(t_command *cmd, t_shell *shell)
{
    (void)cmd; // env doesn't use arguments in basic implementation
    
    if (!shell || !shell->env)
    {
        printf("Environment is empty\n");
        return (SUCCESS);
    }
    
    // Use your existing print_env function
    print_env(shell->env);
    
    return (SUCCESS);
}