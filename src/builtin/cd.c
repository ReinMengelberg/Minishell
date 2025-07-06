/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:41 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/30 17:22:30 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_missing_cwd(t_shell *shell, char *current_dir)
{
	char	*home;

	if (getcwd(current_dir, PATH_MAX))
		return (SUCCESS);
	printf("cd: current directory has been removed\n");
	home = getenv("HOME");
	if (home && chdir(home) == 0)
	{
		printf("Moved to home directory: %s\n", home);
		strcpy(current_dir, home);
	}
	else if (chdir("/") == 0)
	{
		printf("Moved to root directory: /\n");
		strcpy(current_dir, "/");
	}
	else
	{
		perror("cd: cannot access any directory");
		return (FAILURE);
	}
	update_env_var(&shell->env, "PWD", current_dir);
	return (SUCCESS);
}

static char	*get_target_directory(t_command *cmd, t_shell *shell)
{
	char	*oldpwd;
	char	*home_dir;

	if (count_args(cmd->args) == 0)
	{
		home_dir = env_get(shell->env, "HOME");
		if (!home_dir)
			fprintf(stderr, "cd: HOME not set\n");
		return (home_dir);
	}
	if (count_args(cmd->args) == 1)
	{
		if (strcmp(cmd->args->str, "-") == 0)
		{
			oldpwd = env_get(shell->env, "OLDPWD");
			if (!oldpwd)
				fprintf(stderr, "cd: OLDPWD not set\n");
			else
				ft_printf("%s\n", oldpwd);
			return (oldpwd);
		}
		return (cmd->args->str);
	}
	return (fprintf(stderr, "cd: too many arguments\n"), NULL);
}

static int	perform_directory_change(char *target_dir, char *current_dir,
		t_shell *shell)
{
	char	new_dir[PATH_MAX];

	if (chdir(target_dir) != 0)
	{
		perror("cd");
		return (FAILURE);
	}
	if (!getcwd(new_dir, sizeof(new_dir)))
	{
		perror("cd: getcwd failed after chdir");
		chdir(current_dir);
		return (FAILURE);
	}
	if (update_env_var(&shell->env, "OLDPWD", current_dir) != SUCCESS)
	{
		fprintf(stderr, "cd: failed to update OLDPWD\n");
		return (FAILURE);
	}
	if (update_env_var(&shell->env, "PWD", new_dir) != SUCCESS)
	{
		fprintf(stderr, "cd: failed to update PWD\n");
		return (FAILURE);
	}
	return (SUCCESS);
}

int	exec_cd(t_command *cmd, t_shell *shell)
{
	char	*target_dir;
	char	current_dir[PATH_MAX];

	if (!cmd || !shell)
		return (ERROR_NULL_POINTER);
	if (handle_missing_cwd(shell, current_dir) != SUCCESS)
		return (FAILURE);
	target_dir = get_target_directory(cmd, shell);
	if (!target_dir)
		return (FAILURE);
	return (perform_directory_change(target_dir, current_dir, shell));
}
