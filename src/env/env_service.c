/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_service.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/17 17:18:54 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/06/08 14:19:06 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// Print all environment variables

#include "minishell.h"

// Free the entire list
void	free_env(t_env *head)
{
	t_env	*current;
	t_env	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}

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

void	print_env(t_env *head)
{
	t_env	*current;

	current = head;
	if (!current)
	{
		printf("Environment is empty\n");
		return ;
	}
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

int update_env_var(t_env *env, const char *key, const char *value)
{
    t_env *current = env;
    
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
    
    // Add to front of list
    new_var->next = env->next;
    new_var->prev = env;
    if (env->next)
        env->next->prev = new_var;
    env->next = new_var;
    
    return (SUCCESS);
}
