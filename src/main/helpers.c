/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   helpers.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rmengelb <rmengelb@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/06/30 17:01:51 by rmengelb      #+#    #+#                 */
/*   Updated: 2025/07/14 15:52:50 by rmengelb      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_args(t_token *args)
{
	int		count;
	t_token	*arg;

	count = 0;
	arg = args;
	while (arg && arg->type == ARG)
	{
		count++;
		arg = arg->next;
	}
	return (count);
}

// void	print_commands(t_command *cmd)
// {
// 	int		cmd_num;
// 	t_token	*arg;

// 	cmd_num = 1;
// 	while (cmd)
// 	{
// 		printf("Command %d:\n", cmd_num++);
// 		printf("  Command: %s\n", cmd->cmd ? cmd->cmd->str : "NULL");
// 		printf("  Arguments: ");
// 		arg = cmd->args;
// 		while (arg && arg->type == ARG)
// 		{
// 			printf("'%s' ", arg->str);
// 			arg = arg->next;
// 		}
// 		printf("\n");
// 		printf("  Input: %s\n",
// 			cmd->input ? cmd->input->str : "No token(STDIN)");
// 		if (cmd->input && cmd->input->next)
// 			printf("  Input File: %s\n", cmd->input->next->str);
// 		printf("  Output: %s\n",
// 			cmd->output ? cmd->output->str : "No token(STDOUT)");
// 		if (cmd->output && cmd->output->next)
// 			printf("  Output File: %s\n", cmd->output->next->str);
// 		printf("  Is Piped: %s\n", cmd->is_piped ? "Yes" : "No");
// 		printf("  FDs: in=%d, out=%d\n", cmd->in_fd, cmd->out_fd);
// 		cmd = cmd->next;
// 	}
// }
