/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 00:58:56 by marvin            #+#    #+#             */
/*   Updated: 2025/05/10 00:58:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

int	builtin_unset(char *key, t_env **env_list)
{
	t_env	*cur;
	t_env	*prev;

	if (!key || !*key || !env_list)
		return (1);
	cur = *env_list;
	prev = NULL;
	while (cur)
	{
		if (strcmp(cur->key, key) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*env_list = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return (0);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}
