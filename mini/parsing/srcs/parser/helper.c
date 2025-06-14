/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 17:14:35 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/13 17:15:02 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*merge_token_values(const char *first, const char *second)
{
	size_t	len1;
	size_t	len2;
	char	*merged;

	if (!first || !second)
		return (NULL);
	len1 = strlen(first);
	len2 = strlen(second);
	merged = malloc(len1 + len2 + 1);
	if (!merged)
	{
		perror("malloc in merge_token_values");
		return (NULL);
	}
	ft_memcpy(merged, first, len1);
	ft_memcpy(merged + len1, second, len2);
	merged[len1 + len2] = '\0';
	return (merged);
}

int	update_command_arg(t_command *cmd, int arg_index, char *new_value)
{
	if (!cmd || arg_index < 0 || arg_index >= cmd->arg_count || !new_value)
		return (0);
	free(cmd->args[arg_index]);
	cmd->args[arg_index] = strdup(new_value);
	if (!cmd->args[arg_index])
		return (0);
	if (arg_index == 0)
	{
		free(cmd->name);
		cmd->name = strdup(new_value);
		if (!cmd->name)
			return (0);
	}
	return (1);
}
