/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:59 by absaadan          #+#    #+#             */
/*   Updated: 2025/05/13 20:56:08 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//  Quote handling
#include "../includes/minishell.h"


int check_quote_closure(char *input)
{
    int i;
    char quote_type;
    int inside_quotes;

    i = 0;
    inside_quotes = 0;
    quote_type = 0;

    while (input[i])
    {
        if ((input[i] == '\'' || input[i] == '"') && !inside_quotes)
        {
            inside_quotes = 1;
            quote_type = input[i];
        }
        else if (inside_quotes && input[i] == quote_type)
        {
            inside_quotes = 0;
        }
        i++;
    }

    return (!inside_quotes);
}

int process_single_quotes(char *input,
                          int i,
                          t_token **head,
                          int has_space)
{
    int start = i + 1;
    int j     = start;
    char *value;

    // find closing quote
    while (input[j] && input[j] != '\'')
        j++;
    if (!input[j])
        return j;  // unclosed → let caller handle error

    // extract
    value = malloc(j - start + 1);
    if (!value)
        return (j + 1);
    strncpy(value, input + start, j - start);
    value[j - start] = '\0';

    // create and add exactly one token
    t_token *tok = create_token(TOKEN_WORD, value, has_space ,1);
    if (tok)
    {
        add_token(head, tok);
    }
    free(value);

    return (j + 1);
}

int process_double_quotes(char *input,int i,t_token **head,int has_space)
{
    int start = i + 1;
    int j     = start;
    char *value;

    // find closing quote
    while (input[j] && input[j] != '"')
        j++;
    if (!input[j])
    {
        fprintf(stderr, "minishell: syntax error: unclosed double quote\n");
        return j;
    }

    // extract
    value = malloc(j - start + 1);
    if (!value)
        return (j + 1);
    strncpy(value, input + start, j - start);
    value[j - start] = '\0';

    // create and add exactly one token
    t_token *tok = create_token(TOKEN_WORD, value, has_space , 0);
    if (tok)
    {
        add_token(head, tok);
    }
    free(value);

    return (j + 1);
}


