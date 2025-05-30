/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 15:11:17 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/05/27 12:20:57 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
bool check_input(char *input)
{
	int i;
	char quote;

	i = 0;
	quote = 0;
	while (input && input[i])
	{
		// Handle quote tracking
		if ((input[i] == '\'' || input[i] == '"') && (!quote || quote == input[i]))
		{
			if (quote == input[i])
				quote = 0;
			else
				quote = input[i];
		}
		else if (quote == 0)
		{
			// Check basic characters that aren't supported
			if (input[i] == ';' || input[i] == '#' || input[i] == '!' ||
				input[i] == '=' || input[i] == '&')
				return true;
			// Check backslash escape
			if (input[i] == '\\')
				return true;
			// Check special bracket combinations
			if (input[i] == '[' && input[i+1] == '[')
				return true;
			if (input[i] == ']' && input[i+1] == ']')
				return true;
			if (input[i] == '{' || input[i] == '}')
				return true;
			if (input[i] == '(' || input[i] == ')')
				return true;
			// Check for arithmetic expressions
			if (input[i] == '(' && input[i+1] == '(')
				return true;
			if (input[i] == ')' && input[i+1] == ')')
				return true;
			// Check for arithmetic expansion
			if (input[i] == '$' && input[i+1] == '(' && input[i+2] == '(')
				return true;
			// Check glob characters
			if ((input[i] == '*' || input[i] == '?') && quote == 0)
				return true;
			// Check tilde expansion
			if (input[i] == '~' && (i == 0 || input[i-1] == ' '))
				return true;
			if (input[i] == '|' && input[i + 1] == '|')
				return true;
		}
		i++;
	}
	// Check for unclosed quotes
	if (quote != 0)
		return true;
	return false;
}

t_token	*create_token(char *str, t_tokentype type)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	new->str = ft_strdup(str);
	if (!new->str)
	{
		free(new);
		return (NULL);
	}
	new->type = type;
	new->next = NULL;
	new->prev = NULL;
	return (new);
}

void	add_token(t_token **tokens, t_token *new)
{
	t_token	*current;

	if (!*tokens)
	{
		*tokens = new;
		return;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new;
	new->prev = current;
}

t_tokentype	get_token_type(char *str)
{
	if (!str || !*str)
		return (EMPTY);
	if (ft_strcmp(str, "|") == 0)
		return (PIPE);
	if (ft_strcmp(str, ">") == 0)
		return (OUTPUT);
	if (ft_strcmp(str, ">>") == 0)
		return (APPEND);
	if (ft_strcmp(str, "<") == 0)
		return (INPUT);
	if (ft_strcmp(str, "<<") == 0)
		return (HEREDOC);
	if (str[0] == '$' && str[1] != '\0')
		return (EXPANSION);
	return (CMD); // Default to CMD, will adjust in tokenize function
}

t_tokentype	set_token_type(char *token_str)
{
	t_tokentype	type;
	static bool	cmd_found = false;
	static bool	next_arg_is_redir_target = false;

	if (token_str == NULL)
	{
		cmd_found = false;
		next_arg_is_redir_target = false;
		return (EMPTY);
	}
	type = get_token_type(token_str);
	if (next_arg_is_redir_target && type == CMD)
	{
		next_arg_is_redir_target = false;
		return (ARG);
	}
	if (type == INPUT || type == OUTPUT || type == HEREDOC || type == APPEND)
	{
		next_arg_is_redir_target = true;
		return (type);
	}
	if (type == PIPE)
	{
		cmd_found = false;
		next_arg_is_redir_target = false;
		return (type);
	}
	if (type == CMD && cmd_found)
		return (ARG);
	if (type == CMD)
		cmd_found = true;
	return (type);
}

t_token	*tokenize(char *input)
{
	t_token		*tokens;
	char		**split;
	int			i;
	t_tokentype	type;
	t_token		*token;

	set_token_type(NULL);
	if (check_input(input))
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		return NULL;
	}
	tokens = NULL;
	split = ft_split_shell(input);
	if (!split)
		return (NULL);
	i = 0;
	while (split[i])
	{
		type = 	set_token_type(split[i]);
		token = create_token(split[i++], type);
		printf("%p\n str: %s\n", token, token->str);
		if (!token)
		{
			ft_free_array(split);
			free_tokens(tokens);
			return (NULL);
		}
		add_token(&tokens, token);
	}
	ft_free_array(split);
	return(tokens);
}

void print_tokens(t_token *tokens)
{
	t_token *current;
	char *type_names[] = {"EMPTY", "CMD", "ARG", "OUTPUT", "APPEND", "INPUT",
							"PIPE", "HEREDOC", "END", "EXPANSION"};

	current = tokens;
	while (current)
	{
		printf("Token: '%s', Type: %s\n", current->str, type_names[current->type]);
		current = current->next;
	}
}
