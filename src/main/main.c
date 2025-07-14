/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 15:21:53 by rein          #+#    #+#                 */
/*   Updated: 2025/07/14 15:55:14 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
**	Note: This project uses the readline library which has known memory leaks.
**	These leaks are expected and allowed by the project subject.
**	To test without readline leak noise:
**	valgrind --leak-check=full --show-leak-kinds=definite,indirect
**	--suppressions=readline.supp ./minishell
*/

#include "minishell.h"

extern char	**environ;

t_shell	*init_shell(void)
{
	t_shell	*shell;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
		return (NULL);
	shell->commands = NULL;
	shell->tokens = NULL;
	shell->pids = NULL;
	shell->env = create_env(environ);
	if (!shell->env)
	{
		free(shell);
		return (NULL);
	}
	shell->exit_status = SUCCESS;
	shell->status = 1;
	shell->state = INTERACTIVE;
	return (shell);
}

static int	process_input(char *input, t_shell *shell)
{
	shell->tokens = tokenize(input);
	shell->tokens = expand_tokens(shell->tokens, shell->env,
			shell->exit_status);
	if (!shell->tokens)
	{
		shell->exit_status = ERROR_INVALID_INPUT;
		return (0);
	}
	shell->commands = extract_commands(shell->tokens);
	shell->exit_status = execute_commands(shell->commands, shell);
	free_everything(shell, false);
	return (1);
}

static void	handle_signals_and_input(t_shell *shell, bool *signal_received)
{
	if (shell->exit_status != 130)
	{
		if (g_signal_received == SIGINT)
		{
			*signal_received = true;
			g_signal_received = 0;
			shell->exit_status = 130;
		}
		else
			check_signals(shell);
	}
	else
		shell->exit_status = 0;
	if (*signal_received)
		*signal_received = false;
}

static int	shell_loop(t_shell *shell)
{
	char	*input;
	bool	signal_received;

	signal_received = false;
	while (shell->status)
	{
		handle_signals_and_input(shell, &signal_received);
		input = readline(PROMPT);
		if (!input)
			break ;
		if (input[0] != '\0')
		{
			add_history(input);
			process_input(input, shell);
		}
		free(input);
	}
	return (shell->exit_status);
}

int	main(void)
{
	t_shell	*shell;
	int		exit_status;

	shell = init_shell();
	if (!shell)
	{
		printf("Error initializing shell\n");
		return (1);
	}
	set_state(shell, INTERACTIVE);
	exit_status = shell_loop(shell);
	free_everything(shell, true);
	free(shell);
	printf("exit\n");
	return (exit_status);
}
