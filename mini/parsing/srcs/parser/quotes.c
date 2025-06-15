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

#include "../includes/minishell.h"

int	check_quote_closure(char *input)
{
	int		i;
	char	quote_type;
	int		inside_quotes;

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

static char	*extract_quoted_content(char *input, int start, int end)
{
	char	*value;
	int		len;

	len = end - start;
	value = malloc(len + 1);
	if (!value)
		return (NULL);
	ft_strncpy(value, input + start, len);
	value[len] = '\0';
	return (value);
}

static int	create_and_add_quote_token(char *value, t_token **head,
		int has_space, int is_single_quote)
{
	t_token	*tok;

	tok = create_token(TOKEN_WORD, value, has_space, is_single_quote);
	if (tok)
		add_token(head, tok);
	return (1);
}

int	process_single_quotes(char *input, int i, t_token **head, int has_space)
{
	int		start;
	int		j;
	char	*value;

	start = i + 1;
	j = start;
	while (input[j] && input[j] != '\'')
		j++;
	if (!input[j])
		return (j);
	value = extract_quoted_content(input, start, j);
	if (!value)
		return (j + 1);
	create_and_add_quote_token(value, head, has_space, 1);
	free(value);
	return (j + 1);
}

int	process_double_quotes(char *input, int i, t_token **head, int has_space)
{
	int		start;
	int		j;
	char	*value;

	start = i + 1;
	j = start;
	while (input[j] && input[j] != '"')
		j++;
	if (!input[j])
	{
		fprintf(stderr, "minishell: syntax error: unclosed double quote\n");
		return (j);
	}
	value = extract_quoted_content(input, start, j);
	if (!value)
		return (j + 1);
	create_and_add_quote_token(value, head, has_space, 0);
	free(value);
	return (j + 1);
}
