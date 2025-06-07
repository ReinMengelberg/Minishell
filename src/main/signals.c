/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 14:41:34 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/06/07 14:42:01 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int sig)
{
	g_signal_received = sig;
	// Write newline to move to next line
	write(STDOUT_FILENO, "\n", 1);
	// Clear the current readline line
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void setup_signals(void)
{
	struct sigaction sa;

	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);

	// Ignore SIGQUIT (Ctrl+\) in interactive mode
	signal(SIGQUIT, SIG_IGN);
}