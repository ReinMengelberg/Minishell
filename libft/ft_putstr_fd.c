/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_putstr_fd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/10/14 14:06:07 by rbagin        #+#    #+#                 */
/*   Updated: 2025/03/10 13:10:00 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*	Outputs given string onto given file descriptor
	fd 0 = Standard input (keyboard)
	fd 1 = Standard output (terminal)
	fd 2 = Standard error (error output)
	fd 3+ = Other files/sockets
	Uses ft_strlen*/
void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}
