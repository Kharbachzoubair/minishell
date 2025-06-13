/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:24 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/13 13:12:57 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H
# include "../libft/libft.h"
# include "minishell.h"
# include "redirection.h"
# define MAX_HEREDOCS 16

typedef struct s_redirection
{
	t_token_type			type;
	char					*file;

	struct s_redirection	*next;
}							t_redirection;

typedef struct s_command
{
	char					*name;
	char					**args;
	int						arg_count;
	char					*input_file;
	char					*output_file;
	int						append_mode;
	char					**heredoc_delims;
	int						heredoc_count;
	int						heredoc_fds[MAX_HEREDOCS];
	t_redirection			*redirections;
	int						type;
	struct s_command		*next;
}							t_command;

t_command					*parse_tokens(t_token *tokens);
t_command					*create_command(void);
void						add_command(t_command **head, t_command *new_cmd);
int							add_argument(t_command *cmd, char *arg);
void						set_redirection(t_command *cmd, t_token *type_token,
								t_token *file_token);
void						free_commands(t_command *head);
void						print_commands(t_command *head);
int							check_quote_closure(char *input);
int							process_single_quotes(char *input, int i,
								t_token **head, int has_space);
int							process_double_quotes(char *input, int i,
								t_token **head, int has_space);

void						add_redirection(t_command *cmd,
								t_redirection *new_redir);
void						free_redirections(t_redirection *head);
t_redirection				*create_redirection(t_token_type type, char *file);
#endif
