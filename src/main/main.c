/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 15:21:53 by rein          #+#    #+#                 */
/*   Updated: 2025/05/11 14:37:16 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 64
#define PROMPT "myshell$ "

int main() {
	char input[MAX_INPUT_SIZE];
	char *args[MAX_ARGS];
	int status = 1;

	while (status) {
		// Display prompt
		printf("%s", PROMPT);
		fflush(stdout);

		// Read input
		if (fgets(input, MAX_INPUT_SIZE, stdin) == NULL) {
			break; // Handle EOF (Ctrl+D)
		}

		// Remove trailing newline
		input[strcspn(input, "\n")] = '\0';

		// Exit command
		if (strcmp(input, "exit") == 0) {
			status = 0;
			continue;
		}

		// Parse input into arguments
		int arg_count = 0;
		char *token = strtok(input, " ");

		while (token != NULL && arg_count < MAX_ARGS - 1) {
			args[arg_count++] = token;
			token = strtok(NULL, " ");
		}
		args[arg_count] = NULL; // Null-terminate the argument list

		// Skip if empty command
		if (arg_count == 0) {
			continue;
		}

		// Fork and execute
		pid_t pid = fork();

		if (pid < 0) {
			// Fork error
			perror("fork failed");
		} else if (pid == 0) {
			// Child process
			if (execvp(args[0], args) == -1) {
				perror("command execution failed");
				exit(EXIT_FAILURE);
			}
		} else {
			// Parent process
			waitpid(pid, &status, 0);
		}
	}

	printf("Shell terminated\n");
	return 0;
}

