/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:36 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/13 13:36:01 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_token	*create_token(t_token_type type, char *value, int has_space,
		int no_expand)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	token->type = type;
	if (value)
	{
		token->value = ft_strdup(value);
		token->has_space = has_space;
		token->no_expand = no_expand;
	}
	else
		token->value = ft_strdup("");
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*temp;

	if (!*head)
		*head = new;
	else
	{
		temp = *head;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}

static const char	*type_name(int type)
{
	static const char	*type_names[] = {"WORD", "PIPE", "REDIR_IN",
		"REDIR_OUT", "APPEND", "HEREDOC", "EOF"};

	if (type >= 0 && type < 7)
		return (type_names[type]);
	return ("UNKNOWN");
}

void	print_tokens(t_token *head)
{
	t_token	*current;
	char	*value;

	current = head;
	while (current)
	{
		if (current->value)
			value = current->value;
		else
			value = "(null)";
		printf("Type: %-10s Value: %-10s has_space=%d\n",
			type_name(current->type),
			value, current->has_space);
		current = current->next;
	}
}

void	free_tokens(t_token *head)
{
	t_token	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->value);
		free(temp);
	}
}
