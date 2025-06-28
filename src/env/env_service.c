/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_service.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/17 17:18:54 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/06/28 16:49:12 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Find a value by key
char	*env_get(t_env *head, const char *key)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

int update_env_var(t_env **env_head, const char *key, const char *value)
{
    t_env *current = *env_head;
    
    // Search for existing variable
    while (current)
    {
        if (strcmp(current->key, key) == 0)
        {
            free(current->value);
            current->value = strdup(value);
            return (current->value ? SUCCESS : ERROR_MEMORY_ALLOCATION);
        }
        current = current->next;
    }
    
    // Variable doesn't exist, create new one
    t_env *new_var = malloc(sizeof(t_env));
    if (!new_var)
        return (ERROR_MEMORY_ALLOCATION);
    
    new_var->key = strdup(key);
    new_var->value = strdup(value);
    if (!new_var->key || !new_var->value)
    {
        free(new_var->key);
        free(new_var->value);
        free(new_var);
        return (ERROR_MEMORY_ALLOCATION);
    }
    
    new_var->next = NULL;
    new_var->prev = NULL;
    
    // Add to end of list (consistent with env_set)
    if (!*env_head)
    {
        *env_head = new_var;
    }
    else
    {
        current = *env_head;
        while (current->next)
            current = current->next;
        current->next = new_var;
        new_var->prev = current;
    }
    
    return (SUCCESS);
}

int remove_env_var(t_env **env_head, const char *key)
{
    t_env *current;
    
    if (!env_head || !*env_head || !key)
        return (ERROR_INVALID_INPUT);
        
    current = *env_head;
    while (current)
    {
        if (current->key && ft_strcmp(current->key, key) == 0)
        {
            // Update head pointer if we're removing the head
            if (current == *env_head)
            {
                *env_head = current->next;
                if (*env_head)  // If new head exists, clear its prev pointer
                    (*env_head)->prev = NULL;
            }
            
            // Update linked list pointers
            if (current->prev)
                current->prev->next = current->next;
            if (current->next)
                current->next->prev = current->prev;
                
            free(current->key);
            free(current->value);
            free(current);
            return (SUCCESS);
        }
        current = current->next;
    }
    return (ERROR_INVALID_INPUT);
}