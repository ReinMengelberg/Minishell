/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_printf.c                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/09 13:46:55 by rbagin        #+#    #+#                 */
/*   Updated: 2025/06/24 14:55:07 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// static int	handle_stdin(char c)
// {
// 	ft_putchar_fd((char)c, 1);
// 	return (1);
// }

// int	ft_choose_flag(const char *format, va_list args, int fd)
// {
// 	if (*format == 'd' || *format == 'i')
// 		return (handle_int(va_arg(args, int), fd));
// 	else if (*format == 'u')
// 		return (handle_u_int(va_arg(args, unsigned int), fd));
// 	else if (*format == 's')
// 		return (handle_str(va_arg(args, char *), fd));
// 	else if (*format == 'c')
// 		return (handle_char(va_arg(args, unsigned int), fd));
// 	else if (*format == 'p')
// 		return (handle_ptraddress(va_arg(args, uintptr_t), fd));
// 	else if (*format == 'x')
// 		return (handle_low_hex(va_arg(args, unsigned int), fd));
// 	else if (*format == 'X')
// 		return (handle_upper_hex(va_arg(args, unsigned int), fd));
// 	else if (*format == '%')
// 		return (handle_char('%', fd));
// 	return (0);
// }

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;
	int		fd;

	count = 0;
	fd = 1;
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

// int main(void)
// {
// 	// ft_printf("%");
// 	// printf("blablla%t la");
// 		//check basic input
//     // int a = 0;
//     // int b = 0;
//     // a = printf("djaddjdhadjsadkjsadjsa\n");
//     // b = ft_printf("djaddjdhadjsadkjsadjsa\n");
//     // printf("%d, %d\n", a, b);
//     // void *s = NULL;
// 	// ft_printf("my : %c\n", 'f');
// 	// printf("orig : %c\n", 'f');
// 	// ft_printf("my : %%\n");
// 	// printf("orig : %%\n");
// 	// ft_printf("my : NULL %s NULL ", s);
// 	// printf("or : NULL %s NULL ", (char *)s);
// 	// ft_printf("\nmy did: %d\n", -2147483648);
// 	// printf("\norig did: %ld\n", -2147483648);
// 	// ft_printf("my : %i\n", 3243);
// 	// printf("orig : %i\n", 3243);
// 	// // ft_printf("my : %");
// 	// // printf("orig : %");
// 	// ft_printf("my : %X\n", 3);
// 	// printf("orig : %X\n", 3);
// 	// ft_printf("\nmy : %u\n", -1);
// 	// printf("\norig : %u\n", -1);
// 	// ft_printf("my : %p %p ", 0, 0);
// 		// unsigned_int %u testing
// 	// printf("or: %u \n", -1);
// 	// ft_printf("-: %u\n", -1);
// 	// printf("or: %u \n", -9);
// 	// ft_printf("my: %u \n", -9);
// 	// printf("or: %u \n", -10);
// 	// ft_printf("my: %u \n", -10);
// 	// printf("or: %u \n", -11);
// 	// printf("or: %u \n", -14);
// 	// printf("or: %u \n", -15);
// 	// printf("or: %u \n", -16);
// 	// printf("or: %u \n", -99);
// 	// printf("or: %u \n", -100);
// 	// printf("or: %u \n", -101);
// 	// printf("or: %u \n", UINT_MAX);
// 	// ft_printf("my: %u \n", -11);
// 	// ft_printf("my: %u \n", -14);
// 	// ft_printf("my: %u \n", -15);
// 	// ft_printf("my: %u \n", -16);
// 	// ft_printf("my: %u \n", -99);
// 	// ft_printf("my: %u \n", -100);
// 	// ft_printf("my: %u \n", -101);
// 	// ft_printf("my: %u \n", UINT_MAX);
// 		// hex %x testing
// 	// printf("or: --%x-- \n", -1);
// 	// printf("or: --%x-- \n", 1);
// 	// printf("or: --%x-- \n", 9);
// 	// printf("or: --%x-- \n", 10);
// 	// printf("or: --%x-- \n", 11);
// 	// printf("or: --%x-- \n", 15);
// 	// printf("or: --%x-- \n", 16);
// 	// printf("or: --%x-- \n", 17);
// 	// printf("or: --%x-- \n", 99);
// 	// printf("or: --%x-- \n", 100);
// 	// printf("or: --%x-- \n", 101);
// 	// printf("or: --%x-- \n", -9);
// 	// printf("or: --%x-- \n", -10);
// 	// printf("or: --%x-- \n", -11);
// 	// printf("or: --%x-- \n", -14);
// 	// printf("or: --%x-- \n", -15);
// 	// printf("or: --%x-- \n", -16);
// 	// printf("or: --%x-- \n", -99);
// 	// printf("or: --%x-- \n", -100);
// 	// ft_printf("my: ++%x++ \n", -1);
// 	// ft_printf("my: ++%x++ \n", 1);
// 	// ft_printf("my: ++%x++ \n", 9);
// 	// ft_printf("my: ++%x++ \n", 10);
// 	// ft_printf("my: ++%x++ \n", 11);
// 	// ft_printf("my: ++%x++ \n", 15);
// 	// ft_printf("my: ++%x++ \n", 16);
// 	// ft_printf("my: ++%x++ \n", 17);
// 	// ft_printf("my: ++%x++ \n", 99);
// 	// ft_printf("my: ++%x++ \n", 100);
// 	// ft_printf("my: ++%x++ \n", 101);
// 	// ft_printf("my: ++%x++ \n", -9);
// 	// ft_printf("my: ++%x++ \n", -10);
// 	// ft_printf("my: ++%x++ \n", -11);
// 	// ft_printf("my: ++%x++ \n", -14);
// 	// ft_printf("my: ++%x++ \n", -15);
// 	// ft_printf("my: ++%x++ \n", -16);
// 	// ft_printf("my: ++%x++ \n", -99);
// 	// ft_printf("my: ++%x++ \n", -100);
// 	// void *p = (void *)LONG_MAX;
// 	// ft_printf("%p\n", p);
// 	// printf("%p\n", p);
// }
