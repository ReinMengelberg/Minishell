/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   token.h                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rbagin <rbagin@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/07/14 13:34:10 by rbagin        #+#    #+#                 */
/*   Updated: 2025/07/15 15:09:25 by rein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token		*create_tokens_from_split(char **strings);
t_tokentype	set_tokentype(char *token_str);
char		*remove_quotes(char *str);
void		handle_quote_char(char c, char *quote);
t_tokentype	process_token_type(t_tokentype type, bool *cmd_found,
				bool *next_redir_target, bool *after_redir_file);
void		reset_token_state(bool *cmd_found, bool *next_arg,
				bool *after_file);
t_tokentype	handle_cmd_arg_token(t_tokentype type, bool *cmd_found,
				bool *after_redir_file);
