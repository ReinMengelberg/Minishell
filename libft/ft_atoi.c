/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 14:52:44 by rbagin        #+#    #+#                 */
/*   Updated: 2025/03/10 15:45:44 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*	Converts a string to an interger
	doesn't handle overflow*/
int	ft_atoi(const char *str)
{
	int	i;
	int	nbr;
	int	is_negative;

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

// #include <stdio.h>
// #include <stdlib.h>

// int main(void)
// {
// 	int num = ft_atoi(" ---+-1-+1234ab567");
// 	printf("Number: %d\n", num);
// 	printf("Number: %d\n", atoi(" ---+-1-+1234ab567"));
// 	return (0);
// }
