/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:05:20 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/15 12:52:39 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_pipe_syntax(t_command *current, t_token *tok)
{
	if (!current || !tok->next || tok->next->type == TOKEN_PIPE)
	{
		fprintf(stderr,
			"minishell: syntax error near unexpected token `|`\n");
		return (0);
	}
	return (1);
}

void	finalize_current_command(t_command **head, t_command **current)
{
	if (*current && ((*current)->name || (*current)->heredoc_count > 0))
	{
		if (!(*current)->name && (*current)->heredoc_count > 0)
		{
			(*current)->name = ft_strdup("cat");
			add_argument(*current, "cat");
		}
		add_command(head, *current);
	}
	else
		free(*current);
}

static t_token	*handle_redirection_types(t_command **current, t_token *tok)
{
	if (tok->type >= TOKEN_REDIR_IN && tok->type <= TOKEN_APPEND)
	{
		tok = handle_redirection_token(current, tok);
		if (!tok)
			return (NULL);
	}
	else if (tok->type == TOKEN_HEREDOC)
	{
		tok = handle_heredoc_token(current, tok);
		if (!tok)
			return (NULL);
	}
	return (tok);
}

t_token	*process_single_token(t_command **current, t_command **head,
					t_token *tok)
{
	if (tok->type == TOKEN_WORD)
	{
		*current = handle_word_token(*current, tok);
		if (!*current)
			return (NULL);
	}
	else if (tok->type == TOKEN_PIPE)
	{
		if (!check_pipe_syntax(*current, tok))
			return (NULL);
		add_command(head, *current);
		*current = NULL;
	}
	else
		return (handle_redirection_types(current, tok));
	return (tok);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_token		*tok;

	head = NULL;
	current = NULL;
	tok = tokens;
	while (tok)
	{
		tok = process_single_token(&current, &head, tok);
		if (!tok)
		{
			free_commands(head);
			return (NULL);
		}
		tok = tok->next;
	}
	finalize_current_command(&head, &current);
	return (head);
}
