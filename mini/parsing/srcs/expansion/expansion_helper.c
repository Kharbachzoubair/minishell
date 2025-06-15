/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 20:44:17 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/15 12:38:22 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*create_env_var(char *key, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->key = ft_strdup(key);
	if (value)
		new->value = ft_strdup(value);
	else
		new->value = NULL;
	new->next = NULL;
	return (new);
}

void	add_env_var(t_env **env_list, t_env *new)
{
	t_env	*tmp;

	tmp = *env_list;
	while (tmp)
	{
		if (strcmp(tmp->key, new->key) == 0)
		{
			free(tmp->value);
			tmp->value = ft_strdup(new->value);
			free(new->key);
			free(new->value);
			free(new);
			return ;
		}
		tmp = tmp->next;
	}
	tmp = *env_list;
	if (!tmp)
		*env_list = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

char	*get_env_value(t_env *env_list, char *key)
{
	t_env	*current;

	current = env_list;
	if (!key || !*key || !env_list)
		return (ft_strdup(""));
	while (current)
	{
		if (strcmp(current->key, key) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (ft_strdup(""));
}

char	*safe_strdup(const char *str)
{
	if (!str)
		return (ft_strdup(""));
	return (ft_strdup(str));
}

int	ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}
