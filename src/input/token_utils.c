/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 15:56:21 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/30 18:47:59 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_quote_char(char c, char *quote)
{
	if (*quote == c)
		*quote = 0;
	else
		*quote = c;
}

static char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	int		len;
	char	*result;
	char	quote;

	len = ft_strlen(str);
	result = malloc(len + 1);
	quote = 0;
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if ((str[i] == '\'' || str[i] == '\"') && (!quote || quote == str[i]))
			handle_quote_char(str[i], &quote);
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

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

static char	*handle_quoted_string(char *input, int *pos, t_quote_state *quote_state)
{
    int		start;
    int		end;
    char	quote;
    char	*token;
    char	*result;

    start = *pos;
    quote = input[start];
    *quote_state = (quote == '\'') ? SINGLE : DOUBLE;
    end = start + 1;
    while (input[end] && input[end] != quote)
        end++;
    if (input[end] == quote)
        end++;
    *pos = end;
    token = ft_substr(input, start, end - start);
    result = remove_quotes(token);
    free(token);
    return (result);
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

static char	*extract_token(char *input, int *pos, t_quote_state *quote_state)
{
    char *token;
    char *result;

    *quote_state = NONE;
    
    if (input[*pos] == '|'
        || (input[*pos] == '>' && input[*pos + 1] != '>')
        || (input[*pos] == '<' && input[*pos + 1] != '<'))
        return (handle_redirection(input, pos, 1));
    else if ((input[*pos] == '>' && input[*pos + 1] == '>')
        || (input[*pos] == '<' && input[*pos + 1] == '<'))
        return (handle_redirection(input, pos, 2));
    
    // Handle pure quoted strings
    if (input[*pos] == '\'' || input[*pos] == '\"')
        return (handle_quoted_string(input, pos, quote_state));
    
    // Handle regular tokens (including mixed quotes)
    token = handle_regular_token(input, pos);
    if (token && (ft_strchr(token, '\'') || ft_strchr(token, '"')))
    {
        // For tokens with quotes, remove quotes but keep quote_state as NONE
        // This allows expansion for mixed quotes like hello'world'
        *quote_state = NONE;
        result = remove_quotes(token);
        free(token);
        return (result);
    }
    return (token);
}

t_split_result	*ft_split_shell(char *input)
{
    t_split_result	*result_struct;
    int				token_count;
    char			**result;
    t_quote_state	*quote_states;
    int				i;
    int				pos;

    token_count = count_tokens(input);
    result_struct = malloc(sizeof(t_split_result));
    result = (char **)malloc(sizeof(char *) * (token_count + 1));
    quote_states = (t_quote_state *)malloc(sizeof(t_quote_state) * token_count);
    
    if (!result_struct || !result || !quote_states)
        return (free(result_struct), free(result), free(quote_states), NULL);
    
    i = 0;
    pos = 0;
    while (i < token_count)
    {
        while (input[pos] && input[pos] == ' ')
            pos++;
        result[i] = extract_token(input, &pos, &quote_states[i]);
        if (!result[i])
        {
            ft_free_array(result);
            free(quote_states);
            free(result_struct);
            return (NULL);
        }
        i++;
    }
    result[i] = NULL;
    
    result_struct->tokens = result;
    result_struct->quote_states = quote_states;
    result_struct->count = token_count;
    return (result_struct);
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
