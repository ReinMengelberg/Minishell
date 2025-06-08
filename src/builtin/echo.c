/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   echo.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:45 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/08 15:15:49 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to check if a string is exactly "-n"
static bool is_n_flag(const char *str)
{
    return (str && strcmp(str, "-n") == 0);
}

int exec_echo(t_command *cmd, t_env *env_list)
{
    t_token *current_arg;
    bool newline = true;
    bool first_arg = true;
    
    (void)env_list; // echo doesn't need environment variables directly
    
    if (!cmd)
        return (ERROR_NULL_POINTER);
    
    current_arg = cmd->args;
    
    // Check for -n flag (suppress trailing newline)
    if (current_arg && is_n_flag(current_arg->str))
    {
        newline = false;
        current_arg = current_arg->next;
    }
    
    // Print all arguments separated by spaces
    while (current_arg)
    {
        if (!first_arg)
            printf(" ");
        
        printf("%s", current_arg->str);
        first_arg = false;
        current_arg = current_arg->next;
    }
    
    // Add newline unless -n flag was used
    if (newline)
        printf("\n");
    
    return (SUCCESS);
}

