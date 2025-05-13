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

#include "shell.h"

int builtin_pwd(t_cmd *cmd)
{
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
        perror("pwd: getcwd");
        return FAILURE;
    }
    printf("%s\n", cwd);
    return SUCCESS;
}