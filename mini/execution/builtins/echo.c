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


#include "shell.h"



int builtin_echo(t_cmd *cmd)
{
    int i = 1;
    int no_newline = 0;

    if (cmd->args[1] && strcmp(cmd->args[1], "-n") == 0)
	{
        no_newline = 1;
        i = 2;
    }
    while (cmd->args[i])
	{
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
		{
            printf(" ");
        }
        i++;
    }
    if (!no_newline)
	{
        printf("\n");
    }
    return SUCCESS;
}
