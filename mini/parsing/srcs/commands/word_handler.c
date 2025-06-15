/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 18:32:41 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/14 19:02:34 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	handle_token_merging(t_command *current, t_token *tok)
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

t_command	*handle_word_token(t_command *current, t_token *tok)
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
