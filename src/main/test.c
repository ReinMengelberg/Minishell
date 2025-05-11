/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 16:10:44 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/05/11 16:12:33 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdbool.h>

#define PROMPT "tokenizer test $ "

int main() {
    char *input;
    int status = 1;
    
    printf("Tokenizer Test - Enter commands to see tokenization\n");
    printf("Type 'exit' to quit\n\n");
    
    while (status) {
        // Display prompt and get input using readline
        input = readline(PROMPT);
        
        // Handle EOF or error
        if (input == NULL) {
            printf("\nExiting...\n");
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
        
        // Use the tokenizer instead of strtok
        printf("\n--- Tokenizing: '%s' ---\n", input);
        t_token *tokens = tokenize(input);
        
        if (!tokens) {
            printf("Error: Tokenization failed\n");
        } else {
            // Print tokens for debugging
            print_tokens(tokens);
            
            // Clean up tokens
            free_tokens(tokens);
        }
        
        // Free the input string allocated by readline
        free(input);
        printf("\n");
    }
    
    printf("Tokenizer test completed\n");
    return 0;
}

