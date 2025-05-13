/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mora <mel-mora@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:33:07 by absaadan          #+#    #+#             */
/*   Updated: 2025/05/13 18:39:50 by mel-mora         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*create_env_var(char *key, char *value)
{
	t_env	*new_var;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return (NULL);
	new_var->key = ft_strdup(key);
	if (!new_var->key)
	{
		free(new_var);
		return (NULL);
	}
	new_var->value = ft_strdup(value);
	if (!new_var->value)
	{
		free(new_var->key);
		free(new_var);
		return (NULL);
	}
	new_var->next = NULL;
	return (new_var);
}
void	add_env_var(t_env **head, t_env *new_var)
{
	t_env	*current;

	if (!head || !new_var)
		return ;
	/* If variable already exists, update its value */
	current = *head;
	while (current)
	{
		if (strcmp(current->key, new_var->key) == 0)
		{
			free(current->value);
			current->value = strdup(new_var->value);
			/* Free the new node as we're just updating */
			free(new_var->key);
			free(new_var->value);
			free(new_var);
			return ;
		}
		current = current->next;
	}
	/* Add to beginning of list for simplicity */
	new_var->next = *head;
	*head = new_var;
}

void	free_env_list(t_env *head)
{
	t_env	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}
t_env *init_env(char **environ)
{
    t_env *env_list = NULL;

    if (!environ)
        return NULL;

    int i = 0;
    while (environ[i])
    {
        char *eq_pos = strchr(environ[i], '=');
        if (eq_pos)
        {
            /* Split the string at '=' */
            int key_len = eq_pos - environ[i];
            char *key = malloc(key_len + 1);
            if (!key)
                continue;

            strncpy(key, environ[i], key_len);
            key[key_len] = '\0';

            /* Get value (everything after '=') */
            char *value = strdup(eq_pos + 1);
            if (!value)
            {
                free(key);
                continue;
            }

            /* Create and add new env var to list */
            t_env *new_var = create_env_var(key, value);
            if (new_var)
                add_env_var(&env_list, new_var);

            free(key);
            free(value);
        }
        i++;
    }

    return env_list;
}

char *get_env_value(t_env *env_list, char *key)
{
	t_env *current;

	current = env_list;
	if(!key || !*key || !env_list)
		return ft_strdup("");
	while (current)
	{
		if(strcmp(current->key, key) == 0)  // Not current->value
			return (ft_strdup(current->value));
	current = current->next;
	}
	return ft_strdup("");
}
void expand_env_variables_improved(t_token *tokens, t_env *env_list, int last_exit_status)
{
    t_token *current = tokens;

    while (current)
    {
		//   printf("[DEBUG] Processing token: %s, no_expand=%d\n",
        //        current->value, current->no_expand);
        // Only expand if it's a word token, has a value, and isn't marked as no_expand
        if (current->type == TOKEN_WORD && current->value && !current->no_expand)
        {
            char *expanded = expand_env_in_string(current->value, env_list, last_exit_status);
            if (expanded)
            {
                free(current->value);
                current->value = expanded;
            }
        }
        current = current->next;
    }
}

char *replace_substring_2(char *str, int start, int end, char *replacement)
{
    int   prefix_len = start;
    char *new_str    = malloc(prefix_len+ strlen(replacement)+ strlen(str + end)+ 1);
    if (!new_str)
        return str;

    // copy the prefix up to `start`
    strncpy(new_str, str, start);
    new_str[start] = '\0';

    // append the replacement text
    strcat(new_str, replacement);

    // append the original text from `end` onward
    strcat(new_str, str + end);

    free(str);
    return new_str;
}
