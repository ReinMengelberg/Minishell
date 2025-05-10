/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strtrim.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 14:03:40 by rbagin        #+#    #+#                 */
/*   Updated: 2024/10/21 14:14:13 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	to_be_trimmed(char const *s1, char const *set, int *start, int *end)
{
	*start = 0;
	*end = (int)ft_strlen(s1) - 1;
	while (s1[*start] != '\0')
	{
		if (ft_strchr(set, s1[*start]) != NULL)
			(*start)++;
		else
			break ;
	}
	while (*end > *start)
	{
		if (ft_strchr(set, s1[*end]) != NULL)
			(*end)--;
		else
			break ;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;

	if (!s1 || !set)
		return (NULL);
	to_be_trimmed(s1, set, &start, &end);
	return (ft_substr(s1, start, end - start + 1));
}

// #include <stdio.h>

// int main(void)
// {
// 	char *s1 = "aabcbaa";
// 	char *set = "ab";
// 	printf("%s\n", ft_strtrim(s1, set));
// 	return (0);
// }

// cc -Wall -Wextra -Werror ft_strchr.c ft_strlen.c ft_substr.c ft_strdup.c
// ft_strtrim.c -o test
