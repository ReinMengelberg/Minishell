/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_hex.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/11 16:49:20 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/05 14:56:27 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	print_hexa(char *buffer, int start, int fd)
{
	int	count;

	count = 0;
	while (buffer[start])
	{
		ft_putchar_fd(buffer[start], fd);
		count++;
		start++;
	}
	return (count);
}

int	handle_low_hex(unsigned int num, int fd)
{
	char	buffer[9];
	int		i;
	int		digit;

	i = 7;
	buffer[8] = '\0';
	if (num == 0)
	{
		ft_putchar_fd('0', fd);
		return (1);
	}
	while (i >= 0)
	{
		digit = num % 16;
		if (digit < 10)
			buffer[i] = '0' + digit;
		else
			buffer[i] = 'a' + (digit - 10);
		num /= 16;
		i--;
	}
	while (buffer[i + 1] == '0')
		i++;
	return (print_hexa(buffer, i + 1, fd));
}

int	handle_upper_hex(unsigned int num, int fd)
{
	char	buffer[9];
	int		i;
	int		digit;

	i = 7;
	buffer[8] = '\0';
	if (num == 0)
	{
		ft_putchar_fd('0', fd);
		return (1);
	}
	while (i >= 0)
	{
		digit = num % 16;
		if (digit < 10)
			buffer[i] = '0' + digit;
		else
			buffer[i] = 'A' + (digit - 10);
		num /= 16;
		i--;
	}
	while (buffer[i + 1] == '0')
		i++;
	return (print_hexa(buffer, i + 1, fd));
}
