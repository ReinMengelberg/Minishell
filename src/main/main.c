/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 15:21:53 by rein          #+#    #+#                 */
/*   Updated: 2025/06/07 15:15:07 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ;
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

t_shell *init_shell()
{
	t_shell *shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);

	shell->commands = NULL;
	shell->tokens = NULL;
	shell->env = create_env(environ);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	shell->exit_status = SUCCESS; // Assuming SUCCESS is defined in minishell.h
	shell->status = 1; // 1 = shell is running

	return (shell);
}

int main()
{
	char *input;
	t_shell *shell;

	shell = init_shell();
	if (!shell)
	{
		printf("Error initializing shell\n");
		return (1);
	}

	// Set up signal handling
	setup_signals();

	while (shell->status)
	{
		// Display prompt and get input using readline
		input = readline(PROMPT);

		// Handle EOF (Ctrl+D) or error
		if (input == NULL)
			break;
		if (input[0] != '\0')
		{
			add_history(input);
			shell->tokens = tokenize(input);
			shell->tokens = expand_tokens(shell->tokens, shell->env, shell->exit_status);
			if (!shell->tokens)
			{
				shell->exit_status = ERROR_INVALID_INPUT;
				free(input);
				continue;
			}
			shell->commands = extract_commands(shell->tokens);
			shell->exit_status = execute_commands(shell->commands, shell);
		}
		printf("Exit_status: %d\n", shell->exit_status);
		free(input);
	}
	free_env(shell->env);
	printf("exit\n");
	return (free(shell), shell->exit_status);
}
