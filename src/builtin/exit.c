/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:48 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/23 17:00:58 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_exit(t_shell *shell, char **args)
{
    int exit_code = 0;
    
    // If no arguments provided, use the last exit status
    if (!args || !args[1])
    {
        exit_code = shell->exit_status;
    }
    else
    {
        // Parse the exit code from the first argument
        char *endptr;
        long temp = strtol(args[1], &endptr, 10);
        
        // Check if the argument is a valid number
        if (*endptr != '\0' || args[1][0] == '\0')
        {
            printf("minishell: exit: %s: numeric argument required\n", args[1]);
            exit_code = 2;  // Bash convention for invalid argument
        }
        // Check for too many arguments
        else if (args[2])
        {
            printf("minishell: exit: too many arguments\n");
            return (1);  // Don't exit, just return error status
        }
        else
        {
            // Bash uses modulo 256 for exit codes
            exit_code = (int)(temp % 256);
            if (exit_code < 0)
                exit_code += 256;
        }
    }
    
    printf("exit\n");
    shell->status = 0;  // This will terminate the main loop
    return (exit_code);
}