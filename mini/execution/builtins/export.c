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

void print_export(t_env *env_list)
{
    t_env *cur;

    cur = env_list;
    while (cur)
    {
        write(1, "declare -x ", 11);
        write(1, cur->key, ft_strlen(cur->key));
        write(1, "=", 1);
        write(1, cur->value, ft_strlen(cur->value));
        write(1, "\n", 1);
        cur = cur->next;
    }
}

int builtin_export(char *arg, t_env *env_list)
{
    char *eq;
    char *key;
    char *val;
    t_env *new;

    if (!arg)
        return (print_export(env_list), 0);
    eq = ft_strchr(arg, '=');
    if (eq)
    {
        key = ft_substr(arg, 0, eq - arg);
        val = ft_strdup(eq + 1);
    }
    else
    {
        key = ft_strdup(arg);
        val = ft_strdup("");
    }
    new = create_env_var(key, val);
    free(key);
    free(val);
    if (!new)
        return (1);
    add_env_var(&env_list, new);
    return (0);
}
