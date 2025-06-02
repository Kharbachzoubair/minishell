/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:39 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/02 12:01:27 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_operator(char *input, int i, t_token **head, int has_space)
{
	t_token	*tok;

	if (input[i] == '|')
	{
		tok = create_token(TOKEN_PIPE, "|", has_space, 0);
		i++;
	}
	else if (input[i] == '<' && input[i + 1] == '<')
	{
		tok = create_token(TOKEN_HEREDOC, "<<", has_space, 0);
		i += 2;
	}
	else if (input[i] == '<')
	{
		tok = create_token(TOKEN_REDIR_IN, "<", has_space, 0);
		i++;
	}
	else if (input[i] == '>' && input[i + 1] == '>')
	{
		tok = create_token(TOKEN_APPEND, ">>", has_space, 0);
		i += 2;
	}
	else
	{
		tok = create_token(TOKEN_REDIR_OUT, ">", has_space, 0);
		i++;
	}
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

static int	process_whitespace( int i)
{
	return (i + 1);
}

static int	process_operators(char *input, int i, t_token **head, int *space)
{
	int	new_i;

	new_i = handle_operator(input, i, head, *space);
	*space = 0;
	return (new_i);
}

static int	process_quotes(char *input, int i, t_token **head, int *space)
{
	int	new_i;

	new_i = handle_quotes(input, i, head, *space);
	*space = 0;
	return (new_i);
}

t_token	*tokenize(char *input)
{
	t_token	*head;
	int		i;
	int		prev_was_space;

	head = NULL;
	i = 0;
	prev_was_space = 1;
	if (!check_quote_closure(input))
	{
		ft_putstr_fd("Error: Unclosed quotes\n", STDERR_FILENO);
		return (NULL);
	}
	while (input[i])
	{
		if (isspace((unsigned char)input[i]))
		{
				i = process_whitespace(i);
			prev_was_space = 1;
		}
		else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			i = process_operators(input, i, &head, &prev_was_space);
		else if (input[i] == '\'' || input[i] == '"')
			i = process_quotes(input, i, &head, &prev_was_space);
		else
		{
			i = handle_word(input, i, &head, prev_was_space);
			prev_was_space = 0;
		}
	}
	return (head);
}
