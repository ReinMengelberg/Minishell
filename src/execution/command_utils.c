/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   command_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/15 15:38:46 by rein          #+#    #+#                 */
/*   Updated: 2025/07/15 15:51:17 by rein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	wait_for_remain(pid_t *pids, int count)
{
	int	status;
	int	i;

	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
		{
			if (waitpid(pids[i], &status, 0) == -1)
				perror("waitpid failed");
		}
		i++;
	}
}

static char	**allocate_env_array(t_env *env_list)
{
	int		count;
	t_env	*current;
	char	**result;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	result = malloc(sizeof(char *) * (count + 1));
	return (result);
}

static char	*create_env_string(t_env *current)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(current->key, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, current->value);
	free(temp);
	return (result);
}

char	**env_to_array(t_env *env_list)
{
	char	**result;
	t_env	*current;
	int		i;

	result = allocate_env_array(env_list);
	if (!result)
		return (NULL);
	current = env_list;
	i = 0;
	while (current)
	{
		result[i] = create_env_string(current);
		if (!result[i])
		{
			ft_free_array(result);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	result[i] = NULL;
	return (result);
}

int	count_commands(t_command *commands)
{
	int			count;
	t_command	*current;

	count = 0;
	current = commands;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}
