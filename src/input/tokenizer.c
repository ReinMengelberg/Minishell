/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 15:11:17 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/05/17 17:40:16 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_input(char *input)
{
	int i;
	char quote;

	i = 0;
	quote = 0;
	while (input && input[i])
	{
		if (input[i] == ';' || input[i] == '#')
			return true;
		if (input[i] == '\\')
			return true;
		if ((input[i] == '\'' || input[i] == '"') && (!quote || quote == input[i]))
		{
			if (quote == input[i])
				quote = 0;
			else
				quote = input[i];
		}
		i++;
	}
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
	return (CMD); // Default to CMD, will adjust in tokenize function
}

t_tokentype	set_token_type(char *token_str)
{
	t_tokentype	type;
	static bool	next_arg = false;

	type = get_token_type(token_str);
	if (type == CMD && next_arg)
	{
		type = ARG;
		next_arg = false;
	}
	if (type == CMD || type == INPUT || type == OUTPUT || type == HEREDOC || \
		type == APPEND)
		next_arg = true;
	else if ( type == PIPE)
		next_arg = false;
	return (type);
}

t_token	*tokenize(char *input)
{
	t_token		*tokens;
	char		**split;
	int			i;
	t_tokentype	type;

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
		add_token(&tokens, create_token(split[i++], type));
	}
	ft_free_array(split);
	return(tokens);
}
