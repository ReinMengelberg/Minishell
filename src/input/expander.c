/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   expander.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/24 15:16:33 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/14 18:10:31 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	should_expand_variable(t_token *token)
{
    if (token->quotestate == SINGLE)
        return (false);
    return (ft_strchr(token->str, '$') != NULL);
}

t_token	*expand_token(t_token *token, t_env *env, t_exitstatus status)
{
    char	*original;
    char	*result;
    char	*var_name;
    char	*var_value;
    int		i;

    original = token->str;
    result = ft_strdup("");
    i = 0;

    while (original[i])
    {
        if (original[i] == '$' && original[i + 1] != '\0')
        {
            i++; // Skip $
            if (original[i] == '?')
            {
                var_value = ft_itoa(status);
                char *temp = ft_strjoin(result, var_value);
                free(result);
                free(var_value);
                result = temp;
                i++;
            }
            else if (ft_isalnum(original[i]) || original[i] == '_')
            {
                int start = i;
                while (original[i] && (ft_isalnum(original[i]) || original[i] == '_'))
                    i++;
                var_name = ft_substr(original, start, i - start);
                var_value = env_get(env, var_name);
                if (!var_value)
                    var_value = "";
                char *temp = ft_strjoin(result, var_value);
                free(result);
                free(var_name);
                result = temp;
            }
            else
            {
                // Invalid variable, keep the $
                char *temp = ft_strjoin(result, "$");
                free(result);
                result = temp;
            }
        }
        else
        {
            char temp_str[2] = {original[i], '\0'};
            char *temp = ft_strjoin(result, temp_str);
            free(result);
            result = temp;
            i++;
        }
    }
    free(original);
    token->str = result;
    return (token);
}

bool	valid_expansions(t_token *token_head)
{
    t_token	*current;
    char	*str;
    int		i;

    if (!token_head)
        return (true);
    current = token_head;
    while (current != NULL)
    {
        if (current->type == EXPANSION && should_expand_variable(current))
        {
            str = current->str;
            i = 0;
            while (str[i])
            {
                if (str[i] == '$' && str[i + 1] != '\0')
                {
                    i++; // Skip $
                    if (str[i] == '?')
                    {
                        i++; // Valid special variable
                    }
                    else if (ft_isalnum(str[i]) || str[i] == '_')
                    {
                        int start = i;
                        while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                            i++;
                        char *var_name = ft_substr(str, start, i - start);
                        if (!is_valid_var_syntax(var_name, false))
                        {
                            ft_dprintf(2, "minishell: invalid variable: %s\n", var_name);
                            free(var_name);
                            return (false);
                        }
                        free(var_name);
                    }
                    else
                    {
                        // Skip invalid $ followed by non-variable characters
                        i++;
                    }
                }
                else
                {
                    i++;
                }
            }
        }
        current = current->next;
    }
    return (true);
}

t_token	*expand_tokens(t_token *token, t_env *env_head, t_exitstatus status)
{
    t_token	*current;
    bool	expect_cmd;

    if (!valid_expansions(token))
        return (NULL);
    current = token;
    expect_cmd = true; // First token or token after pipe/redirect is typically CMD
    while (current != NULL)
    {
        if (current->type == EXPANSION && should_expand_variable(current))
        {
            current = expand_token(current, env_head, status);
            // Determine if this should be CMD or ARG based on context
            if (expect_cmd)
                current->type = CMD;
            else
                current->type = ARG;
        }
        else if (current->type == EXPANSION)
        {
            if (expect_cmd)
                current->type = CMD;
            else
                current->type = ARG;
        }
        
        // Update expect_cmd based on current token type
        if (current->type == PIPE)
            expect_cmd = true; // Next token should be CMD
        else if (current->type == CMD || current->type == ARG)
            expect_cmd = false; // Subsequent tokens are typically ARGs
        else if (current->type == INPUT || current->type == OUTPUT || 
                 current->type == APPEND || current->type == HEREDOC)
            expect_cmd = false; // Next token is a filename, not a command
        current = current->next;
    }
    return (token);
}
