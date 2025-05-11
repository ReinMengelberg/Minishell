/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strnstr.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 14:51:47 by rbagin        #+#    #+#                 */
/*   Updated: 2024/10/15 14:03:01 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	if (*little == '\0' || little == NULL)
		return ((char *)big);
	if (*big == '\0' || big == NULL)
		return (NULL);
	while (*big != '\0' && len > 0)
	{
		if (*big == *little)
		{
			if (ft_strncmp((char *)big, (char *)little, ft_strlen(little)) == 0)
			{
				if (ft_strlen(little) > len)
					return (NULL);
				return ((char *)big);
			}
		}
		big++;
		len--;
	}
	return (NULL);
}

// #include <stdio.h>
// #include <string.h>

// int main()
// {
// 	char *s1 = "Hello World!";
// 	char *s2 = "World";
// 	printf("%s\n", ft_strnstr(s1, s2, 12));
// 	// printf("%s\n", strnstr(s1, s2, 12));
// }
// cc -Wall -Wextra -Werror ft_strnstr.c ft_strncmp.c ft_strlen.c -o strnstr
