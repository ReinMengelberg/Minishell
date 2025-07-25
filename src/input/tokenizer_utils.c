/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_utils.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 16:00:00 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/23 12:29:02 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_token_state(bool *cmd_found, bool *next_arg, bool *after_file)
{
	*cmd_found = false;
	*next_arg = false;
	*after_file = false;
}

bool	is_unsupported_char(char c)
{
	return (c == ';' || c == '#' || c == '!' || c == '&' || c == '\\');
}

bool	is_unsupported_syntax(char *input, int i, char quote)
{
	if (quote != 0)
		return (false);
	if ((input[i] == '[' && input[i + 1] == '[')
		|| (input[i] == ']' && input[i + 1] == ']'))
		return (true);
	if (input[i] == '{' || input[i] == '}')
		return (true);
	if (input[i] == '(' || input[i] == ')')
		return (true);
	if ((input[i] == '(' && input[i + 1] == '(')
		|| (input[i] == ')' && input[i + 1] == ')'))
		return (true);
	return (false);
}

bool	is_unsupported_expansion(char *input, int i, char quote)
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

void	update_quote_status(char input_char, char *quote)
{
	if (*quote == input_char)
		*quote = 0;
	else
		*quote = input_char;
}
