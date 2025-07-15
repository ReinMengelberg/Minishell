/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 15:11:17 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/15 15:56:23 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		if ((input[i] == '\'' || input[i] == '"')
			&& (!quote || quote == input[i]))
			update_quote_status(input[i], &quote);
		else if (check_input_char(input, i, quote))
			return (true);
		i++;
	}
	if (quote != 0)
		return (true);
	return (false);
}

t_token	*create_tokens_from_split(char **strings)
{
	t_token	*tokens;
	t_token	*token;
	int		i;
	char	*unquoted_str;

	tokens = NULL;
	i = 0;
	while (strings[i])
	{
		unquoted_str = remove_quotes(strings[i]);
		token = create_token(unquoted_str, set_tokentype(strings[i]),
				set_quotestate(strings[i]));
		free(unquoted_str);
		if (!token)
		{
			ft_free_array(strings);
			free_tokens(tokens);
			return (NULL);
		}
		add_token(&tokens, token);
		i++;
	}
	return (tokens);
}

t_token	*tokenize(char *input)
{
	t_token	*tokens;
	char	**token_strings;

	set_tokentype(NULL);
	if (check_input(input))
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		return (NULL);
	}
	token_strings = ft_split_shell(input);
	if (!token_strings)
		return (NULL);
	tokens = create_tokens_from_split(token_strings);
	ft_free_array(token_strings);
	if (tokens && !validate_pipe_syntax(tokens))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
		free_tokens(tokens);
		return (NULL);
	}
	return (tokens);
}
