/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 00:58:56 by marvin            #+#    #+#             */
/*   Updated: 2025/05/10 00:58:56 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int builtin_unset(char *var_name) {
    if (!var_name || strlen(var_name) == 0) {
        fprintf(stderr, "unset: missing variable name\n");
        return FAILURE;
    }

    if (unsetenv(var_name) == -1) {
        perror("unset");
        return FAILURE;
    }

    return SUCCESS;
}
