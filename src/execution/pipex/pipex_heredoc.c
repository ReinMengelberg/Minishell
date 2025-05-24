/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_heredoc.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/14 14:27:55 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/14 14:53:42 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

bool	is_here_doc(char *arg)
{
	return (arg && ft_strncmp(arg, "here_doc", 8) == 0);
}

void	setup_heredoc(t_pipex *pipex, char **argv, int argc)
{
	t_command	*last_cmd;

	if (argc < 6)
		error_exit("Usage: ./pipex here_doc LIMITER cmd1 cmd2 ... outfile",
			NULL, 1);
	init_pipex_heredoc(pipex, argv, argc);
	handle_here_doc(pipex, argv[2]);
	last_cmd = pipex->cmds;
	while (last_cmd->next)
		last_cmd = last_cmd->next;
	last_cmd->outfile_fd = open(argv[argc - 1], O_WRONLY | O_CREAT
			| O_APPEND, 0644);
}

void	handle_here_doc(t_pipex *pipex, char *limiter)
{
	int		pipe_fd[2];
	char	*line;
	size_t	limiter_len;

	limiter_len = ft_strlen(limiter);
	if (pipe(pipe_fd) == -1)
		error_exit("Pipe creation failed", pipex, 1);
	ft_printf("heredoc>");
	line = get_next_line(0);
	while (line != NULL)
	{
		if (ft_strncmp(line, limiter, limiter_len) == 0
			&& (line[limiter_len] == '\n' || line[limiter_len] == '\0'))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipe_fd[1]);
		free(line);
		ft_putstr_fd("heredoc> ", 1);
		line = get_next_line(0);
	}
	close(pipe_fd[1]);
	pipex->cmds->infile_fd = pipe_fd[0];
}

void	init_pipex_heredoc(t_pipex *pipex, char *argv[], int argc)
{
	t_command	*cmd_head;
	int			i;

	pipex->count_cmds = argc - 4;
	pipex->cmds = allocate_cmds(pipex->count_cmds);
	if (!pipex->cmds)
		error_exit("Memory allocation failed", NULL, 1);
	i = 3;
	cmd_head = pipex->cmds;
	while (cmd_head)
	{
		if (argv[i] && argv[i][0] != '\0')
			cmd_head->args = ft_split(argv[i], ' ');
		else
			cmd_head->args = ft_calloc(sizeof(char *), 1);
		if (!cmd_head->args)
			error_exit("ft_split failed", pipex, 1);
		cmd_head = cmd_head->next;
		i++;
	}
	pipex->pipe_fd = ft_calloc(sizeof(int) * pipex->count_cmds * 2, 1);
	if (!pipex->pipe_fd)
		error_exit("pipe creation failed", pipex, 1);
}
