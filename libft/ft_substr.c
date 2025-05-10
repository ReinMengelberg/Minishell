/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_substr.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/19 16:04:58 by rbagin        #+#    #+#                 */
/*   Updated: 2024/10/21 15:24:51 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static char	*ft_malloc_empty(void)
{
	char	*dest;

	dest = (char *)malloc(sizeof(char) * 1);
	if (dest != NULL)
		dest[0] = '\0';
	return (dest);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	s_len;
	char	*substr;

	if (s == NULL)
		return (NULL);
	s_len = ft_strlen(s);
	if (start >= ft_strlen(s))
		return (ft_malloc_empty());
	if (len > s_len - start)
		len = s_len - start;
	substr = (char *)malloc(sizeof(char) * (len + 1));
	if (substr == NULL)
		return (NULL);
	i = 0;
	while (i < len && s[start + i] != '\0')
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

// #include <stdio.h>

// int	main(void)
// {
// 	char *s = "Hello, World!";
// 	char *result;

// 	// Test 1: Normal case
// 	result = ft_substr(s, 7, 5);
// 	printf("Test 1: %s\n", result); // Expected: "World"
// 	free(result);

// 	// Test 2: Start index beyond the length of the string
// 	result = ft_substr(s, 20, 5);
// 	printf("Test 2: %s\n", result); // Expected: ""
// 	free(result);

// 	// Test 3: Length exceeds the remaining length of the string
// 	result = ft_substr(s, 7, 50);
// 	printf("Test 3: %s\n", result); // Expected: "World!"
// 	free(result);

// 	// Test 4: Start index at the beginning
// 	result = ft_substr(s, 0, 5);
// 	printf("Test 4: %s\n", result); // Expected: "Hello"
// 	free(result);

// 	// Test 5: Empty string
// 	result = ft_substr("", 0, 5);
// 	printf("Test 5: %s\n", result); // Expected: ""
// 	free(result);

// 	// Test 6: Length is zero
// 	result = ft_substr(s, 7, 0);
// 	printf("Test 6: %s\n", result); // Expected: ""
// 	free(result);

// 	// Test 7: Start index is zero and length is zero
// 	result = ft_substr(s, 0, 0);
// 	printf("Test 7: %s\n", result); // Expected: ""
// 	free(result);

// 	return (0);
// }
