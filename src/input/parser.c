/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/20 14:28:09 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/15 16:55:42 by rein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_token(t_command *cmd, t_token *token)
{
	if (token->type == CMD)
		cmd->cmd = token;
	else if (token->type == ARG)
		add_to_args(cmd, token);
	else if (token->type == INPUT || token->type == HEREDOC)
	{
		cmd->input = token;
		add_to_input_list(cmd, token);
	}
	else if (token->type == OUTPUT || token->type == APPEND)
	{
		cmd->output = token;
		add_to_output_list(cmd, token);
	}
}

t_command	*extract_commands(t_token *tokens)
{
	t_command	*cmd_head;
	t_command	*current_cmd;
	t_token		*current;

	cmd_head = NULL;
	current_cmd = NULL;
	current = tokens;
	while (current)
	{
		if (!current_cmd || current->type == PIPE)
		{
			if (current->type == PIPE)
				current = current->next;
			if (!current || !create_new_command_node(&cmd_head, &current_cmd))
				return (NULL);
		}
		process_token(current_cmd, current);
		current = current->next;
	}
	return (cmd_head);
}

static void	free_arg_array(char **result, int index)
{
	int	i;

	i = 0;
	while (i < index)
		free(result[i++]);
	free(result);
}

static bool	add_args_to_array(char **result, t_token *args, int start_idx)
{
	t_token	*arg;
	int		i;

	i = start_idx;
	arg = args;
	while (arg && arg->type == ARG)
	{
		result[i] = ft_strdup(arg->str);
		if (!result[i])
		{
			free_arg_array(result, i);
			return (false);
		}
		i++;
		arg = arg->next;
	}
	result[i] = NULL;
	return (true);
}

char	**tokens_to_args(t_token *cmd, t_token *args)
{
	int		count;
	char	**result;
	int		i;

	count = 1 + count_args(args);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	if (cmd)
	{
		result[i] = ft_strdup(cmd->str);
		if (!result[i])
			return (free(result), NULL);
		i++;
	}
	if (!add_args_to_array(result, args, i))
		return (NULL);
	return (result);
}
