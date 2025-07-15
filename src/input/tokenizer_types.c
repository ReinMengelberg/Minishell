/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_types.c                                  :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 16:03:00 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/15 15:57:54 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	handle_redirection_token(t_tokentype type, bool *next_redir_target)
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

t_tokentype	set_tokentype(char *token_str)
{
	t_tokentype		type;
	static bool		cmd_found = false;
	static bool		next_redir_target = false;
	static bool		after_redir_file = false;
	char			*unquoted_str;

	if (token_str == NULL)
	{
		reset_token_state(&cmd_found, &next_redir_target, &after_redir_file);
		return (EMPTY);
	}
	unquoted_str = remove_quotes(token_str);
	type = get_tokentype(unquoted_str);
	free(unquoted_str);
	return (process_token_type(type, &cmd_found,
			&next_redir_target, &after_redir_file));
}
