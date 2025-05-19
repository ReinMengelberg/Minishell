/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 15:56:21 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/05/18 13:26:55 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


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
		// Process this token
		while (input[i] && (quote || input[i] != ' '))
		{
			// Handle quotes
			if ((input[i] == '\'' || input[i] == '\"') && (!quote || quote == input[i]))
				quote = (quote == input[i]) ? 0 : input[i];
			// Special character handling (if not in quotes)
			else if (!quote && (input[i] == '|' || input[i] == '<' || input[i] == '>'))
			{
				// Handle >> as one token
				if (input[i] == '>' && input[i + 1] == '>')
					i++;
				// If we're in the middle of a word, this special char starts a new token
				if (i > 0 && input[i - 1] != ' ' && input[i - 1] != '|' &&
					input[i - 1] != '<' && input[i - 1] != '>')
					count++;
				// If there's more text after this special char, it will be another token
				if (input[i + 1] && input[i + 1] != ' ' && input[i + 1] != '|' &&
					input[i + 1] != '<' && input[i + 1] != '>')
					count++;
			}
			i++;
		}
	}
	return (count);
}

static char *extract_token(char *input, int *pos)
{
	int start = *pos;
	int end = start;
	char quote = 0;
	char *token;

	// Handle special characters
	if (input[start] == '|' || input[start] == '<' ||
		(input[start] == '>' && input[start + 1] != '>'))
	{
		*pos = start + 1;
		token = ft_substr(input, start, 1);
		return token;
	}
	else if (input[start] == '>' && input[start + 1] == '>')
	{
		*pos = start + 2;
		token = ft_substr(input, start, 2);
		return token;
	}
	// Handle regular tokens and quoted strings
	while (input[end] && (quote || (input[end] != ' ' &&
			input[end] != '|' && input[end] != '<' && input[end] != '>')))
	{
		if (input[end] == '\'' || input[end] == '\"')
		{
			if (!quote)
				quote = input[end];
			else if (quote == input[end])
				quote = 0;
		}
		end++;
	}
	*pos = end;
	token = ft_substr(input, start, end - start);
	return token;
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
		tokens = tokens->next;
		free(tmp->str);
		free(tmp);
	}
}
/*	Testing only*/
void print_tokens(t_token *tokens)
{
	t_token *current;
	char *type_names[] = {"EMPTY", "CMD", "ARG", "OUTPUT", "APPEND", "INPUT", "PIPE", "END"};

	current = tokens;
	while (current)
	{
		printf("Token: '%s', Type: %s\n", current->str, type_names[current->type]);
		current = current->next;
	}
}
