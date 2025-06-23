/*	Invalid input:
	\ Escape
	# Comment
	= Assignment
	[[ ]] Test
	; Command separator
	{ } Inline group
	( ) Subshell group
	(( )) Arithmetic expression
	$(( )) Arithmetic expansion
	*, ? Globs -- "wildcard"
	~ Home directory
	& Background */
// bool check_input(char *input)
// {
// 	int i;
// 	char quote;

// 	i = 0;
// 	quote = 0;
// 	while (input && input[i])
// 	{
// 		if ((input[i] == '\'' || input[i] == '"') && (!quote || quote == input[i]))
// 		{
// 			if (quote == input[i])
// 				quote = 0;
// 			else
// 				quote = input[i];
// 		}
// 		else if (quote == 0)
// 		{
// 			if (input[i] == ';' || input[i] == '#' || input[i] == '!' ||
// 				input[i] == '=' || input[i] == '&')
// 				return true;
// 			if (input[i] == '\\')
// 				return true;
// 			if (input[i] == '[' && input[i+1] == '[')
// 				return true;
// 			if (input[i] == ']' && input[i+1] == ']')
// 				return true;
// 			if (input[i] == '{' || input[i] == '}')
// 				return true;
// 			if (input[i] == '(' || input[i] == ')')
// 				return true;
// 			if (input[i] == '(' && input[i+1] == '(')
// 				return true;
// 			if (input[i] == ')' && input[i+1] == ')')
// 				return true;
// 			if (input[i] == '$' && input[i+1] == '(' && input[i+2] == '(')
// 				return true;
// 			if ((input[i] == '*' || (input[i] == '?' && input[i + 1] != '\0')) && quote == 0)
// 				return true;
// 			if (input[i] == '~' && (i == 0 || input[i-1] == ' '))
// 				return true;
// 			if (input[i] == '|' && input[i + 1] == '|')
// 				return true;
// 		}
// 		i++;
// 	}
// 	if (quote != 0)
// 		return true;
// 	return false;
// }

// t_tokentype	set_token_type(char *token_str)
// {
// 	t_tokentype	type;
// 	static bool	cmd_found = false;
// 	static bool	next_arg_is_redir_target = false;
// 	static bool	after_redir_file = false;

// 	if (token_str == NULL)
// 	{
// 		cmd_found = false;
// 		next_arg_is_redir_target = false;
// 		after_redir_file = false;
// 		return (EMPTY);
// 	}
// 	type = get_token_type(token_str);
// 	if (next_arg_is_redir_target)
// 	{
// 		next_arg_is_redir_target = false;
// 		after_redir_file = true;
// 		return (ARG);
// 	}
// 	if (type == INPUT || type == OUTPUT || type == HEREDOC || type == APPEND)
// 	{
// 		next_arg_is_redir_target = true;
// 		return (type);
// 	}
// 	if (type == PIPE)
// 	{
// 		cmd_found = false;
// 		next_arg_is_redir_target = false;
// 		after_redir_file = false;
// 		return (type);
// 	}
// 	if (after_redir_file && !cmd_found && type == CMD)
// 	{
// 		cmd_found = true;
// 		after_redir_file = false;
// 		return (CMD);
// 	}
// 	if (type == CMD && cmd_found)
// 		return (ARG);
// 	if (type == CMD)
// 		cmd_found = true;
// 	return (type);
// }

// t_token	*tokenize(char *input)
// {
// 	t_token		*tokens;
// 	char		**split;
// 	int			i;
// 	t_tokentype	type;
// 	t_token		*token;

// 	set_token_type(NULL);
// 	if (check_input(input))
// 	{
// 		ft_putstr_fd("minishell: syntax error\n", 2);
// 		return NULL;
// 	}
// 	tokens = NULL;
// 	split = ft_split_shell(input);
// 	if (!split)
// 		return (NULL);
// 	i = 0;
// 	while (split[i])
// 	{
// 		type = 	set_token_type(split[i]);
// 		token = create_token(split[i++], type);
// 		if (!token)
// 		{
// 			ft_free_array(split);
// 			free_tokens(tokens);
// 			return (NULL);
// 		}
// 		add_token(&tokens, token);
// 	}
// 	ft_free_array(split);
// 	return(tokens);
// }


/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 15:56:21 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/20 13:24:52 by rbagin        ########   odam.nl         */
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
	if (input[start] == '\'' || input[start] == '\"')
	{
		quote = input[start];
		end++;
		while (input[end] && input[end] != quote)
			end++;
		if (input[end] == quote)
			end++;
		*pos = end;
		token = ft_substr(input, start, end - start);
		result = remove_quotes(token);
		free(token);
		return result;
	}
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
		while (input[pos] && input[pos] == ' ')
			pos++;
		result[i] = extract_token(input, &pos);
		if(!result[i])
		{
			ft_free_array(result);
			return (NULL);
		}
			i++;
	}
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
