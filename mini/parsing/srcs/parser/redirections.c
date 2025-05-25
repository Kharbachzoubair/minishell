/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:40:02 by absaadan          #+#    #+#             */
/*   Updated: 2025/05/25 12:35:01 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Create a new redirection
t_redirection *create_redirection(t_token_type type, char *file)
{
    t_redirection *redir = malloc(sizeof(t_redirection));
    if (!redir)
        return NULL;

    redir->type = type;
    redir->file = ft_strdup(file);
	// redir->fd = -1;
    if (!redir->file)
    {
        free(redir);
        return NULL;
    }

    redir->next = NULL;
    return redir;
}

// Add a redirection to the command's list
void add_redirection(t_command *cmd, t_redirection *new_redir)
{
    t_redirection *current;

    if (!cmd->redirections)
    {
        cmd->redirections = new_redir;
        return;
    }

    current = cmd->redirections;
    while (current->next)
        current = current->next;

    current->next = new_redir;
}
void free_redirections(t_redirection *head)
{
    t_redirection *temp;

    while (head)
    {
        temp = head;
        head = head->next;

        free(temp->file);
        free(temp);
    }
}
