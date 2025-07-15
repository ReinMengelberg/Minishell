/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander_utils.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 15:29:21 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/15 15:34:13 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	valid_expansions(t_token *token_head)
{
	t_token	*current;

	if (!token_head)
		return (true);
	current = token_head;
	while (current != NULL)
	{
		if (current->type == EXPANSION && should_expand_variable(current))
		{
			if (!validate_expansion_in_token(current))
				return (false);
		}
		current = current->next;
	}
	return (true);
}

bool	validate_variable_name(char *str, int *i)
{
	char	*var_name;
	int		start;
	bool	is_valid;

	start = *i;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	var_name = ft_substr(str, start, *i - start);
	is_valid = is_valid_var_syntax(var_name, false);
	if (!is_valid)
		ft_dprintf(2, "minishell: invalid variable: %s\n", var_name);
	free(var_name);
	return (is_valid);
}

bool	should_expand_variable(t_token *token)
{
	if (token->quotestate == SINGLE)
		return (false);
	return (ft_strchr(token->str, '$') != NULL);
}

char	*handle_exit_status(char *result, t_exitstatus status)
{
	char	*var_value;
	char	*temp;

	var_value = ft_itoa(status);
	temp = ft_strjoin(result, var_value);
	free(result);
	free(var_value);
	return (temp);
}

char	*handle_variable_expansion(char *result, char *original,
		int *i, t_env *env)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		start;

	start = *i;
	while (original[*i] && (ft_isalnum(original[*i]) || original[*i] == '_'))
		(*i)++;
	var_name = ft_substr(original, start, *i - start);
	var_value = env_get(env, var_name);
	if (!var_value)
		var_value = "";
	temp = ft_strjoin(result, var_value);
	free(result);
	free(var_name);
	return (temp);
}
