/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_service.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/17 17:18:54 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/05/17 17:37:27 by rmengelb      ########   odam.nl         */
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
