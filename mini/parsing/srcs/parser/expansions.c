/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:53 by absaadan          #+#    #+#             */
/*   Updated: 2025/05/05 11:56:18 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void print_env_list(t_env *env_list)
{
    t_env *current = env_list;
    printf("Environment variables:\n");
    while (current)
    {
        printf("%s=%s\n", current->key, current->value);
        current = current->next;
    }
}

/* Safely duplicate a string */
char *safe_strdup(const char *str)
{
    if (!str)
        return strdup("");
    return strdup(str);
}

/* Expand environment variables in a token list */
#include <string.h>  // for strchr

void expand_env_variables(t_token *tokens, t_env *env_list, int last_exit_status)
{
    t_token *current = tokens;
    char    *expanded;

    while (current)
    {
        // Debug the token and its no_expand flag
        printf("[DEBUG] Processing token: %s, no_expand=%d\n",
               current->value, current->no_expand);

        // Only expand if it's a word token, has a value, contains '$', and isn't no_expand
        if (current->type == TOKEN_WORD && current->value &&
            !current->no_expand && strchr(current->value, '$'))
        {
            expanded = expand_env_in_string(current->value, env_list, last_exit_status);
            if (expanded)
            {
                free(current->value);
                current->value = expanded;
            }
        }
        current = current->next;
    }
}


char	*extract_and_expand_var(char *var_str, t_env *env_list)
{
	char	*var_name;
	char	*expanded;
	int		name_len;

	name_len = 0;
	while (var_str[name_len] && ((var_str[name_len] >= 'a' && var_str[name_len] <= 'z') ||
		(var_str[name_len] >= 'A' && var_str[name_len] <= 'Z') ||
		(var_str[name_len] >= '0' && var_str[name_len] <= '9') ||
		var_str[name_len] == '_'))
		name_len++;
	var_name = malloc(name_len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, var_str, name_len + 1);
	expanded = get_env_value(env_list, var_name);
	if (var_str[name_len])
		expanded = append_remaining_str(expanded, &var_str[name_len]);
	free(var_name);
	return (expanded);
}

char	*append_remaining_str(char *value, char *remainder)
{
	char	*temp;
	char	*result;

	temp = value;
	result = malloc(ft_strlen(temp) + ft_strlen(remainder) + 1);
	if (!result)
		return (temp);
	ft_strlcpy(result, temp, ft_strlen(temp) + 1);
	ft_strlcat(result, remainder, ft_strlen(temp) + ft_strlen(remainder) + 1);
	free(temp);
	return (result);
}

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
        // 1) record where the '$' was
        offset = dollar - result;

        // 2) decide which replacement and its length
        if (dollar[1] == '?')
        {
            char *exit_str = ft_itoa(last_exit_status);
            insert_len = strlen(exit_str);
            result = replace_substring_2(
                result,
                offset,
                offset + 2,    // skip over "$?"
                exit_str
            );
            free(exit_str);
        }
        else
        {
            // find end of var name
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
                offset + 1 + var_len,  // skip "$" + var name
                val
            );
            free(val);
        }

        if (!result)
            return NULL;

        // 3) advance pos in the new buffer
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

char	*replace_env_var(char *str, char *dollar, char *pos, t_env *env_list)
{
	char	*var_start;
	char	*var_name;
	char	*value;
	int		name_len;

	var_start = pos;
	while (*pos && ((*pos >= 'a' && *pos <= 'z') ||
			(*pos >= 'A' && *pos <= 'Z') ||
			(*pos >= '0' && *pos <= '9') ||
			*pos == '_'))
		pos++;
	if (var_start == pos)
		return (str);
	name_len = pos - var_start;
	var_name = ft_substr(var_start, 0, name_len);
	if (!var_name)
		return (str);
	value = get_env_value(env_list, var_name);
	free(var_name);
	return (replace_substring(str, dollar, pos, value));
}

char	*replace_substring(char *str, char *start, char *end, char *replacement)
{
	int		prefix_len;
	char	*new_str;

	prefix_len = start - str;
	new_str = malloc(prefix_len + ft_strlen(replacement) + ft_strlen(end) + 1);
	if (!new_str)
		return (str);
	ft_strlcpy(new_str, str, prefix_len + 1);
	ft_strlcat(new_str, replacement, prefix_len + ft_strlen(replacement) + 1);
	ft_strlcat(new_str, end, prefix_len + ft_strlen(replacement) +
		ft_strlen(end) + 1);
	free(str);
	return (new_str);
}
