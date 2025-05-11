/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strncmp.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 14:41:22 by rbagin        #+#    #+#                 */
/*   Updated: 2025/03/10 15:58:22 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*	Compares s1 with s2
	Returns 0 if s1 and s2 are identical
	Returns s1[i] - s2[i] if s1[i] != s2[i]*/
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s1[i] != '\0' && s2[i] != '\0')
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}

// #include <string.h>
// #include <stdio.h>

// int main (void)
// {
// 	char *s1 = "dddddddd"; //0 characters | \0 = 0
// 	char *s2 = "dddddddd"; //8 characters | d = 100
// 	int n = 20;
// 	printf("%d\n", ft_strncmp(s1, s2, n));
// 	printf("%d\n", strncmp(s1, s2, n));
// 	return (0);
// }
