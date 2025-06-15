/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:53 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/02 12:07:22 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *expand_env_in_string(char *str, t_env *env_list, int last_exit_status)
{
    char *result;
    char *dollar;
    char *pos;
    int   offset, insert_len;

    if (!str || !*str)
        return ft_strdup("");
    result = ft_strdup(str);
    if (!result)
        return NULL;
    pos = result;

    while ((dollar = ft_strchr(pos, '$')) != NULL)
    {
        offset = dollar - result;

        if (dollar[1] == '?')
        {
            char *exit_str = ft_itoa(last_exit_status);
            insert_len = strlen(exit_str);
            result = replace_substring_2(
                result,
                offset,
                offset + 2,
                exit_str
            );
            free(exit_str);
        }
        else
        {

            char *p = dollar + 1;
            while (*p && (isalnum(*p) || *p == '_'))
                p++;
            int   var_len = p - (dollar + 1);
            char *var = strndup(dollar + 1, var_len);
            char *val = get_env_value(env_list, var);
            free(var);

            insert_len = strlen(val);
            result = replace_substring_2(
                result,
                offset,
                offset + 1 + var_len,
                val
            );
            free(val);
        }

        if (!result)
            return NULL;

        pos = result + offset + insert_len;
    }

    return result;
}


char	*replace_exit_status(char *str, char *dollar, char *pos, int exit_code)
{
	char	*exit_str;
	char	*new_str;

	exit_str = ft_itoa(exit_code);
	if (!exit_str)
		return (str);
	new_str = replace_substring(str, dollar, pos + 1, exit_str);
	free(exit_str);
	return (new_str);
}

