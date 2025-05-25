/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:14 by absaadan          #+#    #+#             */
/*   Updated: 2025/05/03 14:15:27 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include "../libft/libft.h"

typedef enum e_token_type
{
    TOKEN_WORD,     // arguments / name
    TOKEN_PIPE,     // |
    TOKEN_REDIR_IN, // <
    TOKEN_REDIR_OUT, // >
    TOKEN_APPEND,   // >>
    TOKEN_HEREDOC,  // <<
    TOKEN_EOF       // End of input
} t_token_type;

typedef struct s_token
{
	t_token_type	type; // Type of token
	char			*value;
	int				has_space; // Value of token
	int             no_expand;
	struct s_token	*next; // Pointer to the next token
}	t_token;

t_token *tokenize(char *input);
t_token *create_token(t_token_type type , char *value,int has_space,int no_expand);
void    add_token(t_token **head, t_token *new_token);
void    free_tokens(t_token *head);
void print_tokens(t_token *head);
#endif
