/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_integration.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/22 16:30:02 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/03 11:24:43 by rbagin        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>

extern char **environ;

// Function declarations
void run_test(char *test_name, char *command);
void print_commands(t_command *cmd);
void cleanup_temp_files(void);
void free_commands(t_command *commands);

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;

	printf("\033[1;33m===== MINISHELL INTEGRATION TESTS =====\033[0m\n\n");

	// Simple commands
	run_test("Simple command", "> hello | /bin/echo hi > hello");
	// ABOVE COMMAND DOESNT WORK BECAUSE OF INPROPERLY SETTING UP in_fd of the 2nd command
	// run_test("Simple command with arguments", "cat hello world | grep 'hello' | grep 'hello' | grep 'hello' > test_output.txt");
	// run_test("das", "ls");

	//TODO
	// /bin/echo hi > permission > hello
	// cat | cat | cat | echo hi
	// < hello | /bin/echo hi > hello

	// // Redirections
	// run_test("Output redirection", "ls -la > output.txt");
	// run_test("Input redirection", "ls -la < output.txt");
	// run_test("Append redirection", "ls -la appended >> output.txt");

	// // Pipes
	// run_test("Simple pipe", "ls -la | grep a");
	// run_test("Multiple pipes", "ls -la | grep a | wc -l");

	// // Complex combinations
	// run_test("Pipe with redirections", "ls -la > list.txt | wc -l > count.txt");
	// run_test("Multiple redirections", "cat hello < output.txt > copy.txt");
	// run_test("Command with mixed redirections", "ls > dir.txt | grep a < dir.txt > filtered.txt");

	// // Command sequence with delayed command
	// run_test("Delayed command", "< output.txt > processed.txt cat");

	// // Complex nested redirections
	// run_test("Multiple outputs", "cat hello > file1 > file2 > file3");
	// run_test("Pipeline with multiple redirections",
	// 		 "ls -la | grep a > grep_out.txt | wc -l > wc_out.txt");

	// Clean up temporary files
	// sleep(60);
	// cleanup_temp_files();

	printf("\n\033[1;32mAll tests completed!\033[0m\n");
	return 0;
}

void run_test(char *test_name, char *command)
{
	t_shell	*shell;
	printf("\n\033[1;34m===== TEST: %s =====\033[0m\n", test_name);
	printf("Command: '%s'\n\n", command);

	shell = malloc(sizeof(t_shell));
	// Tokenize the input
	shell->token_head = tokenize(command);
	if (!shell->token_head)
	{
		printf("\033[1;31mTokenization failed\033[0m\n");
		return;
	}

	// Print shell->token_head
	printf("Tokenization Result:\n");
	print_tokens(shell->token_head);

	// Extract commands
	printf("\nCommand Extraction Result:\n");
	shell->command_head = extract_commands(shell->token_head);
	if (!shell->command_head)
	{
		printf("\033[1;31mCommand extraction failed\033[0m\n");
		free_tokens(shell->token_head);
		return;
	}

	shell->env = create_env(environ);
	// printf("\nPrinting environment\n\n");
	// print_env(shell->env);

	// Execute command
	printf("\nExecution Output:\n");
	printf("------------------------\n");
	int status = execute_commands(shell->command_head, shell);
	printf("------------------------\n");
	printf("Exit Status: %d\n", status);

	if(status == 0)
	{
		printf("\nAfter process_redirections, setup_pipes and EXECUTION\n\n");
		print_commands(shell->command_head);
	}

	// // Cleanup
	// // free_and_exit(shell);
	printf("\033[1;32mTest completed\033[0m\n");
	free_env(shell->env);
	free(shell);
}

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

// Clean up temporary files created during testing
void cleanup_temp_files(void)
{
	char *files[] = {
		"output.txt", "list.txt", "count.txt", "copy.txt",
		"dir.txt", "filtered.txt", "processed.txt",
		"file1", "file2", "file3", "grep_out.txt", "wc_out.txt", NULL
	};

	printf("\n\033[1;33mCleaning up temporary files...\033[0m\n");
	for (int i = 0; files[i]; i++)
	{
		if (access(files[i], F_OK) == 0)
		{
			if (unlink(files[i]) == 0)
				printf("Removed: %s\n", files[i]);
			else
				printf("Failed to remove: %s\n", files[i]);
		}
	}
}

// Free command structures
void free_commands(t_command *commands)
{
	t_command *temp;

	while (commands)
	{
		temp = commands;
		commands = commands->next;

		// Close file descriptors
		if (temp->in_fd > 2)
			close(temp->in_fd);
		if (temp->out_fd > 2)
			close(temp->out_fd);

		free(temp);
	}
}

