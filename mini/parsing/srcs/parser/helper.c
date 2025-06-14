/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 17:14:35 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/14 20:35:34 by absaadan         ###   ########.fr       */
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

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	if (!dest || !src)
		return (dest);
	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

char	*ft_strcat(char *dest, const char *src)
{
	size_t	dest_len;
	size_t	i;

	if (!dest || !src)
		return (dest);
	dest_len = 0;
	while (dest[dest_len])
		dest_len++;
	i = 0;
	while (src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}

char	*ft_strcpy(char *dest, const char *src)
{
	size_t	i;

	if (!dest || !src)
		return (dest);
	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}
