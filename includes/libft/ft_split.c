/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_split.c                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/18 22:23:14 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/02 13:15:16 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_free(char **arr, int i)
{
	while (i >= 0)
	{
		free(arr[i]);
		i--;
	}
	free(arr);
}

static int	ft_count_words(char const *s, char c)
{
	int	words;
	int	i;

	words = 0;
	i = 0;
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		if (s[i] != c)
		{
			words++;
			while (s[i] != c && s[i] != '\0')
				i++;
		}
		else
			i++;
	}
	return (words);
}

static char	*ft_add_word(char const *s, int *i, char c, char **arr)
{
	int		start;
	int		j;

	j = 0;
	while (s[j] == c && s[j] != '\0')
		j++;
	start = j;
	while (s[j] != c && s[j])
		j++;
	if (j > start)
	{
		arr[*i] = ft_substr(s, start, j - start);
		if (!arr[*i])
		{
			ft_free(arr, *i - 1);
			return (NULL);
		}
		(*i)++;
	}
	return ((char *)s + j);
}

/*	DONT FORGET TO FREE*/
char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		i;
	char	*next;
	int		check;

	arr = (char **)malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!arr)
		return (NULL);
	i = 0;
	if (*s != '\0')
	{
		next = (char *)s;
		while (*next != '\0')
		{
			check = i;
			next = ft_add_word(next, &i, c, arr);
			if (next == NULL && check == i)
				return (NULL);
		}
	}
	arr[i] = NULL;
	return (arr);
}
/*
#include <stdio.h>

void	ft_free_2(char **arr)
{
	char	**tmp;
	char	*tmp2;

	tmp = arr;
	while (*arr)
	{
		tmp2 = *arr;
		++arr;
		free(tmp2);
	}
	free(tmp);
}


int main(void)
{
	char **arr = ft_split("hello!", ' ');
	int i = 0;
	// printf("%i\n", ft_count_words("Hello!", ' '));
	// printf("%s", arr[1]);
	while (arr[i] != NULL)
	{
		printf("|%s(%zu)|\n", arr[i], ft_strlen(arr[i]));
		i++;
	}
	ft_free_2(arr);
	return (0);
} */
