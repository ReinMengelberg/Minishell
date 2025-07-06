/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 15:11:17 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/06 11:38:32 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_unsupported_char(char c)
{
	return (c == ';' || c == '#' || c == '!' || \
		c == '&' || c == '\\');
}

static bool	is_unsupported_syntax(char *input, int i, char quote)
{
	if (quote != 0)
		return (false);
	if ((input[i] == '[' && input[i + 1] == '[') || \
		(input[i] == ']' && input[i + 1] == ']'))
		return (true);
	if (input[i] == '{' || input[i] == '}')
		return (true);
	if (input[i] == '(' || input[i] == ')')
		return (true);
	if ((input[i] == '(' && input[i + 1] == '(') || \
		(input[i] == ')' && input[i + 1] == ')'))
		return (true);
	return (false);
}

static bool	is_unsupported_expansion(char *input, int i, char quote)
{
	if (quote != 0)
		return (false);
	if (input[i] == '$' && input[i + 1] == '(' && input[i + 2] == '(')
		return (true);
	if (input[i] == '*')
		return (true);
	if (input[i] == '~' && (i == 0 || input[i - 1] == ' '))
		return (true);
	if (input[i] == '|' && input[i + 1] == '|')
		return (true);
	return (false);
}

static void	update_quote_status(char input_char, char *quote)
{
	if (*quote == input_char)
		*quote = 0;
	else
		*quote = input_char;
}

static bool	check_input_char(char *input, int i, char quote)
{
	if (quote == 0)
	{
		if (is_unsupported_char(input[i]))
			return (true);
		if (is_unsupported_syntax(input, i, quote))
			return (true);
		if (is_unsupported_expansion(input, i, quote))
			return (true);
	}
	return (false);
}

bool	check_input(char *input)
{
	int		i;
	char	quote;

	i = 0;
	quote = 0;
	while (input && input[i])
	{
		if ((input[i] == '\'' || input[i] == '"') && \
			(!quote || quote == input[i]))
			update_quote_status(input[i], &quote);
		else if (check_input_char(input, i, quote))
			return (true);
		i++;
	}
	if (quote != 0)
		return (true);
	return (false);
}

// Check for valid pipe syntax in the token list
static bool validate_pipe_syntax(t_token *tokens)
{
	t_token *current;
	bool command_before_pipe = false;
	bool command_after_pipe = false;
	t_token * next;

	current = tokens;
	if (current && current->type == PIPE)
		return false;
	while (current)
	{
		if (current->type == CMD)
			command_before_pipe = true;
		if (current->type == PIPE)
		{
			if (!command_before_pipe)
				return false;
			command_after_pipe = false;
			next = current->next;
			while (next)
			{
				if (next->type == CMD)
				{
					command_after_pipe = true;
					break;
				}
				else if (next->type == PIPE)
					return false;
				next = next->next;
			}
			if (!command_after_pipe)
				return false;
			command_before_pipe = false;
		}
		current = current->next;
	}
	return true;
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
	new->quote_state = NONE;
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
		return ;
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
	return (CMD);
}

static bool	handle_redirection_token(t_tokentype type,
			bool *next_redir_target)
{
	if (type == INPUT || type == OUTPUT || type == HEREDOC || type == APPEND)
	{
		*next_redir_target = true;
		return (true);
	}
	return (false);
}

static t_tokentype	handle_cmd_arg_token(t_tokentype type, bool *cmd_found,
					bool *after_redir_file)
{
	if (*after_redir_file && !*cmd_found && type == CMD)
	{
		*cmd_found = true;
		*after_redir_file = false;
		return (CMD);
	}
	if (type == CMD && *cmd_found)
		return (ARG);
	if (type == CMD)
		*cmd_found = true;
	return (type);
}

static void	reset_token_state(bool *cmd_found, bool *next_arg, bool *after_file)
{
	*cmd_found = false;
	*next_arg = false;
	*after_file = false;
}

static t_tokentype	process_token_type(t_tokentype type, bool *cmd_found,
					bool *next_redir_target, bool *after_redir_file)
{
	if (*next_redir_target)
	{
		*next_redir_target = false;
		*after_redir_file = true;
		return (ARG);
	}
	if (handle_redirection_token(type, next_redir_target))
		return (type);
	if (type == PIPE)
	{
		reset_token_state(cmd_found, next_redir_target, after_redir_file);
		return (type);
	}
	return (handle_cmd_arg_token(type, cmd_found, after_redir_file));
}

t_tokentype	set_token_type(char *token_str)
{
	t_tokentype	type;
	static bool	cmd_found = false;
	static bool	next_redir_target = false;
	static bool	after_redir_file = false;

	if (token_str == NULL)
	{
		reset_token_state(&cmd_found, &next_redir_target,
			&after_redir_file);
		return (EMPTY);
	}
	type = get_token_type(token_str);
	return (process_token_type(type, &cmd_found,
			&next_redir_target, &after_redir_file));
}

static t_token	*create_tokens_from_split(char **split, t_quote_state *quote_states)
{
	t_token		*tokens;
	t_tokentype	type;
	t_token		*token;
	int			i;

	tokens = NULL;
	i = 0;
	while (split[i])
	{
		type = set_token_type(split[i]);
		token = create_token(split[i], type);
		if (!token)
		{
			ft_free_array(split);
			free_tokens(tokens);
			return (NULL);
		}
		token->quote_state = quote_states[i];  // Set quote state
		add_token(&tokens, token);
		i++;
	}
	return (tokens);
}

t_token	*tokenize(char *input)
{
    t_token			*tokens;
    t_split_result	*split_result;

    set_token_type(NULL);
    if (check_input(input))
    {
        ft_putstr_fd("minishell: syntax error\n", 2);
        return (NULL);
    }
    
    split_result = ft_split_shell(input);
    if (!split_result)
        return (NULL);
    
    tokens = create_tokens_from_split(split_result->tokens, split_result->quote_states);
    
    ft_free_array(split_result->tokens);
    free(split_result->quote_states);
    free(split_result);
    
    if (tokens && !validate_pipe_syntax(tokens))
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
        free_tokens(tokens);
        return (NULL);
    }
    return (tokens);
}

// Debug function - can be removed for final version
void	print_tokens(t_token *tokens)
{
	t_token	*current;
	char	*type_names[10];

	type_names[0] = "HEREDOC";
	type_names[1] = "EMPTY";
	type_names[2] = "CMD";
	type_names[3] = "ARG";
	type_names[4] = "OUTPUT";
	type_names[5] = "APPEND";
	type_names[6] = "INPUT";
	type_names[7] = "PIPE";
	type_names[8] = "END";
	type_names[9] = "EXPANSION";
	current = tokens;
	while (current)
	{
		printf("Token: '%s', Type: %s\n", current->str,
			type_names[current->type]);
		current = current->next;
	}
}
