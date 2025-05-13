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

#include "shell.h"

int builtin_export(char *arg)
{
    if (!arg || strlen(arg) == 0)
	{
        fprintf(stderr, "export: missing argument\n");
        return FAILURE;
    }

    char *name = strtok(arg, "=");
    char *value = strtok(NULL, "");

    if (!name || strlen(name) == 0)
	{
        fprintf(stderr, "export: invalid variable name\n");
        return FAILURE;
    }

    if (!value) {
        value = "";
    }

    if (setenv(name, value, 1) == -1) {
        perror("export");
        return FAILURE;
    }

    return SUCCESS;
}