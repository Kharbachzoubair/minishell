/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkharbac <zkharbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:24 by absaadan          #+#    #+#             */
/*   Updated: 2025/05/18 19:22:59 by zkharbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H
# include "../libft/libft.h"
#include "minishell.h"
/* Command structure */
#include "redirection.h"
typedef struct s_redirection
{
    t_token_type	type;          // Use your existing token type enum
    char *file;                 // Filename or heredoc delimiter
    struct s_redirection *next; // Next redirection in list
} t_redirection;

typedef struct s_command
{
    char            *name;           /* Command name (first word) */
    char            **args;          /* Command arguments (including name) */
    int             arg_count;       /* Number of arguments */
    char            *input_file;     /* Input redirection filename */
    char            *output_file;    /* Output redirection filename */
    int             append_mode;     /* 1 if >> is used, 0 otherwise */
    char           **heredoc_delims;  /* Array of heredoc delimiters */
    int                 heredoc_count;
	t_redirection *redirections;
    int             type;
    struct s_command *next;          /* Next command in pipeline */
} t_command;


t_command *parse_tokens(t_token *tokens);
t_command *create_command(void);
void add_command(t_command **head, t_command *new_cmd);
int add_argument(t_command *cmd, char *arg);
void set_redirection(t_command *cmd, t_token *type_token, t_token *file_token);
void free_commands(t_command *head);
void print_commands(t_command *head);
/* Function for quotes */
int check_quote_closure(char *input);
int process_single_quotes(char *input, int i,t_token **head, int has_space);
int process_double_quotes(char *input, int i, t_token **head, int has_space);


void add_redirection(t_command *cmd, t_redirection *new_redir);
void free_redirections(t_redirection *head);
t_redirection *create_redirection(t_token_type type, char *file);
#endif
