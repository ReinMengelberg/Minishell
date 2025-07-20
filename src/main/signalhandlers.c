/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signalhandlers.c                                   :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 14:41:34 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/20 10:44:55 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	handle_signal_interactive(int sig)
{
	g_signal_received = sig;
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_signal_child(int sig)
{
	g_signal_received = sig;
	if (sig == SIGINT)
		write(STDOUT_FILENO, "\n", 1);
	else if (sig == SIGQUIT)
		write(STDERR_FILENO, "Quit: 3\n", 8);
}

void	handle_signal_heredoc(int sig)
{
    g_signal_received = sig;
    if (sig == SIGINT)
    {
        write(STDOUT_FILENO, "\n", 1);
        close(STDIN_FILENO);
    }
}
