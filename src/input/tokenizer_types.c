/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_types.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 16:03:00 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/23 15:27:43 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_redirection_token(t_tokentype type, bool *next_redir_target)
{
	if (type == INPUT || type == OUTPUT || type == HEREDOC || type == APPEND)
	{
		*next_redir_target = true;
		return (true);
	}
	return (false);
}

t_tokentype	handle_cmd_arg_token(t_tokentype type, bool *cmd_found,
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

t_tokentype	process_token_type(t_tokentype type, bool *cmd_found,
		bool *next_redir_target, bool *after_redir_file)
{
	if (*next_redir_target)
	{
		*next_redir_target = false;
		*after_redir_file = true;
		return (FILENAME);
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

static t_tokentype	get_tokentype_unquoted(char *token_str)
{
	char		*unquoted_str;
	t_tokentype	type;

	unquoted_str = remove_quotes(token_str);
	type = get_tokentype(unquoted_str);
	free(unquoted_str);
	return (type);
}

t_tokentype	set_tokentype(char *token_str)
{
	t_tokentype		type;
	t_quotestate	quote_state;
	static bool		cmd_found = false;
	static bool		next_redir_target = false;
	static bool		after_redir_file = false;

	if (token_str == NULL)
	{
		reset_token_state(&cmd_found, &next_redir_target, &after_redir_file);
		return (EMPTY);
	}
	quote_state = set_quotestate(token_str);
	type = get_tokentype_unquoted(token_str);
	if ((quote_state == SINGLE || quote_state == DOUBLE)
		&& (type == PIPE || type == OUTPUT || type == APPEND
			|| type == INPUT || type == HEREDOC))
		type = CMD;
	return (process_token_type(type, &cmd_found,
			&next_redir_target, &after_redir_file));
}
