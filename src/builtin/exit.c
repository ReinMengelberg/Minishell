/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:48 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/06 13:17:15 by rmengelb      ########   odam.nl         */
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

int calc_exit(char* exit_arg)
{
    int exit_code;

    exit_code = ft_atoi(exit_arg);
    exit_code = exit_code % 256;
    if (exit_code < 0)
        exit_code += 256;
    return (exit_code);
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
        if (!isnum(cmd->args->str))
            exit_code = 2;
        else if (cmd->args->next)
            return (printf("minishell: exit: too many arguments\n"), 1);
        else
            exit_code = calc_exit(exit_arg);
    }
    shell->exit_status = exit_code;
    if (shell->state == INTERACTIVE)
        shell->status = 0;
    return (exit_code);
}
