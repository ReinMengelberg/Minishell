/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   handle_char.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/11 15:09:59 by rbagin        #+#    #+#                 */
/*   Updated: 2025/05/05 14:55:10 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	handle_char(int c, int fd)
{
	ft_putchar_fd((char)c, fd);
	return (1);
}
