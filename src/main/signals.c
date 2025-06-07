/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 14:41:34 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/06/07 15:37:16 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

// Simple signal handler - just sets the flag
void handle_signal(int sig)
{
	g_signal_received = sig;
	// Write newline to move to next line
	write(STDOUT_FILENO, "\n", 1);
	// Clear the current readline line
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

// Process monitor that has access to shell context
int process_signal_monitor(t_shell *shell)
{
	int last_signal;
	
	while (shell->status == 0)
	{
		if (g_signal_received != 0)
		{
			last_signal = g_signal_received;
			g_signal_received = 0;
			switch (last_signal)
			{
				case SIGINT:
					// Handle Ctrl+C with shell context
					if (shell->current_process_pid > 0)
					{
						// Kill current running process
						kill(shell->current_process_pid, SIGINT);
						shell->current_process_pid = 0;
					}
					shell->exit_status = 130;  // Standard SIGINT exit status
					break;
				default:
					break;
			}
		}
		
		// Small sleep to avoid busy waiting
		usleep(1000);  // 1ms
		
		// Check for other shell conditions that might stop the monitor
		if (shell->should_exit)
			break;
	}
	
	return 0;
}

void setup_signals(t_shell *shell)
{
	struct sigaction sa;

	sa.sa_handler = handle_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	// Ctrl + C
	sigaction(SIGINT, &sa, NULL);

	// Ignore SIGQUIT (Ctrl+\) in interactive mode
	signal(SIGQUIT, SIG_IGN);
	
	// Note: Ctrl+D is not a signal, it's EOF
	// Handle it in your main readline loop like this:
	// char *line = readline("minishell> ");
	// if (!line) { /* Ctrl+D pressed - handle EOF */ }
}