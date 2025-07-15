/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander_utils2.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 15:34:38 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/15 15:41:05 by ravi-bagin    ########   odam.nl         */
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
		t_env *env, t_exitstatus status)
{
	(*i)++;
	if (original[*i] == '?')
	{
		result = handle_exit_status(result, status);
		(*i)++;
	}
	else if (ft_isalnum(original[*i]) || original[*i] == '_')
		result = handle_variable_expansion(result, original, i, env);
	else
		result = handle_invalid_dollar(result);
	return (result);
}
