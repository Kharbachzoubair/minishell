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

#include "../shell.h"

int builtin_cd(char *path, t_env *env_list)
{
    char buf[PATH_MAX];
    char *oldpwd;

    if (!path || !*path)
        path = get_env_value(env_list, "HOME");
    oldpwd = getcwd(buf, sizeof(buf));
    if (!oldpwd)
        return (1);
    if (chdir(path) == -1)
    {
        perror("cd");
        return (1);
    }
    add_env_var(&env_list, create_env_var("OLDPWD", oldpwd));
    getcwd(buf, sizeof(buf));
    add_env_var(&env_list, create_env_var("PWD", buf));
    return (0);
}
