/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 15:11:17 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/05/11 15:34:19 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (TRUNC);
	if (ft_strcmp(str, ">>") == 0)
		return (APPEND);
	if (ft_strcmp(str, "<") == 0)
		return (INPUT);
	return (CMD); // Default to CMD, will adjust in tokenize function
}

t_token	*tokenize(char *input)
{
	t_token		*tokens;
	char		**split;
	int			i;
	bool		cmd_found;
	t_tokentype	type;

	tokens = NULL;
	split = ft_split_shell(input);
	if (!split)
		return (NULL);
	i = 0;
	cmd_found = false;
	while (split[i])
	{
        type = get_token_type(split[i]);
		if (type == PIPE || type == TRUNC || type == APPEND || type == INPUT)
            cmd_found = false;
		else if (type == CMD && cmd_found)
            type = ARG;
		else if (type == CMD)
            cmd_found = true;
		add_token(&tokens, create_token(split[i], type));
		i++;
	}
	i = 0;
	while (split[i])
		
}
