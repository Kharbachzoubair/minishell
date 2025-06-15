/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:32 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/14 20:30:08 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "lexer.h"

static void	create_and_add_token(t_token_type type, char *value,
						t_token **head, int has_space)
{
	t_token	*tok;

	tok = create_token(type, value, has_space, 0);
	add_token(head, tok);
}

int	handle_operator(char *input, int i, t_token **head, int has_space)
{
	if (input[i] == '|')
	{
		create_and_add_token(TOKEN_PIPE, "|", head, has_space);
		return (i + 1);
	}
	else if (input[i] == '<' && input[i + 1] == '<')
	{
		create_and_add_token(TOKEN_HEREDOC, "<<", head, has_space);
		return (i + 2);
	}
	else if (input[i] == '<')
	{
		create_and_add_token(TOKEN_REDIR_IN, "<", head, has_space);
		return (i + 1);
	}
	else if (input[i] == '>' && input[i + 1] == '>')
	{
		create_and_add_token(TOKEN_APPEND, ">>", head, has_space);
		return (i + 2);
	}
	create_and_add_token(TOKEN_REDIR_OUT, ">", head, has_space);
	return (i + 1);
}

int	handle_quotes(char *input, int i, t_token **head, int has_space)
{
	if (input[i] == '\'')
		return (process_single_quotes(input, i, head, has_space));
	else
		return (process_double_quotes(input, i, head, has_space));
}

int	handle_word(char *input, int i, t_token **head, int has_space)
{
	int		start;
	int		j;
	char	*value;

	start = i;
	j = i;
	while (input[j] && !isspace(input[j]) && !strchr("|<>'\"", input[j]))
		j++;
	value = malloc(j - start + 1);
	if (!value)
		return (j);
	ft_strncpy(value, input + start, j - start);
	value[j - start] = '\0';
	add_token(head, create_token(TOKEN_WORD, value, has_space, 0));
	free(value);
	return (j);
}
