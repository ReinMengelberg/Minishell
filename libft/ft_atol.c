/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atol.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/03/10 15:48:00 by rbagin        #+#    #+#                 */
/*   Updated: 2025/04/11 16:14:06 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*	converts a string to a long
	doesn't handle overflow*/
long	ft_atol(const char *str)
{
	int		i;
	long	nbr;
	int		is_negative;

	i = 0;
	nbr = 0;
	is_negative = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			is_negative = 1;
		if (str[i + 1] == '-' || str[i + 1] == '+')
			return (0);
		i++;
	}
	while ((str[i] >= '0' && str[i] <= '9') && str[i] != '\0')
	{
		nbr = nbr * 10 + (str[i] - '0');
		i++;
	}
	if (is_negative % 2 != 0)
		nbr = -nbr;
	return (nbr);
}
