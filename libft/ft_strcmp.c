/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_strcmp.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/04/17 16:56:14 by rbagin        #+#    #+#                 */
/*   Updated: 2025/04/17 16:58:36 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*	The function compares two strings lexicographically and returns an integer
	indicating their relationship.*/
int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}
