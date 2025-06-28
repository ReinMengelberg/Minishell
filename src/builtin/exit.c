/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:48 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/28 11:22:46 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	isnum(char *str)
{
    int	i;
    if (!str || !*str)
        return (false);
    i = 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return (false);
        i++;
    }
    return (true);
}

int exec_exit(t_command *cmd, t_shell *shell)
{
    int exit_code = 0;
    char *exit_arg;
    
    if (!cmd->args)
        exit_code = shell->exit_status;
    else
    {
        exit_arg = cmd->args->str;
        
        // Check if the argument is a valid number
        if (!isnum(cmd->args->str))
        {
            printf("minishell: exit: %s: numeric argument required\n", exit_arg);
            return (1);
        }
        else if (cmd->args->next)
        {
            printf("minishell: exit: too many arguments\n");
            return (1);
        }
        else
            exit_code = ft_atoi(exit_arg);
    }
    printf("Exit code %d\n", exit_code);
    shell->exit_status = exit_code;
    printf("Exit status in exit %d\n", shell->exit_status);
    if (shell->state == INTERACTIVE)
        shell->status = 0;
    return (exit_code);
}
