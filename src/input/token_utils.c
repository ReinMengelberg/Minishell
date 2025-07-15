/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 15:56:21 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/15 15:53:15 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_whitespace(char *input, int i)
{
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	return (i);
}

static int	handle_special_chars(char *input, int i)
{
	if (input[i] == '|' || input[i] == '<'
		|| (input[i] == '>' && input[i + 1] != '>'))
		return (i + 1);
	else if (input[i] == '>' && input[i + 1] == '>')
		return (i + 2);
	return (i);
}

static int	process_token_content(char *input, int *i)
{
	char	quote;

	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
	{
		if (input[*i] == '\'' || input[*i] == '\"')
		{
			quote = input[*i];
			*i = handle_quoted_token(input, *i, quote);
		}
		else
		{
			while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
				&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>'
				&& input[*i] != '\'' && input[*i] != '\"')
				(*i)++;
		}
	}
	return (1);
}

static int	process_next_token(char *input, int *i)
{
	if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
	{
		*i = handle_special_chars(input, *i);
		return (1);
	}
	return (process_token_content(input, i));
}

int	count_tokens(char *input)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (input[i])
	{
		i = skip_whitespace(input, i);
		if (!input[i])
			break ;
		count += process_next_token(input, &i);
	}
	return (count);
}
