/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 00:45:05 by marvin            #+#    #+#             */
/*   Updated: 2025/05/09 00:45:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"


int builtin_cd(char *path) {
    if (!path || strlen(path) == 0) {
        path = getenv("HOME");
        if (!path) {
            fprintf(stderr, "cd: HOME not set\n");
            return FAILURE;
        }
    }

    if (strcmp(path, "-") == 0) {
        path = getenv("OLDPWD");
        if (!path) {
            fprintf(stderr, "cd: OLDPWD not set\n");
            return FAILURE;
        }
        printf("%s\n", path);
    }

    if (chdir(path) == -1) {
        perror("cd");
        return FAILURE;
    }

    update_env_vars();
    return SUCCESS;
}
