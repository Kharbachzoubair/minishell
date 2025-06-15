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

static int	handle_exit_status_expansion(char **result, int offset,
				int last_exit_status)
{
	char	*status_str;
	char	*new_result;

	status_str = ft_itoa(last_exit_status);
	if (!status_str)
		return (0);
	new_result = replace_substring_2(*result, offset, offset + 2, status_str);
	free(status_str);
	if (!new_result)
		return (0);
	free(*result);
	*result = new_result;
	return (1);
}

static int	get_var_name_length(char *start)
{
	char	*p;

	p = start;
	while (*p && (ft_isalnum(*p) || *p == '_'))
		p++;
	return (p - start);
}


static int	handle_variable_expansion(char **result, int offset,
				char *dollar, t_env *env_list)
{
	int		var_len;
	char	*var;
	char	*val;
	char	*new_result;

	var_len = get_var_name_length(dollar + 1);
	if (var_len == 0)
		return (1);
	var = ft_strndup(dollar + 1, var_len);
	if (!var)
		return (0);
	val = get_env_value(env_list, var);
	free(var);
	if (!val)
		val = ft_strdup("");
	if (!val)
		return (0);
	new_result = replace_substring_2(*result, offset, offset + 1 + var_len,
			val);
	free(val);
	if (!new_result)
		return (0);
	free(*result);
	*result = new_result;
	return (1);
}

static int	process_expansion(char **result, char *dollar, t_env *env_list,
				int last_exit_status)
{
	int	offset;

	offset = dollar - *result;
	if (dollar[1] == '?')
		return (handle_exit_status_expansion(result, offset,
				last_exit_status));
	else
		return (handle_variable_expansion(result, offset, dollar, env_list));
}


char	*expand_env_in_string(char *str, t_env *env_list, int last_exit_status)
{
	char	*result;
	char	*dollar;
	char	*pos;
	int		old_offset;

	if (!str || !*str)
		return (ft_strdup(""));
	result = ft_strdup(str);
	if (!result)
		return (NULL);
	pos = result;
	dollar = ft_strchr(pos, '$');
	while (dollar != NULL)
	{
		old_offset = dollar - result;
		if (!process_expansion(&result, dollar, env_list, last_exit_status))
		{
			free(result);
			return (NULL);
		}
		pos = result + old_offset;
		dollar = ft_strchr(pos, '$');
	}
	return (result);
}
