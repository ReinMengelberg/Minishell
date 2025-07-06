/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/20 14:28:09 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/06 16:06:44 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_command	*create_new_command_node(t_command **cmd_head,
										t_command **current_cmd)
{
	t_command	*new_cmd;

	new_cmd = create_command();
	if (!new_cmd)
	{
		if (*cmd_head)
			free_commands(*cmd_head);
		return (NULL);
	}
	if (!*cmd_head)
		*cmd_head = new_cmd;
	else
	{
		(*current_cmd)->next = new_cmd;
		(*current_cmd)->is_piped = true;
	}
	*current_cmd = new_cmd;
	(*current_cmd)->in_fd = STDIN_FILENO;
	(*current_cmd)->out_fd = STDOUT_FILENO;
	return (new_cmd);
}

static void	process_token_type(t_command *cmd, t_token *token)
{
	if (token->type == CMD)
		cmd->cmd = token;
	else if (token->type == ARG)
		add_to_args(cmd, token);
	else if (token->type == INPUT || token->type == HEREDOC)
		cmd->input = token;
	else if (token->type == OUTPUT || token->type == APPEND)
		cmd->output = token;
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
		process_token_type(current_cmd, current);
		current = current->next;
	}
	return (cmd_head);
}

// t_command	*extract_commands(t_token *tokens)
// {
// 	t_command	*cmd_head;
// 	t_command	*current_cmd;
// 	t_command	*cmd;
// 	t_token		*current;
// 	t_token		*default_cmd;

// 	cmd_head = NULL;
// 	current_cmd = NULL;
// 	current = tokens;
// 	while (current)
// 	{
// 		if (!current_cmd || current->type == PIPE)
// 		{
// 			if (current->type == PIPE)
// 			{
// 				current = current->next;
// 				if (!current)
// 					break;
// 			}
// 			t_command *new_cmd = create_command();
// 			if (!new_cmd)
// 			{
// 				free_commands(cmd_head);
// 				return (NULL);
// 			}
// 			if (!cmd_head)
// 				cmd_head = new_cmd;
// 			else
// 			{
// 				current_cmd->next = new_cmd;
// 				current_cmd->is_piped = true;
// 			}
// 			current_cmd = new_cmd;
// 			current_cmd->in_fd = STDIN_FILENO;
// 			current_cmd->out_fd = STDOUT_FILENO;
// 		}
// 		if (current->type == CMD)
// 			current_cmd->cmd = current;
// 		else if (current->type == ARG && !(current->prev && \
// 			(current->prev->type == INPUT || current->prev->type == OUTPUT || \
// 			current->prev->type == APPEND || current->prev->type == HEREDOC)))
// 			add_to_args(current_cmd, current);
// 		else if (current->type == INPUT || current->type == HEREDOC)
// 			current_cmd->input = current;
// 		else if (current->type == OUTPUT || current->type == APPEND)
// 			current_cmd->output = current;
// 		current = current->next;
// 	}
// 	cmd = cmd_head;
// 	while (cmd)
// 	{
// 		if (!cmd->cmd && (cmd->input || cmd->output))
// 		{
// 			default_cmd = create_token("cat", CMD);
// 			if (!default_cmd)
// 				return (free_commands(cmd_head), NULL);
// 			cmd->cmd = default_cmd;
// 			cmd->args = default_cmd;
// 		}
// 		cmd = cmd->next;
// 	}
// 	return (cmd_head);
// }

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = (t_command *)malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->cmd = NULL;
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->in_fd = STDIN_FILENO;
	cmd->out_fd = STDOUT_FILENO;
	cmd->is_piped = false;
	cmd->next = NULL;
	return (cmd);
}

void	add_to_args(t_command *cmd, t_token *arg_token)
{
    t_token	*new_token;
    t_token	*current;
    
    // Create a copy of the token
    new_token = create_token(arg_token->str, arg_token->type, arg_token->quotestate);
    if (!new_token)
        return;
    
    if (!cmd->args)
    {
        cmd->args = new_token;
    }
    else
    {
        // Find the last argument in the chain
        current = cmd->args;
        while (current->next)
            current = current->next;
        
        // Link the new argument to the end
        current->next = new_token;
        new_token->prev = current;
    }
}

bool	setup_pipes(t_command *commands)
{
	t_command	*current;
	int			pipe_fd[2];

	current = commands;
	while (current && current->next)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("setup_pipes: pipe creation failed");
			return (false);
		}
		if (current->out_fd == STDOUT_FILENO)
			current->out_fd = pipe_fd[1];
		else
			close(pipe_fd[1]);
		if (current->next->in_fd == STDIN_FILENO)
			current->next->in_fd = pipe_fd[0];
		else
			close(pipe_fd[0]);
		current->is_piped = true;
		current = current->next;
	}
	return (true);
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

	count =  1 + count_args(args);
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

bool	check_commands(t_command *cmd_head)
{
	t_command	*cmds;

	cmds = cmd_head;
	while (cmds)
	{
		if (!cmds->cmd)
			return (false);
		cmds = cmds->next;
	}
	return (true);
}
