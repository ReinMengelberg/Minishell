/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strmapi.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 14:05:43 by rbagin        #+#    #+#                 */
/*   Updated: 2024/10/15 13:50:54 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	unsigned int	i;

	if (!s || !f)
		return (NULL);
	str = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		str[i] = f(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

// #include <stdio.h>

// char	to_upper_wrapper(unsigned int i, char c)
// {
//     (void)i; // Unused parameter
//     return (char)ft_toupper((int)c);
// }

// int main()
// {
// 	char *s = "Hello";
// 	char *str = ft_strmapi(s, to_upper_wrapper);
// 	printf("%s\n", str);
// }
