/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:50 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/08 15:17:32 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

static bool parse_assignment(const char *arg, char **key, char **value)
{
    char *equals_pos;
    int key_len;
    
    equals_pos = ft_strchr(arg, '=');
    if (!equals_pos)
    {
        // No '=' found, just export the variable name
        *key = ft_strdup(arg);
        *value = ft_strdup("");
        return (*key != NULL && *value != NULL);
    }
    
    key_len = equals_pos - arg;
    *key = ft_substr(arg, 0, key_len);
    *value = ft_strdup(equals_pos + 1);
    
    return (*key != NULL && *value != NULL);
}

static void print_export_vars(t_env *env_head)
{
    t_env *current = env_head;
    
    while (current)
    {
        if (current->key && current->value)
            printf("declare -x %s=\"%s\"\n", current->key, current->value);
        current = current->next;
    }
}

// Helper function to count arguments
static int count_export_args(t_token *args)
{
    int count = 0;
    t_token *current = args;
    
    while (current)
    {
        count++;
        current = current->next;
    }
    return (count);
}

int exec_export(t_command *cmd, t_env *env_list)
{
    t_token *current_arg;
    char *key;
    char *value;
    int exit_status = SUCCESS;
    
    if (!cmd || !env_list)
        return (ERROR_NULL_POINTER);
    
    // If no arguments, print all exported variables
    if (count_export_args(cmd->args) == 0)
    {
        print_export_vars(env_list);
        return (SUCCESS);
    }
    
    // Process each argument
    current_arg = cmd->args;
    while (current_arg)
    {
        if (!parse_assignment(current_arg->str, &key, &value))
        {
            ft_dprintf(2, "export: memory allocation error\n");
            exit_status = ERROR_MEMORY_ALLOCATION;
            break;
        }
        // Validate variable name
        if (!is_valid_var_syntax(key, false))
        {
            ft_dprintf(2, "export: `%s': not a valid identifier\n", current_arg->str);
            exit_status = FAILURE;
        }
        else
        {
            // Set the environment variable using existing helper
            if (update_env_var(env_list, key, value) != SUCCESS)
            {
                ft_dprintf(2, "export: memory allocation error\n");
                exit_status = ERROR_MEMORY_ALLOCATION;
            }
        }
        
        free(key);
        free(value);
        
        if (exit_status == ERROR_MEMORY_ALLOCATION)
            break;
        
        current_arg = current_arg->next;
    }
    
    return (exit_status);
}
