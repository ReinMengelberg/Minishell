/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   env_utils.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/08 14:49:42 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/14 15:36:02 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_valid_var_syntax(const char *name, bool dollar)
{
	if (!name || !*name)
		return (false);
	if (dollar && *name != '$')
		return (false);
	else if (dollar)
		name++;
	if (!*name)
		return (false);
	if (*name == '?')
		return (true);
	if (!ft_isalpha(*name) && *name != '_')
		return (false);
	name++;
	while (*name && (ft_isalnum(*name) || *name == '_'))
		name++;
	return (*name == '\0');
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
