/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strjoin.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/11 14:31:33 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/02 13:41:16 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

/* DONT FORGET TO FREE*/
char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	char	*str1;
	char	*str2;
	size_t	str_len;

	str1 = (char *)s1;
	str2 = (char *)s2;
	if (!s1 || !s2)
		return (NULL);
	str_len = (ft_strlen(s1) + ft_strlen(s2)) + 1;
	str = (char *)malloc(sizeof(char) * (str_len));
	if (str == NULL)
		return (NULL);
	ft_strlcpy(str, str1, str_len);
	ft_strlcat(str, str2, str_len);
	return (str);
}

// int	main(void)
// {
// 	char	*str = ft_strjoin("Hello", " World!");
// 	printf("String: %s\n", str);
// 	free(str);
// 	return (0);
// }
