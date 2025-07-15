/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander_utils2.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 15:34:38 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/15 16:45:05 by rein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*handle_invalid_dollar(char *result)
{
	char	*temp;

	temp = ft_strjoin(result, "$");
	free(result);
	return (temp);
}

char	*process_dollar_expansion(char *result, char *original, int *i,
		t_expand_context *ctx)
{
	(*i)++;
	if (original[*i] == '?')
	{
		result = handle_exit_status(result, ctx->status);
		(*i)++;
	}
	else if (ft_isalnum(original[*i]) || original[*i] == '_')
		result = handle_variable_expansion(result, original, i, ctx->env);
	else
		result = handle_invalid_dollar(result);
	return (result);
}
