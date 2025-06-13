/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkharbac <zkharbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 19:27:26 by zkharbac          #+#    #+#             */
/*   Updated: 2025/06/13 16:30:14 by zkharbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "shell.h"

static int is_numeric(const char *str)
{
    int i = 0;
    if (!str)
        return 0;
    if (str[0] == '-' || str[0] == '+')
        i++;
    while (str[i])
    {
        if (!isdigit(str[i]))
            return 0;
        i++;
    }
    return 1;
}

int builtin_exit(char **args)
{
    int exit_status = get_last_exit_status();

    if (args[1])  // There is at least one argument
    {
        if (!is_numeric(args[1]))
        {
            // If the first argument is NOT a valid number, exit with 255
            fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
            exit(255);
        }
        // Add this near the top of builtin_exit
        if (args[1] && args[2])
        {
            fprintf(stderr, "exit: too many arguments\n");
            return -2;  // signal: don't exit
        }

        exit_status = atoi(args[1]);  // Only 1 valid numeric arg
    }

    // ✅ Only exit if args[1] exists and args[2] doesn't
    exit(exit_status & 0xFF);
}

