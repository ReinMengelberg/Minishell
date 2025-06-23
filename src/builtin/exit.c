/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:48 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/23 17:26:57 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_exit(t_command *cmd, t_shell *shell)
{
    int exit_code = 0;
    t_token *args = cmd->args;  // This is a token linked list
    
    // If no arguments provided, use the last exit status
    if (!args)
    {
        exit_code = shell->exit_status;
    }
    else
    {
        // Get the first argument (assuming it's the exit code)
        char *exit_arg = args->str;  // Assuming t_token has a str field
        
        // Parse the exit code from the first argument
        char *endptr;
        long temp = strtol(exit_arg, &endptr, 10);
        
        // Check if the argument is a valid number
        if (*endptr != '\0' || exit_arg[0] == '\0')
        {
            printf("minishell: exit: %s: numeric argument required\n", exit_arg);
            exit_code = 2;  // Bash convention for invalid argument
        }
        // Check for too many arguments (if there's a second token)
        else if (args->next)
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
    shell->exit_status = exit_code;  // Store the exit code
    return (exit_code);
}