/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:33:07 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/14 20:31:18 by absaadan         ###   ########.fr       */
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
	new->value = value ? ft_strdup(value) : NULL;
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

t_env	*init_env(char **environ)
{
	t_env	*env_list;
	int		i;
	char	*eq_pos;
	int		key_len;
	char	*key;
	char	*value;
	t_env	*new_var;

	env_list = NULL;
	if (!environ)
		return (NULL);
	i = 0;
	while (environ[i])
	{
		eq_pos = strchr(environ[i], '=');
		if (eq_pos)
		{
			key_len = eq_pos - environ[i];
			key = malloc(key_len + 1);
			if (!key)
				continue ;
			ft_strncpy(key, environ[i], key_len);
			key[key_len] = '\0';
			value = strdup(eq_pos + 1);
			if (!value)
			{
				free(key);
				continue ;
			}
			new_var = create_env_var(key, value);
			if (new_var)
				add_env_var(&env_list, new_var);
			free(key);
			free(value);
		}
		i++;
	}
	return (env_list);
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
void	expand_env_variables_improved(t_token *tokens, t_env *env_list,
		int last_exit_status)
{
	t_token	*current;
	t_token	*prev;
	char	*expanded;

	current = tokens;
	prev = NULL;
	while (current)
	{
		if (current->type == TOKEN_WORD && current->value && !current->no_expand
			&& !(prev && prev->type == TOKEN_HEREDOC))
		{
			expanded = expand_env_in_string(current->value, env_list,
					last_exit_status);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		prev = current;
		current = current->next;
	}
}

char	*replace_substring_2(char *str, int start, int end, char *replacement)
{
	int		prefix_len;
	char	*new_str;

	prefix_len = start;
	new_str = malloc(prefix_len + strlen(replacement) + strlen(str + end) + 1);
	if (!new_str)
		return (str);
	ft_strncpy(new_str, str, start);
	new_str[start] = '\0';
	ft_strcat(new_str, replacement);
	ft_strcat(new_str, str + end);
	free(str);
	return (new_str);
}
