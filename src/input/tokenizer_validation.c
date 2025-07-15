/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_validation.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 16:01:00 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/15 16:01:00 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	check_command_after_pipe(t_token *pipe_token)
{
	t_token	*next;

	next = pipe_token->next;
	while (next)
	{
		if (next->type == CMD)
			return (true);
		else if (next->type == PIPE)
			return (false);
		next = next->next;
	}
	return (false);
}

bool	validate_pipe_syntax(t_token *tokens)
{
	t_token	*current;
	bool	command_before_pipe;

	current = tokens;
	command_before_pipe = false;
	if (current && current->type == PIPE)
		return (false);
	while (current)
	{
		if (current->type == CMD)
			command_before_pipe = true;
		if (current->type == PIPE)
		{
			if (!command_before_pipe)
				return (false);
			if (!check_command_after_pipe(current))
				return (false);
			command_before_pipe = false;
		}
		current = current->next;
	}
	return (true);
}

bool	is_expansion(char *str)
{
	int	i;

	if (str[0] == '$' && str[1] != '\0')
	{
		if (str[1] == '?')
			return (true);
		if (ft_isalpha(str[1]) || str[1] == '_')
		{
			i = 2;
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
			if (str[i] == '\0')
				return (true);
		}
		if (str[1] == '\0')
			return (false);
	}
	return (false);
}

t_quotestate	set_quotestate(char *string)
{
	int	len;

	if (!string || !*string)
		return (NONE);
	len = ft_strlen(string);
	if (len >= 2 && string[0] == '\'' && string[len - 1] == '\'')
		return (SINGLE);
	if (len >= 2 && string[0] == '"' && string[len - 1] == '"')
		return (DOUBLE);
	return (NONE);
}

t_tokentype	get_tokentype(char *str)
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
	if (ft_strchr(str, '$') != NULL)
		return (EXPANSION);
	return (CMD);
}
