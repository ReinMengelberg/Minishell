/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   heredoc_utils.c                                    :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/20 15:09:25 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/20 15:18:13 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_delimiter(char *line, char *delimiter)
{
	if (!line || !delimiter)
		return (0);
	return (ft_strcmp(line, delimiter) == 0);
}

char	*get_heredoc_name(void)
{
	static int	i = 0;
	char		*name;
	char		*number;

	number = ft_itoa(i);
	if (!number)
		return (NULL);
	name = ft_strjoin(HEREDOC_NAME, number);
	free(number);
	i++;
	return (name);
}

char	*get_delim(char *delim, int *quotes)
{
	int	len;

	len = ft_strlen(delim) - 1;
	if ((delim[0] == '\"' && delim[len] == '\"')
		|| (delim[0] == '\'' && delim[len] == '\''))
	{
		*quotes = 1;
		return (ft_strtrim(delim, "\'\""));
	}
	*quotes = 0;
	return (ft_strdup(delim));
}

void	heredoc_sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

char	*expand_heredoc_line(char *line, t_expand_context *ctx)
{
	char	*expanded_line;
	int		i;

	expanded_line = ft_strdup(line);
	i = 0;
	while (expanded_line && expanded_line[i])
	{
		if (expanded_line[i] == '$')
			expanded_line = expand_variable(expanded_line, &i, ctx);
		else
			i++;
	}
	return (expanded_line);
}
