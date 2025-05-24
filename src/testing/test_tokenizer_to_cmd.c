/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   test_tokenizer_to_cmd.c                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/21 14:48:30 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/05/22 14:00:02 by ravi-bagin    ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

/* test_tokenizer.c */
#include "minishell.h"
#include <stdio.h>

void print_commands(t_command *cmd);
void free_commands(t_command *commands);

void run_tokenizer_test(char *input, char *test_name) {
    printf("\n\033[1;34m=== TEST: %s ===\033[0m\n", test_name);
    printf("Input: '%s'\n\n", input);
    
    // Tokenize the input
    t_token *tokens = tokenize(input);
    
    if (!tokens) {
        printf("\033[1;31mTokenization failed or input rejected\033[0m\n");
        return;
    }
    
    // Print the tokens
    printf("Tokenization Result:\n");
    print_tokens(tokens);
    
    // Extract commands
    printf("\nCommand Extraction Result:\n");
    t_command *commands = extract_commands(tokens);
    
    if (!commands) {
        printf("\033[1;31mCommand extraction failed\033[0m\n");
        free_tokens(tokens);
        return;
    }
    
    // Process redirections (without actually opening files)
    printf("\nTesting Redirection Processing:\n");
    bool redir_result = process_redirections(commands);
    printf("Redirection processing %s\n", redir_result ? 
           "\033[1;32msucceeded\033[0m" : "\033[1;31mfailed\033[0m");
    
    // Print command structure
    printf("\nCommand Structure:\n");
    print_commands(commands);
    
    // Clean up
    free_commands(commands);
    free_tokens(tokens);
    printf("\033[1;32mTest completed\033[0m\n");
}

// Helper function to print command structure
void print_commands(t_command *cmd) {
    int cmd_num = 1;
    
    while (cmd) {
        printf("Command %d:\n", cmd_num++);
        printf("  Command: %s\n", cmd->cmd ? cmd->cmd->str : "NULL");
        
        printf("  Arguments: ");
        t_token *arg = cmd->args;
        while (arg && arg->type == ARG) {
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

// Helper function to clean up commands
void free_commands(t_command *commands) {
    t_command *tmp;
    
    while (commands) {
        tmp = commands;
        commands = commands->next;
        free(tmp);
    }
}

void test_quote_handling() {
    printf("\n\033[1;34m=== TESTING QUOTE HANDLING ===\033[0m\n");
    
    char *test_inputs[] = {
        "echo \"hello world\"",
        "echo 'hello world'",
        "echo \"hello 'nested' world\"",
        "echo 'hello \"nested\" world'",
        "echo \"$HOME\"",  // Should expand
        "echo '$HOME'",    // Should not expand
        NULL
    };
    
    for (int i = 0; test_inputs[i]; i++) {
        printf("\nTest: '%s'\n", test_inputs[i]);
        
        // Tokenize
        t_token *tokens = tokenize(test_inputs[i]);
        if (!tokens) {
            printf("Tokenization failed\n");
            continue;
        }
        
        print_tokens(tokens);
        free_tokens(tokens);
    }
}

int main() {
    printf("\033[1;33m***** MINISHELL TOKENIZER & PARSER TESTS *****\033[0m\n");
    
    // Simple command tests
    run_tokenizer_test("ls -la", "Simple command with arguments");
    run_tokenizer_test("echo hello world", "Echo with multiple arguments");
    
    // Redirection tests
    run_tokenizer_test("ls > output.txt", "Output redirection");
    run_tokenizer_test("cat < input.txt", "Input redirection");
    run_tokenizer_test("echo hello >> append.txt", "Append redirection");
    run_tokenizer_test("cat << EOF", "Heredoc redirection");
    
    // Pipeline tests
    run_tokenizer_test("ls | grep a", "Simple pipe");
    run_tokenizer_test("ls -la | grep a | wc -l", "Multiple pipes");
    
    // Complex command tests
    run_tokenizer_test("echo \"quoted text with spaces\"", "Double quotes");
    run_tokenizer_test("echo 'single quoted text'", "Single quotes");
    run_tokenizer_test("echo \"mixed 'quotes' here\"", "Mixed quotes");
    run_tokenizer_test("echo $HOME", "Environment variable");
    
    // Complex pipeline tests
    run_tokenizer_test("cat < input.txt | grep pattern | sort > output.txt", 
                       "Pipe with redirections");
    
    // Edge cases
    run_tokenizer_test("", "Empty input");
    run_tokenizer_test("     ", "Whitespace only");
    run_tokenizer_test(">", "Redirection without file");
    run_tokenizer_test("|", "Pipe without commands");
	run_tokenizer_test("< input.txt >output.txt cat", "Delayed command");
    
    // Should be rejected by syntax check
    run_tokenizer_test("ls & echo", "Background operations (should fail)");
    run_tokenizer_test("ls; echo", "Command separator (should fail)");
    run_tokenizer_test("echo hello\\world", "Backslash (should fail)");
    run_tokenizer_test("echo \"unclosed quotes", "Unclosed quotes (should fail)");
    
    return 0;
}
