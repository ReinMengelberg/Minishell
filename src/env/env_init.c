/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_init.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/17 14:54:29 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/06/08 14:58:37 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*new_env_node(const char *key, const char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	node->value = ft_strdup(value);
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

// Add a node to the end of the list
void	env_add(t_env **head, t_env *new_node)
{
	t_env	*current;

	if (!*head)
	{
		*head = new_node;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_node;
	new_node->prev = current;
}

// Updates an exsisting key or sets a new one
void	env_set(t_env **head, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_node;
	current = *head;
	while (current)
	{
		if (strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = strdup(value);
			return ;
		}
		current = current->next;
	}
	new_node = new_env_node(key, value);
	if (new_node)
		env_add(head, new_node);
}

// Create a linked list of environment variables
t_env	*create_env(char **environ)
{
	t_env	*head;
	int		i;
	char	*env_copy;
	char	*key;
	char	*value;

	i = 0;
	head = NULL;
	while (environ[i] != NULL)
	{
		env_copy = ft_strdup(environ[i]);
		key = env_copy;
		value = ft_strchr(env_copy, '=');
		if (value)
		{
			*value = '\0';
			value++;
			env_set(&head, key, value);
		}
		free(env_copy);
		i++;
	}
	return (head);
}
