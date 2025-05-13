/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkharbac <zkharbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:36:35 by zkharbac          #+#    #+#             */
/*   Updated: 2025/05/05 13:38:37 by zkharbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

int builtin_pwd(void)
{
    char buf[PATH_MAX];

    if (!getcwd(buf, sizeof(buf)))
    {
        perror("pwd");
        return (1);
    }
    write(1, buf, ft_strlen(buf));
    write(1, "\n", 1);
    return (0);
}
