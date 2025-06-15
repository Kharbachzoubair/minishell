/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:40:06 by absaadan          #+#    #+#             */
/*   Updated: 2025/04/12 15:40:07 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_redirections(t_redirection *head)
{
	t_redirection	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->file);
		free(temp);
	}
}
