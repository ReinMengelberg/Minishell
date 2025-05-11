/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 15:21:53 by rein          #+#    #+#                 */
/*   Updated: 2025/05/11 15:09:19 by rein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define MAX_ARGS 64
#define PROMPT "armadillo $ "

int main() {
    char *input;
    char *args[MAX_ARGS];
    int status = 1;
    
    while (status) {
        // Display prompt and get input using readline
        input = readline(PROMPT);
        
        // Handle EOF or error
        if (input == NULL) {
            printf("\n");
            break;
        }
        
        // Skip empty input but add to history if not empty
        if (input[0] != '\0') {
            add_history(input);
        } else {
            free(input);
            continue;
        }
        
        // Exit command
        if (strcmp(input, "exit") == 0) {
            free(input);
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
            free(input);
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
        
        // Free the input string allocated by readline
        free(input);
    }
    
    printf("Shell terminated\n");
    return 0;
}

