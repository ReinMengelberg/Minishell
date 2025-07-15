/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/24 15:16:33 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/15 15:42:02 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_regular_char(char *result, char c)
{
	char	temp_str[2];
	char	*temp;

	temp_str[0] = c;
	temp_str[1] = '\0';
	temp = ft_strjoin(result, temp_str);
	free(result);
	return (temp);
}

t_token	*expand_token(t_token *token, t_env *env, t_exitstatus status)
{
	char	*orig;
	char	*result;
	int		i;

	orig = token->str;
	result = ft_strdup("");
	i = 0;
	while (orig[i])
	{
		if (orig[i] == '$' && orig[i + 1] != '\0')
			result = process_dollar_expansion(result, orig, &i, env, status);
		else
		{
			result = handle_regular_char(result, orig[i]);
			i++;
		}
	}
	free(orig);
	token->str = result;
	return (token);
}

static bool	validate_expansion_in_token(t_token *token)
{
	char	*str;
	int		i;

	str = token->str;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1] != '\0')
		{
			i++;
			if (str[i] == '?')
				i++;
			else if (ft_isalnum(str[i]) || str[i] == '_')
			{
				if (!validate_variable_name(str, &i))
					return (false);
			}
			else
				i++;
		}
		else
			i++;
	}
	return (true);
}

static void	update_token_type(t_token *current, bool *expect_cmd)
{
	if (current->type == EXPANSION)
	{
		if (*expect_cmd)
			current->type = CMD;
		else
			current->type = ARG;
	}
	if (current->type == PIPE)
		*expect_cmd = true;
	else if (current->type == CMD || current->type == ARG)
		*expect_cmd = false;
	else if (current->type == INPUT || current->type == OUTPUT
		|| current->type == APPEND || current->type == HEREDOC)
		*expect_cmd = false;
}

t_token	*expand_tokens(t_token *token, t_env *env_head, t_exitstatus status)
{
	t_token	*current;
	bool	expect_cmd;

	if (!valid_expansions(token))
		return (NULL);
	current = token;
	expect_cmd = true;
	while (current != NULL)
	{
		if (current->type == EXPANSION && should_expand_variable(current))
		{
			current = expand_token(current, env_head, status);
			if (expect_cmd)
				current->type = CMD;
			else
				current->type = ARG;
		}
		update_token_type(current, &expect_cmd);
		current = current->next;
	}
	return (token);
}
