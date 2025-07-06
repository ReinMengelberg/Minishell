/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   path.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/22 15:26:27 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/06 12:01:58 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper functions
static char **get_paths(char **env)
{
	char **paths;
	char *path_var;
	int i = 0;

	if (!env)
		return (NULL);
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path_var = env[i] + 5;
			if (!path_var || !*path_var)
				return (NULL);
			paths = ft_split(path_var, ':');
			return paths;
		}
		i++;
	}
	return (NULL);
}

static char *join_path(char *path, char *cmd)
{
	char *temp = ft_strjoin(path, "/");
	if (!temp)
		return NULL;
	char *result = ft_strjoin(temp, cmd);
	free(temp);
	return result;
}

static int search_in_paths(char **paths, char *cmd, char *final_path)
{
	char *path;
	int i = 0;

	while (paths[i])
	{
		path = join_path(paths[i], cmd);
		if (!path)
			return 1;
		if (access(path, X_OK) == 0)
		{
			ft_strlcpy(final_path, path, PATH_MAX);
			free(path);
			return 0;
		}
		free(path);
		i++;
	}
	return 1;
}

bool find_command_path(char *cmd, char **env, char *path_buffer)
{
	char **paths;
	int	result;

	if (!cmd || !env || !path_buffer)
		return (false);
	if (ft_strchr(cmd, '/'))
	{
		ft_strlcpy(path_buffer, cmd, PATH_MAX);
		return (access(path_buffer, X_OK) == 0);
	}
	paths = get_paths(env);
	if (!paths)
		return false;
	result = search_in_paths(paths, cmd, path_buffer);
	ft_free_array(paths);
	return (result == 0);
}
