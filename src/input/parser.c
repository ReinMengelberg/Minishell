/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   parser.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/20 14:28:09 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/05/20 15:21:14 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pipex.h"

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
		if (current_cmd || current->type == PIPE)
		{
			if (current->type == PIPE)
				current = current->next;
			t_command *new_cmd = create_command();
			if (!new_cmd)
				return (NULL);
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
		else if (current->type == ARG)
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
	// else
	// 	//concatinate with the current ARGS
}

bool	process_redirections(t_command *commands)
{
	t_command	*cmd;
	t_token *filename;

	filename = NULL;
	cmd = commands;
	while(cmd)
	{
		if (cmd->input)
		{
			if (cmd->input->type == INPUT)
			{
				filename = cmd->input->next;
				if (!filename)
					return (false);
				cmd->in_fd = open(filename->str, O_RDONLY);
				if (cmd->in_fd < 0)
				{
					perror("INPUT open error");
					return (false);
				}
			}
			else if (cmd->input->type == HEREDOC)
			{
				t_token *delimiter = cmd->input->next;
				if (!delimiter)
					return (false);
				int	pipe_fd[2];
				if (pipe(pipe_fd) == -1)
				{
					perror("HEREDOC pipe error");
					return (false);
				}
				// setup_heredoc(pipe_fd, delimiter->str);
				// need to REIMPLEMENT from pipex;
				cmd->in_fd = pipe_fd[0];
				close(pipe_fd[1]);
			}
		}
		if (cmd->output)
		{
			if (cmd->output->type == OUTPUT)
			{
				filename = cmd->output->next;
				if (!filename)
					return false;
				cmd->out_fd = open(filename->str, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (cmd->out_fd < 0)
				{
					perror("OUTPUT open error");
					return false;
				}
			}
			else if (cmd->output->type == APPEND)
			{
				filename = cmd->output->next;
				if (!filename)
					return false;
				cmd->out_fd = open(filename->str, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (cmd->out_fd < 0)
				{
					perror("APPEND open error");
					return false;
				}
			}
		}
		cmd = cmd->next;
	}
	return (true);
}

bool	setup_pipes(t_command *commands)
{
	t_command	*current;

	current = commands;
	while (current && current->next)
	{
		int pipe_fd[2];
		if (pipe(pipe_fd) == -1)
		{
			perror("setup_pipes: pipe creation failed");
			return (false);
		}
		current->out_fd = pipe_fd[1];
		current->next->in_fd = pipe_fd[0];
		current->is_piped = true;
		current = current->next;
	}
	return (true);
}

bool	commands_to_pipex(t_command *commands, t_pipex *pipex)
{
	int			count;
	t_command	*cmd;
	t_command	*current;
	t_command	*pipex_cmd;
	t_pipex_command	*new_cmd;

	count = 0;
	cmd = commands;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	pipex->count_cmds = count;
	pipex->cmds = NULL;
	pipex->pipe_fd = malloc(sizeof(int) * (count - 1) * 2);
	if (!pipex->pipe_fd)
		return (false);
	current = commands;
	pipex_cmd = NULL;
	while (current)
	{
		new_cmd = allocate_cmds(1);
		if (!new_cmd)
			return (false);
		new_cmd->infile_fd = current->in_fd;
		new_cmd->outfile_fd = current->out_fd;
		new_cmd->args = tokens_to_args(current->cmd, current->args);
		if (!pipex->cmds)
			pipex->cmds = new_cmd;
		else
			pipex_cmd->next = new_cmd;
		pipex_cmd = new_cmd;
		current = current->next;
	}
	return (true);
}

char **tokens_to_args(t_token *cmd, t_token *args)
{
	int	i;
	int	count;
	t_token *current;
	char **result;

	count = 1;
	current = args;
	while (current)
	{
		count++;
		current = current->next;
	}
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return NULL;
	// First argument is the command
	result[0] = ft_strdup(cmd->str);
	// Add remaining arguments
	current = args;
	i = 1;
	while (current)
	{
		result[i++] = ft_strdup(current->str);
		current = current->next;
	}
	result[i] = NULL;
	return result;
}
