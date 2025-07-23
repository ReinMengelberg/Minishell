/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   exit_utils.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/22 21:14:57 by rein          #+#    #+#                 */
/*   Updated: 2025/07/22 21:23:58 by rein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

long int	ft_strtol(char *line)
{
	long int	num;
	int			i;
	int			sign;

	num = 0;
	i = 0;
	sign = 1;
	if (line[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (line[i] == '+')
		i++;
	while (line[i] >= '0' && line[i] <= '9')
	{
		num = num * 10 + (line[i] - '0');
		i++;
	}
	return (num * sign);
}

bool	isnum(char *str)
{
	int	i;

	if (!str || !*str)
		return (false);
	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (false);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

bool	is_valid_exit_num(char *str)
{
	char	*endptr;

	if (!isnum(str))
		return (false);
	errno = 0;
	strtol(str, &endptr, 10);
	if (errno == ERANGE || *endptr != '\0')
		return (false);
	return (true);
}

int	calc_exit(char *exit_arg)
{
	long	exit_code;

	exit_code = strtol(exit_arg, NULL, 10);
	exit_code = exit_code % 256;
	if (exit_code < 0)
		exit_code += 256;
	return ((int)exit_code);
}
