/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   helpers.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/30 17:01:51 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/06/30 17:08:13 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(t_token *args)
{
	int		count;
	t_token	*arg;

	count = 0;
	arg = args;
	while (arg && arg->type == ARG)
	{
		count++;
		arg = arg->next;
	}
	return (count);
}