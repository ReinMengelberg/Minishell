/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   export.c                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:50 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/07/14 15:32:09 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	parse_export_arg(const char *arg, char **key, char **value,
		bool *has_equals)
{
	char	*equals_pos;
	int		key_len;

	*has_equals = false;
	equals_pos = ft_strchr(arg, '=');
	if (!equals_pos)
	{
		*key = ft_strdup(arg);
		*value = NULL;
		return (*key != NULL);
	}
	*has_equals = true;
	key_len = equals_pos - arg;
	*key = ft_substr(arg, 0, key_len);
	*value = ft_strdup(equals_pos + 1);
	return (*key != NULL && *value != NULL);
}

static void	print_export_vars(t_env *env_head)
{
	t_env	*current;

	current = env_head;
	while (current)
	{
		if (current->key && current->value)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
}

static int	validate_and_update_env(char *key, char *value, bool has_equals,
		t_shell *shell)
{
	int	result;

	if (!is_valid_var_syntax(key, false))
	{
		ft_dprintf(2, "export: `%s': not a valid identifier\n", key);
		return (FAILURE);
	}
	result = SUCCESS;
	if (has_equals)
	{
		if (update_env_var(&shell->env, key, value) != SUCCESS)
		{
			ft_dprintf(2, "export: memory allocation error\n");
			result = ERROR_MEMORY_ALLOCATION;
		}
	}
	return (result);
}

static int	process_export_args(t_token *args, t_shell *shell)
{
	t_token	*current_arg;
	int		exit_status;
	char	*key;
	char	*value;
	bool	has_equals;

	exit_status = SUCCESS;
	current_arg = args;
	while (current_arg && current_arg->type == ARG)
	{
		if (!parse_export_arg(current_arg->str, &key, &value, &has_equals))
			return (ft_dprintf(2, "export: memory allocation error\n"), -3);
		exit_status = validate_and_update_env(key, value, has_equals, shell);
		free(key);
		if (value)
			free(value);
		if (exit_status == ERROR_MEMORY_ALLOCATION)
			break ;
		if (exit_status == FAILURE)
			exit_status = FAILURE;
		else if (exit_status == SUCCESS)
			exit_status = SUCCESS;
		current_arg = current_arg->next;
	}
	return (exit_status);
}

int	exec_export(t_command *cmd, t_shell *shell)
{
	if (!cmd || !shell)
		return (ERROR_NULL_POINTER);
	if (!cmd->args || cmd->args->type != ARG)
	{
		print_export_vars(shell->env);
		return (SUCCESS);
	}
	return (process_export_args(cmd->args, shell));
}
