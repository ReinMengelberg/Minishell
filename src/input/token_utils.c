/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 15:56:21 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/05/27 12:16:43 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *remove_quotes(char *str)
{
	int i, j;
	int len = ft_strlen(str);
	char *result = malloc(len + 1);
	char quote = 0;

	if (!result)
		return NULL;
	i = 0;
	j = 0;
	while (i < len)
	{
		if ((str[i] == '\'' || str[i] == '\"') && (!quote || quote == str[i]))
		{
			if (!quote)
				quote = str[i];
			else
				quote = 0;
		}
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return result;
}

static int count_tokens(char *input)
{
	int count = 0;
	int i = 0;
	char quote = 0;

	while (input[i])
	{
		// Skip whitespace between tokens
		while (input[i] && (input[i] == ' ' || input[i] == '\t'))
			i++;
		if (!input[i])
			break;
		// Found start of a token
		count++;
		// Handle quoted strings as complete tokens
		if (input[i] == '\'' || input[i] == '\"')
		{
			quote = input[i++];
			while (input[i] && input[i] != quote)
				i++;
			if (input[i])
				i++;  // Skip the closing quote
		}
		// Handle special characters
		else if (input[i] == '|' || input[i] == '<' ||
				(input[i] == '>' && input[i + 1] != '>'))
		{
			i++;
		}
		else if (input[i] == '>' && input[i + 1] == '>')
		{
			i += 2;
		}
		// Handle regular tokens
		else
		{
			while (input[i] && input[i] != ' ' && input[i] != '|' &&
					input[i] != '<' && input[i] != '>' &&
					input[i] != '\'' && input[i] != '\"')
			{
				i++;
			}
		}
	}
	return count;
}

static char *extract_token(char *input, int *pos)
{
	int start = *pos;
	int end = start;
	char quote = 0;
	char *token;
	char *result;

	// Handle special characters
	if (input[start] == '|' ||
		(input[start] == '>' && input[start + 1] != '>') ||
		(input[start] == '<' && input[start + 1] != '<'))
	{
		*pos = start + 1;
		return ft_substr(input, start, 1);
	}
	else if ((input[start] == '>' && input[start + 1] == '>') ||
			(input[start] == '<' && input[start + 1] == '<' ))
	{
		*pos = start + 2;
		return ft_substr(input, start, 2);
	}
	// Handle quoted strings specially
	if (input[start] == '\'' || input[start] == '\"')
	{
		quote = input[start];
		end++;
		while (input[end] && input[end] != quote)
			end++;
		if (input[end] == quote)
			end++;  // Include the closing quote
		*pos = end;
		token = ft_substr(input, start, end - start);
		result = remove_quotes(token);
		free(token);
		return result;
	}
	// Handle regular tokens
	while (input[end] && input[end] != ' ' && input[end] != '|' &&
			input[end] != '<' && input[end] != '>' &&
			input[end] != '\'' && input[end] != '\"')
	{
		end++;
	}

	*pos = end;
	return ft_substr(input, start, end - start);
}

/*	This function needs special handling for:
	1. Spaces as delimiters (except within quotes)
	2. Quoted strings (both single and double quotes)
	3. Special characters like |, >, >>, <
	4. Escape characters*/
char	**ft_split_shell(char *input)
{
	int token_count = count_tokens(input);
	char **result = (char **)malloc(sizeof(char *) * (token_count + 1));
	int i = 0;
	int pos = 0;

	if (!result)
		return NULL;

	while (i < token_count)
	{
		// Skip spaces
		while (input[pos] && input[pos] == ' ')
			pos++;

		// Extract the next token
		result[i] = extract_token(input, &pos);
		if(!result[i])
		{
			ft_free_array(result);
			return (NULL);
		}
			i++;
	}

	// NULL-terminate the array
	result[i] = NULL;
	return result;
}

void	ft_free_array(char **arr)
{
	int i;

	if (!arr)
		return;
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
	t_token *tmp;

	while (tokens)
	{
		tmp = tokens;
		printf("%p\n str: %s\n", tokens, tokens->str);
		tokens = tokens->next;
		free(tmp->str);
		free(tmp);
	}
}
