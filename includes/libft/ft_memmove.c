/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_memmove.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/10 13:46:50 by rbagin        #+#    #+#                 */
/*   Updated: 2024/10/14 21:40:05 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>
#include <stdio.h>

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s = (const unsigned char *)src;

	if (dest == NULL && src == NULL)
		return (NULL);
	d = (unsigned char *)dest;
	if (s > d)
	{
		while (n--)
			*(d++) = *(s++);
	}
	else
	{
		d += n;
		s += n;
		while (n--)
			*(--d) = *(--s);
	}
	return (dest);
}

// #define SIZE    21
// char target[SIZE] = "a shiny white sphere";
// char target2[SIZE] = "a shiny white sphere";
// int main( void )
// {
//  char * p = target + 3;
//  /* p points at the starting character of the word we want to replace */
//  char * source = target + 8; /* start of "shiny" */
//  printf( "Before memmove, target is \"%s\"\n", target );
//  memmove( p, source, 5 );
//  printf( "After memmove, target becomes \"%s\"\n", target );
//  char * p2 = target2 + 3;
//  /* p points at the starting character of the word we want to replace */
//  char * source2 = target2 + 8; /* start of "shiny" */
//  printf( "\nBefore ft_memmove, target is \"%s\"\n", target2 );
//  ft_memmove( p2, source2, 5 );
//  printf( "After ft_memmove, target becomes \"%s\"\n", target2 );
// }
