/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_dprintf.c                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/09 13:46:55 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/05 15:08:45 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// static int	handle_stdin(char c)
// {
// 	ft_putchar_fd((char)c, 1);
// 	return (1);
// }

int	ft_choose_flag(const char *format, va_list args, int fd)
{
	if (*format == 'd' || *format == 'i')
		return (handle_int(va_arg(args, int), fd));
	else if (*format == 'u')
		return (handle_u_int(va_arg(args, unsigned int), fd));
	else if (*format == 's')
		return (handle_str(va_arg(args, char *), fd));
	else if (*format == 'c')
		return (handle_char(va_arg(args, unsigned int), fd));
	else if (*format == 'p')
		return (handle_ptraddress(va_arg(args, uintptr_t), fd));
	else if (*format == 'x')
		return (handle_low_hex(va_arg(args, unsigned int), fd));
	else if (*format == 'X')
		return (handle_upper_hex(va_arg(args, unsigned int), fd));
	else if (*format == '%')
		return (handle_char('%', fd));
	return (0);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list	args;
	int		count;

	count = 0;
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			count += ft_choose_flag(format, args, fd);
			format++;
		}
		else
		{
			count ++;
			ft_putchar_fd(*format, fd);
			format++;
		}
	}
	va_end(args);
	return (count);
}
