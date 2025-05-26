/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_integration.c                                 :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/22 16:30:02 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/05/24 17:22:29 by ravi-bagin    ########   odam.nl         */
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
int execute_test_command(t_command *commands, char **envp);
void cleanup_temp_files(void);
void free_commands(t_command *commands);

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;

	printf("\033[1;33m===== MINISHELL INTEGRATION TESTS =====\033[0m\n\n");

	// Simple commands
	run_test("Simple command", "ls -la");
	run_test("Simple command with arguments", "cat hello world");

	// Redirections
	run_test("Output redirection", "ls -la > output.txt");
	run_test("Input redirection", "cat < output.txt");
	run_test("Append redirection", "cat appended >> output.txt");

	// Pipes
	run_test("Simple pipe", "ls -la | grep a");
	run_test("Multiple pipes", "ls -la | grep a | wc -l");

	// Complex combinations
	run_test("Pipe with redirections", "ls -la > list.txt | wc -l > count.txt");
	run_test("Multiple redirections", "cat < output.txt > copy.txt");
	run_test("Command with mixed redirections", "ls > dir.txt | grep a < dir.txt > filtered.txt");

	// Command sequence with delayed command
	run_test("Delayed command", "< output.txt > processed.txt cat");

	// Complex nested redirections
	run_test("Multiple outputs", "cat hello > file1 > file2 > file3");
	run_test("Pipeline with multiple redirections",
			 "ls -la | grep a > grep_out.txt | wc -l > wc_out.txt");

	// Clean up temporary files
	sleep(60);
	cleanup_temp_files();

	printf("\n\033[1;32mAll tests completed!\033[0m\n");
	return 0;
}

void run_test(char *test_name, char *command)
{
	printf("\n\033[1;34m===== TEST: %s =====\033[0m\n", test_name);
	printf("Command: '%s'\n\n", command);

	// Tokenize the input
	t_token *tokens = tokenize(command);
	if (!tokens)
	{
		printf("\033[1;31mTokenization failed\033[0m\n");
		return;
	}

	// Print tokens
	printf("Tokenization Result:\n");
	print_tokens(tokens);

	// Extract commands
	printf("\nCommand Extraction Result:\n");
	t_command *commands = extract_commands(tokens);
	if (!commands)
	{
		printf("\033[1;31mCommand extraction failed\033[0m\n");
		free_tokens(tokens);
		return;
	}

	// Print command structure
	print_commands(commands);

	// Process redirections
	printf("\nRedirection Processing Result: ");
	if (process_redirections(commands))
		printf("\033[1;32mSuccess\033[0m\n");
	else
		printf("\033[1;31mFailed\033[0m\n");

	// Setup pipes
	printf("Pipe Setup Result: ");
	if (setup_pipes(commands))
		printf("\033[1;32mSuccess\033[0m\n");
	else
		printf("\033[1;31mFailed\033[0m\n");

	// Execute command
	printf("\nExecution Output:\n");
	printf("------------------------\n");
	int status = execute_test_command(commands, environ);
	printf("------------------------\n");
	printf("Exit Status: %d\n", status);

	// Cleanup
	free_tokens(tokens);
	free_commands(commands);
	printf("\033[1;32mTest completed\033[0m\n");
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

		printf("  Input: %s\n", cmd->input ? cmd->input->str : "STDIN");
		if (cmd->input && cmd->input->next)
			printf("  Input File: %s\n", cmd->input->next->str);

		printf("  Output: %s\n", cmd->output ? cmd->output->str : "STDOUT");
		if (cmd->output && cmd->output->next)
			printf("  Output File: %s\n", cmd->output->next->str);

		printf("  Is Piped: %s\n", cmd->is_piped ? "Yes" : "No");
		printf("  FDs: in=%d, out=%d\n", cmd->in_fd, cmd->out_fd);

		cmd = cmd->next;
	}
}
int execute_test_command(t_command *commands, char **envp)
{
    pid_t pid;
    int status;
    time_t start_time;
    const int TIMEOUT_SECONDS = 3; // Strict 3-second timeout
    
    // Backup stdout
    int stdout_backup = dup(STDOUT_FILENO);
    if (stdout_backup == -1) {
        perror("dup");
        return 1;
    }
    
    // Create a pipe to capture output
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        close(stdout_backup);
        return 1;
    }
    
    // Fork a child process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(stdout_backup);
        return 1;
    }
    
    if (pid == 0) {
        // Child process
        
        // Redirect stdout to pipe
        if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
            perror("dup2");
            exit(1);
        }
        close(pipe_fd[0]); // Close read end in child
        
        // Set up shell structure
        t_shell shell;
        shell.command_head = commands;
        shell.token_head = NULL;
        shell.env = create_env(envp ? envp : environ);
        
        if (!shell.env) {
            ft_dprintf(2, "Failed to create environment\n");
            exit(1);
        }
        
        // Execute commands
        int result = execute_commands(commands, &shell);
        
        // Clean up
        if (shell.env)
            free_env(shell.env);
            
        // Make sure output is flushed
        fflush(stdout);
        close(pipe_fd[1]);
        exit(result);
    } 
    else {
        // Parent process
        close(pipe_fd[1]); // Close write end in parent
        
        // Restore stdout
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdout_backup);
        
        // Check if command has output redirection
        bool has_redirection = false;
        t_command *cmd_check = commands;
        while (cmd_check) {
            if (cmd_check->output != NULL) {
                has_redirection = true;
                break;
            }
            cmd_check = cmd_check->next;
        }
        
        // If output is redirected, display a message and use a short timeout
        if (has_redirection) {
            printf("(Output redirected to file)\n");
        }
        
        // Read output with strict timeout
        char buffer[4096];
        ssize_t bytes_read;
        start_time = time(NULL);
        
        // Set non-blocking mode
        int flags = fcntl(pipe_fd[0], F_GETFL, 0);
        fcntl(pipe_fd[0], F_SETFL, flags | O_NONBLOCK);
        
        // Read with timeout
        bool timeout_occurred = false;
        while (!timeout_occurred) {
            bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
            
            if (bytes_read > 0) {
                buffer[bytes_read] = '\0';
                printf("%s", buffer);
                start_time = time(NULL); // Reset timeout on successful read
            } 
            else if (bytes_read == 0) {
                // EOF reached
                break;
            }
            else {
                // Check if we've timed out
                if (time(NULL) - start_time >= TIMEOUT_SECONDS) {
                    timeout_occurred = true;
                } else {
                    usleep(50000); // 50ms sleep to prevent CPU spinning
                }
            }
        }
        
        close(pipe_fd[0]);
        
        // Check if child is still running
        int wait_result = waitpid(pid, &status, WNOHANG);
        if (wait_result == 0) {
            // Child still running, kill it
            printf("Command execution timed out, terminating...\n");
            kill(pid, SIGTERM);
            usleep(100000); // Give it 100ms to terminate
            
            // Force kill if still running
            wait_result = waitpid(pid, &status, WNOHANG);
            if (wait_result == 0) {
                kill(pid, SIGKILL);
                waitpid(pid, &status, 0);
            }
            
            return 1; // Return error for timeout
        }
        
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            return 128 + WTERMSIG(status);
        else
            return 1;
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

