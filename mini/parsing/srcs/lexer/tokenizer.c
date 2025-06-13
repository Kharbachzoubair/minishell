/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:39 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/13 13:34:37 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

static int	process_char(char *input, int i, t_token **head, int *p_space)
{
	if (isspace((unsigned char)input[i]))
	{
		i = process_whitespace(i);
		*p_space = 1;
	}
	else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
		i = process_operators(input, i, head, p_space);
	else if (input[i] == '\'' || input[i] == '"')
		i = process_quotes(input, i, head, p_space);
	else
	{
		i = handle_word(input, i, head, *p_space);
		*p_space = 0;
	}
	return (i);
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
		i = process_char(input, i, &head, &prev_was_space);
	return (head);
}
