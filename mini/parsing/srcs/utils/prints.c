/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 17:19:51 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/15 12:45:39 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_env_list(t_env *env_list)
{
	t_env	*current;

	current = env_list;
	printf("Environment variables:\n");
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}

static void	print_redirection(t_redirection *redir)
{
	if (redir->type == TOKEN_REDIR_IN)
		printf("Input: %s\n", redir->file);
	else if (redir->type == TOKEN_REDIR_OUT)
		printf("Output: %s (append: 0)\n", redir->file);
	else if (redir->type == TOKEN_APPEND)
		printf("Output: %s (append: 1)\n", redir->file);
	else if (redir->type == TOKEN_HEREDOC)
		printf("Heredoc delimiter: %s\n", redir->file);
}

static void	print_command_info(t_command *current)
{
	t_redirection	*redir;
	int				i;

	printf("Command: %s\n", current->name);
	printf("Arguments (%d): ", current->arg_count);
	i = 0;
	while (i < current->arg_count)
	{
		printf("[%s] ", current->args[i]);
		i++;
	}
	printf("\n");
	redir = current->redirections;
	while (redir)
	{
		print_redirection(redir);
		redir = redir->next;
	}
	if (current->next)
		printf("| (pipe)\n");
}

void	print_commands(t_command *head)
{
	t_command	*current;

	current = head;
	while (current)
	{
		print_command_info(current);
		current = current->next;
	}
}
