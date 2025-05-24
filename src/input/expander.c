/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/24 15:16:33 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/05/24 17:22:00 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *expand_token(t_token *token, t_env *env, t_exitstatus status)
{
    char *value;
    char *expansion;
    char *original;
    
    original = token->str;
    value = original;
    if (*value == '$')
        value++;
    if (*value == '?')
        expansion = ft_itoa(status);
    else
        expansion = env_get(env, value);
    free(original);
    token->str = expansion;
    return token;
}

t_token *expand_tokens(t_token *token_head, t_env *env_head, t_exitstatus status)
{
    t_token *current;

    current = token_head;
    while (current != NULL)
    {
        if (current->type == EXPANSION)
        {
            current = expand_token(current, env_head, status);
            current->type = ARG;
        }
        current = current->next;
    }
    return (token_head);
}
