/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   get_next_line.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/11/14 13:53:47 by rbagin        #+#    #+#                 */
/*   Updated: 2025/04/16 16:34:11 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	polish_list(t_list **list)
{
	t_list	*last_node;
	t_list	*clean_node;
	int		i;
	int		k;
	char	*buf;

	buf = malloc(BUFFER_SIZE + 1);
	clean_node = malloc(sizeof(t_list));
	if (!buf || !clean_node)
	{
		free(buf);
		free(clean_node);
		return ;
	}
	last_node = find_last_node(*list);
	i = 0;
	k = 0;
	while (last_node->str_buf[i] && last_node->str_buf[i] != '\n')
		++i;
	while (last_node->str_buf[i] && last_node->str_buf[++i])
		buf[k++] = last_node->str_buf[i];
	buf[k] = '\0';
	clean_node->str_buf = buf;
	clean_node->next = NULL;
	dealloc(list, clean_node, buf);
}

char	*exctract_line(t_list *list)
{
	int		str_len;
	char	*next_str;

	if (NULL == list)
		return (NULL);
	str_len = len_to_newline(list);
	next_str = malloc(str_len + 1);
	if (NULL == next_str)
	{
		free(next_str);
		return (NULL);
	}
	copy_str(list, next_str);
	return (next_str);
}

void	append(t_list **list, char *buf, int fd)
{
	t_list	*new_node;
	t_list	*last_node;

	last_node = find_last_node(*list);
	new_node = malloc(sizeof(t_list));
	if (!new_node)
	{
		free(buf);
		return ;
	}
	if (!last_node)
		*list = new_node;
	else
		last_node->next = new_node;
	new_node->str_buf = buf;
	new_node->next = NULL;
	new_node->file_descriptor = fd;
}

void	*create_list(t_list **list, int fd)
{
	int		char_read;
	char	*buf;

	char_read = 1;
	while (!found_newline(*list))
	{
		buf = malloc(BUFFER_SIZE + 1);
		if (NULL == buf)
			return (NULL);
		char_read = read(fd, buf, BUFFER_SIZE);
		if (char_read < 0)
		{
			free(buf);
			return (NULL);
		}
		if (char_read == 0)
		{
			free(buf);
			return (list);
		}
		buf[char_read] = '\0';
		append(list, buf, fd);
	}
	return (list);
}

char	*get_next_line(int fd)
{
	static t_list	*list = NULL;
	char			*next_line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (create_list(&list, fd) == NULL)
		return (dealloc(&list, NULL, NULL), NULL);
	if (list == NULL)
		return (NULL);
	next_line = exctract_line(list);
	polish_list(&list);
	return (next_line);
}

// #include "get_next_line.h"
// int  main(void)
// {
// 	int fd;
// 	char    *str = "";
// 	fd = open("test1.txt", O_RDONLY);
// 	if (fd < 0)
// 	{
// 		write (1, "ERRO to open fd", 16);
// 		close(fd);
// 		return (0);
// 	}
// 	while (str != NULL)
// 	{
// 		str = get_next_line(fd);
// 		if (str != NULL)
// 		{
// 			printf ("%s", str);
// 			free(str);
// 		}
// 	}
// 	printf ("%s", str);
// 	return (0);
// }
// Testing.txt
// 1 2 3 4  5 6 7 8 9
// a|b|c|5|\n|a|b|c|1|

// 10
// 0  1 2 3 4 5  6  7 8 9
// 0|\n|a|b|1|5|\n|\n|a|b|

// 20
// 0 1  2 3 4 5 6  7 8 9 0 1  2  3 4 5 6 7  8 9
// 2|1|\n|a|b|2|6|\n|a|b|3|1|\n|\n|a|b|3|7|\n|a

// 30
// 0 1 2  3 4 5 6 7  8  9
// b|4|2|\n|a|b|4|7|\n|\n|

// 40
//  0  1 2 3 4 5  6 7 8 9
// \n|\n|a|b|5|5|\n|a|b|6

// 50
// 0  1 2 3 4 5  6 7 8 9
// 0|\n|a|b|6|5|\n|e|n|d

// 60
// 0 1  2 3 4 5
// 7|1|\n|7|5|\n
