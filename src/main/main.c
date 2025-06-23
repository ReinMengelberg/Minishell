/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 15:21:53 by rein          #+#    #+#                 */
/*   Updated: 2025/06/23 17:28:06 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

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
	shell->sig_state = INTERACTIVE;
	return (shell);
}

static int	process_input(char *input, t_shell *shell)
{
	shell->tokens = tokenize(input);
	shell->tokens = expand_tokens(shell->tokens, shell->env, shell->exit_status);
	if (!shell->tokens)
	{
		shell->exit_status = ERROR_INVALID_INPUT;
		return (0);
	}
	shell->commands = extract_commands(shell->tokens);
	shell->exit_status = execute_commands(shell->commands, shell);
	return (1);
}

static int	shell_loop(t_shell *shell)
{
	char	*input;

	while (shell->status)
	{
		check_signals(shell);
		input = readline(PROMPT);
		if (!input)
			break ;
		if (input[0] != '\0')
		{
			add_history(input);
			process_input(input, shell);
		}
		printf("Exit_status: %d\n", shell->exit_status);
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
	set_sigstate(shell, INTERACTIVE);
	exit_status = shell_loop(shell);
	free_everything(shell);
	free(shell);
	printf("exit\n");
	return (exit_status);
}

// int	main()
// {
// 	char *input;
// 	t_shell *shell;
// 	int		exit_status;

// 	shell = init_shell();
// 	if (!shell)
// 	{
// 		printf("Error initializing shell\n");
// 		return (1);
// 	}
// 	set_sigstate(shell, INTERACTIVE);
// 	while (shell->status)
// 	{
// 		check_signals(shell);
// 		input = readline(PROMPT);
// 		if (input == NULL)  // Ctrl+D
// 			break;
// 		if (input[0] != '\0')
// 		{
// 			add_history(input);
// 			shell->tokens = tokenize(input);
// 			shell->tokens = expand_tokens(shell->tokens, shell->env, shell->exit_status);
// 			if (!shell->tokens)
// 			{
// 				shell->exit_status = ERROR_INVALID_INPUT;
// 				free(input);
// 				continue;
// 			}
// 			shell->commands = extract_commands(shell->tokens);
// 			shell->exit_status = execute_commands(shell->commands, shell);
// 		}
// 		printf("Exit_status: %d\n", shell->exit_status);
// 		free(input);
// 	}
// 	free_everything(shell);
// 	exit_status = shell->exit_status;
// 	free(shell);
// 	printf("exit\n");
// 	return (exit_status);
// }
