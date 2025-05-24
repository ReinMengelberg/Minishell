/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rein <rein@student.codam.nl>                 +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/10 15:21:53 by rein          #+#    #+#                 */
/*   Updated: 2025/05/24 17:41:46 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#define MAX_ARGS 64
#define PROMPT "armadillo $ "

extern char **environ;

t_shell *init_shell()
{
    t_shell *shell;

    shell = (t_shell *)malloc(sizeof(t_shell));
    if (!shell)
        return (NULL);
    
    shell->commands = NULL;
    shell->tokens = NULL;
    shell->env = create_env(environ);
    if (!shell->env)
    {
        free(shell);
        return (NULL);
    }
    shell->exit_status = SUCCESS; // Assuming SUCCESS is defined in minishell.h
    shell->status = 1; // 1 = shell is running
    
    return (shell);
}

int main()
{
    char *input;
    t_shell *shell;

    shell = init_shell();
    if (!shell)
    {
        printf("Error initializing shell\n");
        return (1);
    }
    print_env(shell->env);
    
    while (shell->status)
    {
        // Display prompt and get input using readline
        input = readline(PROMPT);
        
        // Handle EOF or error
        if (input == NULL)
        {
            printf("\n");
            break;
        }
        
        // Skip empty input but add to history if not empty
        if (input[0] != '\0')
        {
            add_history(input);
        }
        else
        {
            free(input);
            continue;
        }
        
        shell->tokens = tokenize(input);
        print_tokens(shell->tokens);
        shell->tokens = expand_tokens(shell->tokens, shell->env, shell->exit_status);
        print_tokens(shell->tokens);
        
        // Free the input string allocated by readline
        free(input);
        
        // Clear tokens between commands
        // TODO: Add proper token cleanup function
    }
    
    // TODO: Add function to free all shell resources
    printf("Shell terminated\n");
    return (shell->exit_status);
}

