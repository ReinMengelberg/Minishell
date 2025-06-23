/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:59 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/23 17:23:32 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_pwd(t_command *cmd, t_env *env_list)
{
    char current_dir[PATH_MAX];
    
    (void)cmd;      // pwd doesn't use arguments
    (void)env_list; // pwd doesn't need environment variables
    
    if (!getcwd(current_dir, sizeof(current_dir)))
    {
        perror("pwd");
        return (FAILURE);
    }
    
    printf("%s\n", current_dir);
    return (SUCCESS);
}