/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pipex_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/01 14:44:05 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/14 15:07:02 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/*	Uses split, so needs to be freed eventually*/
static char	**get_paths(char **envp)
{
	char	**paths;
	char	*path_var;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_var = envp[i] + 5;
			paths = ft_split(path_var, ':');
			return (paths);
		}
		i++;
	}
	return (NULL);
}

/* Uses strjoin, so needs to be freed eventually*/
static char	*join_path(char *path, char *cmd)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(path, "/");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, cmd);
	free(temp);
	return (result);
}

int	check_path_and_copy(char *path, char *final_path)
{
	int	j;

	if (access(path, X_OK) == 0)
	{
		j = -1;
		while (path[++j] != '\0')
			final_path[j] = path[j];
		final_path[j] = '\0';
		return (1);
	}
	return (0);
}

static int	search_in_paths(char **paths, char *cmd, char *final_path)
{
	char	*path;
	int		i;

	i = 0;
	while (paths[i])
	{
		path = join_path(paths[i], cmd);
		if (!path)
			return (1);
		if (check_path_and_copy(path, final_path))
		{
			free_array(paths);
			free(path);
			return (0);
		}
		free(path);
		i++;
	}
	free_array(paths);
	return (1);
}

int	find_path(char *cmd, char **envp, char *final_path)
{
	char	**paths;

	if (ft_strchr(cmd, '/'))
	{
		copy_direct_path(cmd, final_path);
		return (0);
	}
	paths = get_paths(envp);
	if (!paths)
		return (1);
	return (search_in_paths(paths, cmd, final_path));
}
