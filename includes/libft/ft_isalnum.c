/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_isalnum.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/09 17:30:44 by rbagin        #+#    #+#                 */
/*   Updated: 2024/10/14 16:57:32 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z') && (c < '0' || c > '9'))
	{
		return (0);
	}
	return (1);
}

// #include <stdio.h>

// int main(void)
// {
// 	int c = '5';
// 	printf("Testing : %d\n", ft_isalnum(c));
// }
