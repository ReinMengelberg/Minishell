/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_ptr_address.c                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/11 15:25:32 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/05 14:58:20 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	print_ptraddress(char *buffer, int i, int fd)
{
	int	length;

	length = 2;
	while (buffer[i])
	{
		ft_putchar_fd(buffer[i], fd);
		length++;
		i++;
	}
	return (length);
}

int	handle_ptraddress(uintptr_t add, int fd)
{
	char	buffer[20];
	int		i;
	int		digit;

	if (!add)
	{
		ft_putstr_fd("(nil)", fd);
		return (5);
	}
	buffer[16] = '\0';
	i = 15;
	while (add > 0)
	{
		digit = add % 16;
		if (digit < 10)
			buffer[i] = '0' + digit;
		else
			buffer[i] = 'a' + (digit - 10);
		add /= 16;
		i--;
	}
	i++;
	ft_putstr_fd("0x", fd);
	return (print_ptraddress(buffer, i, fd));
}
