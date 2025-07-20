/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signalsetup.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/14 15:49:05 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/20 10:55:26 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_signal_handler(void (*handler)(int))
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	check_signals(t_shell *shell)
{
	int	sig;

	if (g_signal_received != 0)
	{
		sig = g_signal_received;
		g_signal_received = 0;
		if (sig == SIGINT)
			shell->exit_status = 130;
		else if (sig == SIGQUIT)
		{
			if (shell->state == IN_CHILD)
				shell->exit_status = 131;
		}
	}
}

void	set_state(t_shell *shell, t_state state)
{
	shell->state = state;
	if (state == INTERACTIVE)
	{
		setup_signal_handler(handle_signal_interactive);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (state == IN_CHILD)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
	else if (state == IN_HEREDOC)
	{
		setup_signal_handler(handle_signal_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
}
