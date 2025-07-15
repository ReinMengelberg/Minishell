/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils2.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 15:55:00 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/15 15:52:05 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quoted_token(char *input, int i, char quote)
{
	i++;
	while (input[i] && input[i] != quote)
		i++;
	if (input[i])
		i++;
	return (i);
}

static char	*handle_redirection(char *input, int *pos, int len)
{
	int	start;

	start = *pos;
	*pos = start + len;
	return (ft_substr(input, start, len));
}

static char	*handle_regular_token(char *input, int *pos)
{
	int		start;
	int		end;
	char	quote;

	start = *pos;
	end = start;
	while (input[end] && input[end] != ' ' && input[end] != '\t'
		&& input[end] != '<' && input[end] != '>' && input[end] != '|')
	{
		if (input[end] == '\'' || input[end] == '\"')
		{
			quote = input[end];
			end++;
			while (input[end] && input[end] != quote)
				end++;
			if (input[end] == quote)
				end++;
		}
		else
			end++;
	}
	*pos = end;
	return (ft_substr(input, start, end - start));
}

char	*extract_token(char *input, int *pos)
{
	if (input[*pos] == '|'
		|| (input[*pos] == '>' && input[*pos + 1] != '>')
		|| (input[*pos] == '<' && input[*pos + 1] != '<'))
		return (handle_redirection(input, pos, 1));
	else if ((input[*pos] == '>' && input[*pos + 1] == '>')
		|| (input[*pos] == '<' && input[*pos + 1] == '<'))
		return (handle_redirection(input, pos, 2));
	return (handle_regular_token(input, pos));
}

char	**ft_split_shell(char *input)
{
	int		token_count;
	char	**result;
	int		i;
	int		pos;

	token_count = count_tokens(input);
	result = (char **)malloc(sizeof(char *) * (token_count + 1));
	if (!result)
		return (NULL);
	i = 0;
	pos = 0;
	while (i < token_count)
	{
		while (input[pos] && input[pos] == ' ')
			pos++;
		result[i] = extract_token(input, &pos);
		if (!result[i])
		{
			ft_free_array(result);
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}
