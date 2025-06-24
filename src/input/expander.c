/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/24 15:16:33 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/06/24 12:20:22 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*expand_token(t_token *token, t_env *env, t_exitstatus status)
{
	char	*value;
	char	*expansion;
	char	*original;

	original = token->str;
	value = original;
	if (*value == '$')
		value++;
	if (*value == '?')
		expansion = ft_itoa(status);
	else
	{
		expansion = env_get(env, value);
		if (!expansion)
			expansion = ft_strdup("");
	}
	free(original);
	token->str = expansion;
	return (token);
}

bool	valid_expansions(t_token *token_head)
{
	t_token	*current;

	if (!token_head)
		return (true);
	current = token_head;
	while (current != NULL)
	{
		if (current->type == EXPANSION)
		{
			if (!is_valid_var_syntax(current->str, true))
			{
				ft_dprintf(2, "minishell: invalid variable: %s\n",
					current->str);
				return (false);
			}
		}
		current = current->next;
	}
	return (true);
}

t_token	*expand_tokens(t_token *token, t_env *env_head, t_exitstatus status)
{
	t_token	*current;

	if (!valid_expansions(token))
		return (NULL);
	current = token;
	while (current != NULL)
	{
		if (current->type == EXPANSION)
		{
			current = expand_token(current, env_head, status);
			current->type = ARG;
		}
		current = current->next;
	}
	return (token);
}
