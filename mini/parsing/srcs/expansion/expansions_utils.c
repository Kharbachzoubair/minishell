/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:33:07 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/14 20:47:42 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char	*extract_env_key(char *env_line, char *eq_pos)
{
	char	*key;
	int		key_len;

	key_len = eq_pos - env_line;
	key = malloc(key_len + 1);
	if (!key)
		return (NULL);
	ft_strncpy(key, env_line, key_len);
	key[key_len] = '\0';
	return (key);
}

static int	process_env_line(char *env_line, t_env **env_list)
{
	char	*eq_pos;
	char	*key;
	char	*value;
	t_env	*new_var;

	eq_pos = ft_strchr(env_line, '=');
	if (!eq_pos)
		return (1);
	key = extract_env_key(env_line, eq_pos);
	if (!key)
		return (1);
	value = ft_strdup(eq_pos + 1);
	if (!value)
	{
		free(key);
		return (1);
	}
	new_var = create_env_var(key, value);
	if (new_var)
		add_env_var(env_list, new_var);
	free(key);
	free(value);
	return (1);
}

t_env	*init_env(char **environ)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	if (!environ)
		return (NULL);
	i = 0;
	while (environ[i])
	{
		process_env_line(environ[i], &env_list);
		i++;
	}
	return (env_list);
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
