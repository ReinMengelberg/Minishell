/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:59 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/30 17:24:02 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include "minishell.h"

int	exec_pwd(t_command *cmd, t_shell *shell)
{
	char	current_dir[PATH_MAX];
	char	*home;

	(void)cmd;
	(void)shell;
	if (!getcwd(current_dir, sizeof(current_dir)))
	{
		printf("pwd: current directory has been removed\n");
		home = getenv("HOME");
		if (home && chdir(home) == 0)
			printf("Moved to home directory: %s\n", home);
		else if (chdir("/") == 0)
			printf("Moved to root directory: /\n");
		else
			return (perror("pwd: cannot access any directory"), FAILURE);
		return (SUCCESS);
	}
	printf("%s\n", current_dir);
	return (SUCCESS);
}
