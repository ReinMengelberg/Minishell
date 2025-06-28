/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 14:41:34 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/06/28 17:17:03 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_signal_received = 0;

void	handle_signal_interactive(int sig)
{
	printf("interactie singal");
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
	if (sig == SIGINT) {
		write(STDOUT_FILENO, "\n", 1);
		exit(130);
		// rl_replace_line("", 0);
		// rl_done = 1;
		// rl_event_hook = NULL;
	}
}

// Non-blocking signal check function
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

void	setup_signal_handler(void (*handler)(int))
{
	struct sigaction	sa;

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
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
		setup_signal_handler(handle_signal_child);
	}
	else if (state == IN_HEREDOC)
	{
		setup_signal_handler(handle_signal_heredoc);
		signal(SIGQUIT, SIG_IGN);
	}
}
