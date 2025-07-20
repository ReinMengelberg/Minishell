/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expand_variable.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/20 14:53:59 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/20 15:01:49 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*get_special_var_value(char *str, int *i, t_expand_context *ctx)
{
	char	*var_value;

	if (str[*i] == '?')
	{
		var_value = ft_itoa(ctx->status);
		(*i)++;
	}
	else if (str[*i] == '$')
	{
		var_value = ft_itoa(getpid());
		(*i)++;
	}
	else if (ft_isdigit(str[*i]))
	{
		(*i)++;
		var_value = ft_strdup("");
	}
	else
		var_value = NULL;
	return (var_value);
}

static char	*get_named_var_value(char *str, int *i, t_expand_context *ctx)
{
	char	*var_name;
	char	*var_value;
	char	*temp;
	int		end;

	end = *i;
	while (str[end] && (ft_isalnum(str[end]) || str[end] == '_'))
		end++;
	var_name = ft_substr(str, *i, end - *i);
	if (!var_name)
		return (NULL);
	temp = env_get(ctx->env, var_name);
	if (!temp)
		var_value = ft_strdup("");
	else
		var_value = ft_strdup(temp);
	free(var_name);
	*i = end;
	return (var_value);
}

static char	*create_expanded_string(char *str, char *var_value, int start,
		int i)
{
	char	*before;
	char	*after;
	char	*new_str;
	char	*temp;

	before = ft_substr(str, 0, start);
	after = ft_strdup(str + i);
	if (!before || !after)
	{
		free(before);
		free(after);
		return (NULL);
	}
	temp = ft_strjoin(before, var_value);
	free(before);
	if (!temp)
	{
		free(after);
		return (NULL);
	}
	new_str = ft_strjoin(temp, after);
	free(temp);
	free(after);
	return (new_str);
}

char	*expand_variable(char *str, int *i, t_expand_context *ctx)
{
	char	*var_value;
	char	*new_str;
	int		start;
	int		var_len;

	if (!str || !i || !ctx || str[*i] != '$')
		return (str);
	start = *i;
	(*i)++;
	var_value = get_special_var_value(str, i, ctx);
	if (!var_value && (ft_isalpha(str[*i]) || str[*i] == '_'))
		var_value = get_named_var_value(str, i, ctx);
	if (!var_value)
		return (str);
	var_len = ft_strlen(var_value);
	new_str = create_expanded_string(str, var_value, start, *i);
	free(var_value);
	if (!new_str)
		return (str);
	*i = start + var_len;
	free(str);
	return (new_str);
}
