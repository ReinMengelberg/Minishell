/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 15:21:53 by rein          #+#    #+#                 */
/*   Updated: 2025/06/07 15:38:06 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char **environ;

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

void start_signal_monitoring(t_shell *shell)
{
    pid_t monitor_pid = fork();
    
    if (monitor_pid == 0)
    {
        // Child process - run the signal monitor
        process_signal_monitor(shell);
        exit(0);
    }
    else if (monitor_pid > 0)
    {
        // Parent continues with main shell logic
        shell->signal_monitor = monitor_pid;
    }
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
	setup_signals(shell);

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
		free(input);
	}
	free_env(shell->env);
	printf("exit\n");
	return (free(shell), shell->exit_status);
}
