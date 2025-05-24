/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/29 14:01:51 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/21 12:36:14 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <sys/wait.h>
# include <string.h>
# include <errno.h>
# include <linux/limits.h>

typedef struct s_command
{
	char				**args;
	pid_t				pid;
	struct s_command	*next;
	int					infile_fd;
	int					outfile_fd;
	char				*infile_path;
}	t_command;

typedef struct s_pipex
{
	int			*pipe_fd;
	t_command	*cmds;
	char		**envp;
	int			count_cmds;
	int			last_error;
}	t_pipex;

typedef struct s_command {
	t_token				*cmd;
	t_token				*args;
	t_token				*input;
	t_token				*output;
	int					in_fd;
	int					out_fd;
	bool				is_piped;
	struct s_command	*next;
} t_command;

//main.c
void		init_pipex(t_pipex *pipex, char *argv[], int argc);
// static int	wait_and_cleanup(t_pipex *pipex)

//pipex_cmd.c
// static t_command *create_cmd_node(void)
t_command	*allocate_cmds(int count);
// static void	free_cmd_list(t_command *head)
void		execute_cmd(t_command *cmd, t_pipex *pipex);
void		execution(t_pipex *pipex);

//pipex_error.c
void		error_exit(char *message, t_pipex *pipex, int exit_num);
void		free_everything(t_pipex *pipex);
void		free_array(char **array);

//pipex_fds.c
void		create_pipes(int *fd, int count, t_pipex *pipex);
void		check_file_fds(t_pipex *pipex);
void		close_unused_pipes(t_pipex *pipex, int cmd_index);
void		dup2_fds(t_pipex *pipex, t_command *cmd, int cmd_index);
void		close_remaining_fds(t_pipex *pipex, t_command *cmd, int cmd_index);

//pipex_heredoc.c
bool		is_here_doc(char *arg);
void		setup_heredoc(t_pipex *pipex, char **argv, int argc);
void		handle_here_doc(t_pipex *pipex, char *limiter);
void		init_pipex_heredoc(t_pipex *pipex, char *argv[], int argc);

//pipex_process.c
void		copy_direct_path(char *cmd, char *final_path);
int			open_input_file(char *file);
int			open_output_file(char *file);
void		get_infile_outfile(t_pipex *pipex, char **argv, int argc);

//pipex_utils.c
// static char	*join_path(char *path, char *cmd)
// static char	**get_paths(char **envp)
int			check_path_and_copy(char *path, char *final_path);
// static int	search_in_paths(char **paths, char *cmd, char *final_path)
int			find_path(char *cmd, char **envp, char *final_path);

#endif
