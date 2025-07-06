/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:50 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/06 13:34:28 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"


static bool parse_export_arg(const char *arg, char **key, char **value, bool *has_equals)
{
    char *equals_pos;
    int key_len;
    
    *has_equals = false;
    equals_pos = ft_strchr(arg, '=');
    if (!equals_pos)
    {
        *key = ft_strdup(arg);
        *value = NULL;
        return (*key != NULL);
    }
    *has_equals = true;
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
    
    while (current && current->type == ARG)
    {
        count++;
        current = current->next;
    }
    return (count);
}

int exec_export(t_command *cmd, t_shell *shell)
{
    t_token *current_arg;
    char *key;
    char *value;
    bool has_equals;
    int exit_status;

    exit_status = SUCCESS;
    if (!cmd || !shell)
        return (ERROR_NULL_POINTER);
    if (count_export_args(cmd->args) == 0)
    {
        print_export_vars(shell->env);
        return (SUCCESS);
    }
    current_arg = cmd->args;
    while (current_arg && current_arg->type == ARG)
    {
        if (!parse_export_arg(current_arg->str, &key, &value, &has_equals))
        {
            ft_dprintf(2, "export: memory allocation error\n");
            exit_status = ERROR_MEMORY_ALLOCATION;
            break;
        }
        if (!is_valid_var_syntax(key, false))
        {
            ft_dprintf(2, "export: `%s': not a valid identifier\n", current_arg->str);
            exit_status = FAILURE;
        }
        else
        {
            if (has_equals)
            {
                if (update_env_var(&shell->env, key, value) != SUCCESS)
                {
                    ft_dprintf(2, "export: memory allocation error\n");
                    exit_status = ERROR_MEMORY_ALLOCATION;
                }
            }
        }
        free(key);
        if (value)
            free(value);
        if (exit_status == ERROR_MEMORY_ALLOCATION)
            break;
        current_arg = current_arg->next;
    }
    return (exit_status);
}
