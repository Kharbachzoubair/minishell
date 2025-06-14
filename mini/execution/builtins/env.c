/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 01:30:11 by marvin            #+#    #+#             */
/*   Updated: 2025/05/10 01:30:11 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/env.h"

int builtin_env(t_env *env_list)
{
    t_env *cur = env_list;

    while (cur)
    {
        if (cur->value) // Only print if the variable has a value
        {
            write(1, cur->key, ft_strlen(cur->key));
            write(1, "=", 1);
            write(1, cur->value, ft_strlen(cur->value));
            write(1, "\n", 1);
        }
        cur = cur->next;
    }
    return (0);
}