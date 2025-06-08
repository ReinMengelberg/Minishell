/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   cd.c                                               :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:41 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/08 14:19:35 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Helper function to get argument from token list
static char *get_first_arg(t_token *args)
{
    if (!args)
        return (NULL);
    return (args->str);
}

// Helper function to count arguments
static int count_args(t_token *args)
{
    int count = 0;
    t_token *current = args;
    
    while (current)
    {
        count++;
        current = current->next;
    }
    return (count);
}

int execute_cd(t_command *cmd, t_env *env)
{
    char *target_dir;
    char *home_dir;
    char *oldpwd;
    char current_dir[PATH_MAX];
    char new_dir[PATH_MAX];
    int arg_count;
    
    if (!cmd || !env)
        return (ERROR_NULL_POINTER);
    
    // Get current working directory
    if (!getcwd(current_dir, sizeof(current_dir)))
    {
        perror("cd: getcwd failed");
        return (FAILURE);
    }
    
    arg_count = count_args(cmd->args);
    
    // Handle different cases
    if (arg_count == 0)
    {
        // No arguments: cd to HOME
        home_dir = env_get(env, "HOME");
        if (!home_dir)
        {
            fprintf(stderr, "cd: HOME not set\n");
            return (FAILURE);
        }
        target_dir = home_dir;
    }
    else if (arg_count == 1)
    {
        target_dir = get_first_arg(cmd->args);
        
        // Handle "cd -" (go to previous directory)
        if (strcmp(target_dir, "-") == 0)
        {
            oldpwd = env_get(env, "OLDPWD");
            if (!oldpwd)
            {
                fprintf(stderr, "cd: OLDPWD not set\n");
                return (FAILURE);
            }
            target_dir = oldpwd;
            // Print the directory we're going to (bash behavior)
            printf("%s\n", target_dir);
        }
    }
    else
    {
        fprintf(stderr, "cd: too many arguments\n");
        return (FAILURE);
    }
    
    // Attempt to change directory
    if (chdir(target_dir) != 0)
    {
        perror("cd");
        return (FAILURE);
    }
    
    // Get the new current directory (resolves . and .. and symlinks)
    if (!getcwd(new_dir, sizeof(new_dir)))
    {
        perror("cd: getcwd failed after chdir");
        // Try to go back to original directory
        chdir(current_dir);
        return (FAILURE);
    }
    
    // Update OLDPWD to previous directory
    if (update_env_var(env, "OLDPWD", current_dir) != SUCCESS)
    {
        fprintf(stderr, "cd: failed to update OLDPWD\n");
        return (FAILURE);
    }
    
    // Update PWD to new directory
    if (update_env_var(env, "PWD", new_dir) != SUCCESS)
    {
        fprintf(stderr, "cd: failed to update PWD\n");
        return (FAILURE);
    }
    
    return (SUCCESS);
}