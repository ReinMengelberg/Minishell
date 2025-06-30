/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/24 15:16:33 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/06/30 18:31:36 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	should_expand_variable(t_token *token)
{
    if (token->quote_state == SINGLE)
        return (false);
    return (ft_strchr(token->str, '$') != NULL);
}

t_token	*expand_token(t_token *token, t_env *env, t_exitstatus status)
{
    char	*original;
    char	*result;
    char	*var_name;
    char	*var_value;
    int		i;

    original = token->str;
    result = ft_strdup("");
    i = 0;

    while (original[i])
    {
        if (original[i] == '$' && original[i + 1] != '\0')
        {
            i++; // Skip $
            if (original[i] == '?')
            {
                var_value = ft_itoa(status);
                char *temp = ft_strjoin(result, var_value);
                free(result);
                free(var_value);
                result = temp;
                i++;
            }
            else if (ft_isalnum(original[i]) || original[i] == '_')
            {
                int start = i;
                while (original[i] && (ft_isalnum(original[i]) || original[i] == '_'))
                    i++;
                var_name = ft_substr(original, start, i - start);
                var_value = env_get(env, var_name);
                if (!var_value)
                    var_value = "";
                char *temp = ft_strjoin(result, var_value);
                free(result);
                free(var_name);
                result = temp;
            }
            else
            {
                // Invalid variable, keep the $
                char *temp = ft_strjoin(result, "$");
                free(result);
                result = temp;
            }
        }
        else
        {
            char temp_str[2] = {original[i], '\0'};
            char *temp = ft_strjoin(result, temp_str);
            free(result);
            result = temp;
            i++;
        }
    }
    free(original);
    token->str = result;
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
        if (current->type == EXPANSION && should_expand_variable(current))
        {
            current = expand_token(current, env_head, status);
            current->type = ARG;
        }
        else if (current->type == EXPANSION)
        {
            current->type = ARG;
        }
        current = current->next;
    }
    return (token);
}
