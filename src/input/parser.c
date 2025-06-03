/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/20 14:28:09 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/02 16:49:45 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
			{
				current = current->next;
				if (!current)
					break;
			}
			t_command *new_cmd = create_command();
			if (!new_cmd)
			{
				free_commands(cmd_head);
				return (NULL);
			}
			if (!cmd_head)
				cmd_head = new_cmd;
			else
			{
				current_cmd->next = new_cmd;
				current_cmd->is_piped = true;
			}
			current_cmd = new_cmd;
			current_cmd->in_fd = STDIN_FILENO;
			current_cmd->out_fd = STDOUT_FILENO;
		}
		if (current->type == CMD)
			current_cmd->cmd = current;
		else if (current->type == ARG && \
			!(current->prev && (current->prev->type == INPUT || current->prev->type == OUTPUT || \
            current->prev->type == APPEND || current->prev->type == HEREDOC)))
			add_to_args(current_cmd, current);
		else if (current->type == INPUT || current->type == HEREDOC)
			current_cmd->input = current;
		else if (current->type == OUTPUT || current->type == APPEND)
			current_cmd->output = current;
		current = current->next;
	}
	return (cmd_head);
}

t_command *create_command(void)
{
	t_command *cmd;

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

void add_to_args(t_command *cmd, t_token *arg_token)
{
	if (!cmd->args)
		cmd->args = arg_token;
}

bool	setup_pipes(t_command *commands)
{
	t_command	*current;
	int pipe_fd[2];

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

char **tokens_to_args(t_token *cmd, t_token *args)
{
	int count = 0;
	t_token *arg = args;
	char **result;
	int i = 0;
	int j = 0;

	// Count the number of arguments (including command)
	if (cmd)
		count++;
	while (arg && arg->type == ARG)
	{
		count++;
		arg = arg->next;
	}

	// Allocate space for arguments plus NULL terminator
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return NULL;

	// Add command as first argument
	if (cmd)
		result[i++] = ft_strdup(cmd->str); //

	// Add remaining arguments
	arg = args;
	while (arg && arg->type == ARG)
	{
		result[i++] = ft_strdup(arg->str);
		if (!result[i - 1])
		{
			while (j < i - 1)
				free(result[j++]);
			free(result);
			return NULL;
		}
		arg = arg->next;
	}
	
	// NULL-terminate the array
	result[i] = NULL;
	
	return result;
}

bool	check_commands(t_command *cmd_head)
{
	t_command	*cmds;

	cmds = cmd_head;
	while(cmds)
	{
		if (!cmds->cmd)
			return (false);
		cmds = cmds->next;
	}
	return (true);
}
