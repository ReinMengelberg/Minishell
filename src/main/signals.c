/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   signals.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/07 14:41:34 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/06/08 10:54:51 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal_received = 0;

void handle_signal(int sig)
{
    g_signal_received = sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void monitor_signals(t_shell *shell)
{
    while (shell->status == 1)
    {
        while (!g_signal_received)
            pause();
        switch (g_signal_received)
        {
            case SIGINT:
                if (shell->state == IN_HEREDOC)

                else if ()

                break;
            default:
                break;
        }
        
    }
}

void setup_signals(t_shell *shell)
{
    struct sigaction sa;

    sa.sa_handler = handle_signal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    // Register signal handlers FIRST
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
    
    // No blocking monitor function!
}