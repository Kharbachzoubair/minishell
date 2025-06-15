/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:42:30 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/14 18:42:44 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_redirection_conflict(t_command *current, t_token *tok)
{
	t_redirection	*redir;

	redir = current->redirections;
	while (redir)
	{
		if ((redir->type == TOKEN_REDIR_IN && tok->type == TOKEN_REDIR_IN)
			|| (redir->type == TOKEN_REDIR_OUT && tok->type == TOKEN_REDIR_OUT)
			|| (redir->type == TOKEN_APPEND && tok->type == TOKEN_APPEND))
		{
			fprintf(stderr, "minishell: ambiguous redirect\n");
			return (0);
		}
		redir = redir->next;
	}
	return (1);
}

static char	*get_redirection_error_token(t_token *tok)
{
	if (tok->type == TOKEN_APPEND)
		return (">>");
	if (tok->type == TOKEN_REDIR_IN)
		return ("<");
	return (">");
}

t_token	*handle_redirection_token(t_command **current, t_token *tok)
{
	if (!*current)
		*current = create_command();
	if (!check_redirection_conflict(*current, tok))
		return (NULL);
	if (tok->next && tok->next->type == TOKEN_WORD)
	{
		set_redirection(*current, tok, tok->next);
		return (tok->next);
	}
	fprintf(stderr, "minishell: syntax error near unexpected token `%s`\n",
		get_redirection_error_token(tok));
	return (NULL);
}

t_token	*handle_heredoc_token(t_command **current, t_token *tok)
{
	if (!*current)
		*current = create_command();
	if (tok->next && tok->next->type == TOKEN_WORD)
	{
		set_redirection(*current, tok, tok->next);
		return (tok->next);
	}
	fprintf(stderr, "minishell: syntax error: missing heredoc delimiter\n");
	return (NULL);
}
