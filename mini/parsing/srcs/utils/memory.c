/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:40:09 by absaadan          #+#    #+#             */
/*   Updated: 2025/05/06 09:57:32 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_commands(t_command *head)
{
	t_command	*temp;
	int			i;

	while (head)
	{
		temp = head;
		head = head->next;
		if (temp->name)
			free(temp->name);
		if (temp->args)
		{
			i = 0;
			while (i < temp->arg_count)
			{
				free(temp->args[i]);
				i++;
			}
			free(temp->args);
		}
		free_redirections(temp->redirections);
		if (temp->input_file)
			free(temp->input_file);
		if (temp->output_file)
			free(temp->output_file);
		if (temp->heredoc_delims)
		{
			for (int i = 0; i < temp->heredoc_count; i++)
				free(temp->heredoc_delims[i]);
			free(temp->heredoc_delims);
		}
		free(temp);
	}
}
