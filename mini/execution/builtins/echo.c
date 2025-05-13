/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkharbac <zkharbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 18:17:49 by marvin            #+#    #+#             */
/*   Updated: 2025/05/05 13:34:39 by zkharbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

int builtin_echo(char **args)
{
    int i;
    int n_flag;

    i = 1;
    n_flag = 0;
    if (!args[1])
    {
        write(1, "\n", 1);
        return (0);
    }
    if (args[1] && strcmp(args[1], "-n") == 0)
    {
        n_flag = 1;
        i = 2;
    }
    while (args[i])
    {
        write(1, args[i], ft_strlen(args[i]));
        if (args[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (!n_flag)
        write(1, "\n", 1);
    return (0);
}
