/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pwd.c                                              :+:    :+:            */
/*                                                     +:+                    */
/*   By: ravi-bagin <ravi-bagin@student.codam.nl      +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/05/11 14:36:59 by ravi-bagin    #+#    #+#                 */
/*   Updated: 2025/06/28 16:55:57 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int exec_pwd(t_command *cmd, t_shell *shell)
{
    char current_dir[PATH_MAX];
    
    (void)cmd;   // pwd doesn't use arguments
    (void)shell; // pwd doesn't need shell data
    
    if (!getcwd(current_dir, sizeof(current_dir)))
    {
        perror("pwd");
        return (FAILURE);
    }
    
    printf("%s\n", current_dir);
    return (SUCCESS);
}