/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_service.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/17 17:18:54 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/14 15:45:32 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*env_get(t_env *head, const char *key)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static t_env	*create_new_env_var(const char *key, const char *value)
{
	t_env	*new_var;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	if (!new_var->key || !new_var->value)
	{
		free(new_var->key);
		free(new_var->value);
		free(new_var);
		return (NULL);
	}
	new_var->next = NULL;
	new_var->prev = NULL;
	return (new_var);
}

static int	find_and_update_var(t_env *head, const char *key, const char *value)
{
	t_env	*current;

	current = head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			if (!current->value)
				return (ERROR_MEMORY_ALLOCATION);
			return (SUCCESS);
		}
		current = current->next;
	}
	return (-1);
}

int	update_env_var(t_env **env_head, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_var;
	int		result;

	result = find_and_update_var(*env_head, key, value);
	if (result != -1)
		return (result);
	new_var = create_new_env_var(key, value);
	if (!new_var)
		return (ERROR_MEMORY_ALLOCATION);
	if (!*env_head)
		*env_head = new_var;
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

int	remove_env_var(t_env **env_head, const char *key)
{
	t_env	*current;

	current = *env_head;
	while (current)
	{
		if (current->key && ft_strcmp(current->key, key) == 0)
		{
			if (current == *env_head)
			{
				*env_head = current->next;
				if (*env_head)
					(*env_head)->prev = NULL;
			}
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
