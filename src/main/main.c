/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 15:21:53 by rein          #+#    #+#                 */
/*   Updated: 2025/05/24 17:26:08 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define MAX_ARGS 64
#define PROMPT "armadillo $ "

extern char **environ;

int main() {
    char *input;
    int status = 1;
    t_env   *env;
    t_token *tokens;

    env = create_env(environ);
    print_env(env);
    
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
        
        tokens = tokenize(input);
        print_tokens(tokens);
        tokens = expand_tokens(tokens, env, SUCCESS); // TODO: replace SUCCESS with last exit command.
        print_tokens(tokens);

        
        // Free the input string allocated by readline
        free(input);
    }
    
    printf("Shell terminated\n");
    return 0;
}

