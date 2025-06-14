/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 17:19:51 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/13 17:20:18 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void	print_commands(t_command *head)
{
	t_command		*current;
	t_redirection	*redir;
	int				i;

	current = head;
	while (current)
	{
		printf("Command: %s\n", current->name);
		printf("Arguments (%d): ", current->arg_count);
		for (i = 0; i < current->arg_count; i++)
			printf("[%s] ", current->args[i]);
		printf("\n");
		redir = current->redirections;
		while (redir)
		{
			if (redir->type == TOKEN_REDIR_IN)
				printf("Input: %s\n", redir->file);
			else if (redir->type == TOKEN_REDIR_OUT)
				printf("Output: %s (append: 0)\n", redir->file);
			else if (redir->type == TOKEN_APPEND)
				printf("Output: %s (append: 1)\n", redir->file);
			else if (redir->type == TOKEN_HEREDOC)
				printf("Heredoc delimiter: %s\n", redir->file);
			redir = redir->next;
		}
		if (current->next)
			printf("| (pipe)\n");
		current = current->next;
	}
}
