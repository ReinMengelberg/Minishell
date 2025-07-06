/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 15:21:53 by rein          #+#    #+#                 */
/*   Updated: 2025/07/06 16:49:35 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

// Print command structure
void print_commands(t_command *cmd)
{
	int cmd_num = 1;

	while (cmd)
	{
		printf("Command %d:\n", cmd_num++);
		printf("  Command: %s\n", cmd->cmd ? cmd->cmd->str : "NULL");

		printf("  Arguments: ");
		t_token *arg = cmd->args;
		while (arg && arg->type == ARG)
		{
			printf("'%s' ", arg->str);
			arg = arg->next;
		}
		printf("\n");

		printf("  Input: %s\n", cmd->input ? cmd->input->str : "No token(STDIN)");
		if (cmd->input && cmd->input->next)
			printf("  Input File: %s\n", cmd->input->next->str);

		printf("  Output: %s\n", cmd->output ? cmd->output->str : "No token(STDOUT)");
		if (cmd->output && cmd->output->next)
			printf("  Output File: %s\n", cmd->output->next->str);

		printf("  Is Piped: %s\n", cmd->is_piped ? "Yes" : "No");
		printf("  FDs: in=%d, out=%d\n", cmd->in_fd, cmd->out_fd);

		cmd = cmd->next;
	}
}

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

static int	shell_loop(t_shell *shell)
{
	char	*input;
	bool	signal_received;

	signal_received = false;
	while (shell->status)
	{
		if (shell->exit_status != 130)
		{
			if (g_signal_received == SIGINT)
			{
				signal_received = true;
				g_signal_received = 0;
				shell->exit_status = 130;
			}
			else
				check_signals(shell);
		}
		else
		{
			shell->exit_status = 0;
		}
		if (signal_received)
			signal_received = false;
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
	free_everything(shell, true);
	free(shell);
	printf("exit\n");
	return (exit_status);
}
