/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strchr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 14:37:44 by rbagin        #+#    #+#                 */
/*   Updated: 2024/10/19 16:12:34 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
		{
			return ((char *)&s[i]);
		}
		i++;
	}
	if ((char)c == '\0')
		return ((char *)&s[i]);
	return (NULL);
}

// #include <string.h>
// #include <stdio.h>

// int	main(void)
// {
// 	char *str = "Hello World!";
// 	char *c = strchr(str, 'W');
// 	printf("strchr Result: %s\n", c);
// 	printf("ft_strchr Result: %s\n", ft_strchr(str, 'W'));
// 	return (0);
// }

// The  strchr()  function returns a pointer to the first occurrence of the
// character c in the
//        string s.
