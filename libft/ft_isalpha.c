/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_isalpha.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/09 17:21:55 by rbagin        #+#    #+#                 */
/*   Updated: 2024/10/14 16:57:57 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalpha(int c)
{
	if ((c < 'A' || c > 'Z') && (c < 'a' || c > 'z'))
	{
		return (0);
	}
	return (1);
}
// #include <stdio.h>

// int main(void)
// {
// 	char str[] = "abcdefg";
// 	printf("%d\n", ft_str_is_alpha(str));
// 	char str2[] = "123456789";
// 	printf("%d\n", ft_str_is_alpha(str2));
// 	return (0);
// }
