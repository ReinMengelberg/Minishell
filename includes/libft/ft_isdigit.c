/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   isdigit.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/09 17:28:33 by rbagin        #+#    #+#                 */
/*   Updated: 2024/10/10 13:55:56 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (c < '0' || c > '9')
	{
		return (0);
	}
	return (1);
}

// #include <stdio.h>
// int main(void)
// {
// 	int c = '5';
// 	printf("Testing : %d\n", ft_isdigit(c));
// }
