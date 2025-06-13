/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 01:20:50 by marvin            #+#    #+#             */
/*   Updated: 2025/05/10 01:20:50 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

int is_valid_identifier(const char *str)
{
	int i = 0;

	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void print_export(t_env *env_list)
{
	t_env *tmp = env_list;

	while (tmp)
	{
		printf("declare -x %s", tmp->key);
		if (tmp->value)
			printf("=\"%s\"", tmp->value);
		printf("\n");
		tmp = tmp->next;
	}
}

int builtin_export(char *arg, t_env **env_list)
{
	if (!arg)
	{
		print_export(*env_list);
		return (0);
	}
	if (!is_valid_identifier(arg))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		set_last_exit_status(1);
		return (1);
	}

	char *equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		// Split key and value
		int key_len = equal_sign - arg;
		char *key = ft_substr(arg, 0, key_len);
		char *value = ft_strdup(equal_sign + 1);
		if (!key || !value)
			return (1);

		// Check if variable exists, update value
		t_env *tmp = *env_list;
		while (tmp)
		{
			if (strcmp(tmp->key, key) == 0)
			{
				free(tmp->value);
				tmp->value = value;
				free(key);
				set_last_exit_status(0);
				return (0);
			}
			tmp = tmp->next;
		}

		// If doesn't exist, create and add
		t_env *new_var = create_env_var(key, value);
		add_env_var(env_list, new_var);
		free(key);
		free(value);
	}
	else
	{
		// Just export a key with NULL value if it doesn’t exist
		t_env *tmp = *env_list;
		while (tmp)
		{
			if (strcmp(tmp->key, arg) == 0)
				return (0); // already exists
			tmp = tmp->next;
		}
		t_env *new_var = create_env_var(arg, NULL);
		add_env_var(env_list, new_var);
	}

	set_last_exit_status(0);
	return (0);
}
