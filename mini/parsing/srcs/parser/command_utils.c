/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:43 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/02 16:39:45 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->name = NULL;
	cmd->args = NULL;
	cmd->arg_count = 0;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->heredoc_delims = NULL;
	cmd->heredoc_count = 0;
	cmd->next = NULL;
	cmd->redirections = NULL;
	return (cmd);
}

void	add_command(t_command **head, t_command *new_command)
{
	t_command	*current;

	if (!*head)
	{
		*head = new_command;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_command;
}

int	add_argument(t_command *cmd, char *arg)
{
	char	**new_args;
	int		i;

	if (!cmd || !arg)
		return (0);
	new_args = malloc((cmd->arg_count + 2) * sizeof *new_args);
	if (!new_args)
		return (0);
	for (i = 0; i < cmd->arg_count; i++)
		new_args[i] = cmd->args[i];
	new_args[cmd->arg_count] = ft_strdup(arg);
	if (!new_args[cmd->arg_count])
	{
		free(new_args);
		return (0);
	}
	new_args[cmd->arg_count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	cmd->arg_count++;
	return (1);
}

static int	handle_token_merging(t_command *current, t_token *tok)
{
	int		last_arg_index;
	char	*merged;

	last_arg_index = current->arg_count - 1;
	merged = merge_token_values(current->args[last_arg_index], tok->value);
	if (!merged)
	{
		fprintf(stderr,
			"minishell: memory allocation error during token merging\n");
		return (0);
	}
	if (!update_command_arg(current, last_arg_index, merged))
	{
		fprintf(stderr,
			"minishell: failed to update argument during token merging\n");
		free(merged);
		return (0);
	}
	free(merged);
	return (1);
}

static t_command	*handle_word_token(t_command *current, t_token *tok)
{
	if (!current)
		current = create_command();
	if (tok->has_space == 0 && current->arg_count > 0)
	{
		if (!handle_token_merging(current, tok))
			return (NULL);
	}
	else
	{
		add_argument(current, tok->value);
		if (current->arg_count == 1)
			current->name = strdup(tok->value);
	}
	return (current);
}

static int	check_pipe_syntax(t_command *current, t_token *tok)
{
	if (!current || !tok->next || tok->next->type == TOKEN_PIPE)
	{
		fprintf(stderr,
			"minishell: syntax error near unexpected token `|`\n");
		return (0);
	}
	return (1);
}

static int	check_redirection_conflict(t_command *current, t_token *tok)
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

static t_token	*handle_redirection_token(t_command **current, t_token *tok)
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
	fprintf(stderr,
		"minishell: syntax error near unexpected token `%s`\n",
		tok->type == TOKEN_APPEND ? ">>" :
		tok->type == TOKEN_REDIR_IN ? "<" : ">");
	return (NULL);
}

static t_token	*handle_heredoc_token(t_command **current, t_token *tok)
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

static void	finalize_current_command(t_command **head, t_command **current)
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

static t_token	*process_single_token(t_command **current, t_command **head,
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
	else if (tok->type == TOKEN_REDIR_IN || tok->type == TOKEN_REDIR_OUT
		|| tok->type == TOKEN_APPEND)
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



