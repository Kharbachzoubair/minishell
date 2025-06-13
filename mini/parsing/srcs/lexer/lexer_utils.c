<<<<<<< HEAD
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:32 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/13 13:37:44 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "lexer.h"

int	handle_operator(char *input, int i, t_token **head, int has_space)
{
	t_token	*tok;

	if (input[i] == '|')
		tok = create_token(TOKEN_PIPE, "|", has_space, 0), i++;
	else if (input[i] == '<' && input[i + 1] == '<')
		tok = create_token(TOKEN_HEREDOC, "<<", has_space, 0), i += 2;
	else if (input[i] == '<')
		tok = create_token(TOKEN_REDIR_IN, "<", has_space, 0), i++;
	else if (input[i] == '>' && input[i + 1] == '>')
		tok = create_token(TOKEN_APPEND, ">>", has_space, 0), i += 2;
	else
		tok = create_token(TOKEN_REDIR_OUT, ">", has_space, 0), i++;
	add_token(head, tok);
	return (i);
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
	strncpy(value, input + start, j - start);
	value[j - start] = '\0';
	add_token(head, create_token(TOKEN_WORD, value, has_space, 0));
	free(value);
	return (j);
}
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:32 by absaadan          #+#    #+#             */
/*   Updated: 2025/04/12 16:12:43 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//   Converts input string to tokens
>>>>>>> 58aedb750c6be8ae3fc66432ab16f261f3caa143
