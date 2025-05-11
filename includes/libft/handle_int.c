/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_int.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/11 15:15:10 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/05 14:57:40 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_put_u_nbr_fd(unsigned int n, int fd)
{
	char	c;

	if (n >= 10)
	{
		ft_putnbr_fd(n / 10, fd);
	}
	c = n % 10 + '0';
	write(fd, &c, 1);
}

static int	ft_intlen(long n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n != 0)
	{
		n = n / 10;
		len++;
	}
	return (len);
}

int	handle_int(int n, int fd)
{
	ft_putnbr_fd(n, fd);
	return (ft_intlen((long)n));
}

int	handle_u_int(unsigned int n, int fd)
{
	ft_put_u_nbr_fd(n, fd);
	return (ft_intlen(n));
}
