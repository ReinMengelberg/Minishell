/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 15:21:53 by rein          #+#    #+#                 */
<<<<<<< HEAD
/*   Updated: 2025/06/28 11:12:57 by rmengelb      ########   odam.nl         */
=======
/*   Updated: 2025/06/28 11:11:12 by rbagin        ########   odam.nl         */
>>>>>>> origin/main
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
	shell->state = INTERACTIVE;
	return (shell);
}

static int	process_input(char *input, t_shell *shell)
{
	shell->tokens = tokenize(input);
	// print_tokens(shell->tokens);
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
		// printf("Exit_status: %d\n", shell->exit_status);
		free(input);
	}
	return (shell->exit_status);
}
/*	Note: This project uses the readline library which has known memory leaks.
	These leaks are expected and allowed by the project subject.
	To test without readline leak noise:
	valgrind --leak-check=full --show-leak-kinds=definite,indirect
	--suppressions=readline.supp ./minishell*/
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
<<<<<<< HEAD
	printf("Exit_status: %d\n", exit_status);
	free_everything(shell);
=======
	free_everything(shell, true);
>>>>>>> origin/main
	free(shell);
	printf("exit\n");
	return (exit_status);
}
<<<<<<< HEAD

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
// 	set_state(shell, INTERACTIVE);
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
=======
>>>>>>> origin/main
