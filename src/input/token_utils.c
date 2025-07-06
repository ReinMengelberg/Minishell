/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 15:56:21 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/06 12:26:48 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_whitespace(char *input, int i)
{
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	return (i);
}

static int	handle_quoted_token(char *input, int i, char quote)
{
	i++;
	while (input[i] && input[i] != quote)
		i++;
	if (input[i])
		i++;
	return (i);
}

static int	handle_special_chars(char *input, int i)
{
	if (input[i] == '|' || input[i] == '<' || \
		(input[i] == '>' && input[i + 1] != '>'))
		return (i + 1);
	else if (input[i] == '>' && input[i + 1] == '>')
		return (i + 2);
	return (i);
}

static int	process_next_token(char *input, int *i)
{
	char	quote;

	if (input[*i] == '\'' || input[*i] == '\"')
	{
		quote = input[*i];
		*i = handle_quoted_token(input, *i, quote);
	}
	else if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
		*i = handle_special_chars(input, *i);
	else
	{
		while (input[*i] && input[*i] != ' ' && input[*i] != '|'
			&& input[*i] != '<' && input[*i] != '>'
			&& input[*i] != '\'' && input[*i] != '\"')
			(*i)++;
	}
	return (1);
}

static int	count_tokens(char *input)
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

static char	*handle_redirection(char *input, int *pos, int len)
{
	int		start;

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
		end++;
	}
	*pos = end;
	return (ft_substr(input, start, end - start));
}

static char	*handle_quoted_string_with_quotes(char *input, int *pos)
{
    int		start;
    int		end;
    char	quote;

    start = *pos;
    quote = input[start];
    end = start + 1;
    while (input[end] && input[end] != quote)
        end++;
    if (input[end] == quote)
        end++;
    *pos = end;
    return (ft_substr(input, start, end - start));
}

static char	*extract_token(char *input, int *pos)
{
    if (input[*pos] == '|'
        || (input[*pos] == '>' && input[*pos + 1] != '>')
        || (input[*pos] == '<' && input[*pos + 1] != '<'))
        return (handle_redirection(input, pos, 1));
    else if ((input[*pos] == '>' && input[*pos + 1] == '>')
        || (input[*pos] == '<' && input[*pos + 1] == '<'))
        return (handle_redirection(input, pos, 2));
    
    // Handle pure quoted strings (keep quotes)
    if (input[*pos] == '\'' || input[*pos] == '\"')
        return (handle_quoted_string_with_quotes(input, pos));
    
    // Handle regular tokens (including mixed quotes, keep quotes)
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

void	ft_free_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->str);
		free(tmp);
	}
}
