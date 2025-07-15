/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   tokenizer_creation.c                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 16:02:00 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/15 16:02:00 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *str, t_tokentype type, t_quotestate quote)
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
	new->quotestate = quote;
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

void	handle_quote_char(char c, char *quote)
{
	if (*quote == c)
		*quote = 0;
	else
		*quote = c;
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	int		len;
	char	*result;
	char	quote;

	len = ft_strlen(str);
	result = malloc(len + 1);
	quote = 0;
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (i < len)
	{
		if ((str[i] == '\'' || str[i] == '\"')
			&& (!quote || quote == str[i]))
			handle_quote_char(str[i], &quote);
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}
