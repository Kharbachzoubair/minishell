/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkharbac <zkharbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 12:27:05 by absaadan          #+#    #+#             */
/*   Updated: 2025/05/19 03:26:37 by zkharbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H
# include "../libft/libft.h"
# include "minishell.h"

typedef struct s_env
{
    char            *key;       /* Variable name */
    char            *value;     /* Variable value */
    struct s_env    *next;      /* Pointer to next env variable */
} t_env;


t_env   *init_env(char **environ);
t_env   *create_env_var(char *key, char *value);
void    add_env_var(t_env **head, t_env *new_var);
void    free_env_list(t_env *head);

t_env *init_env(char **environ);
char *get_env_value(t_env *env_list, char *key);
char	*replace_substring(char *str, char *start, char *end, char *replacement);
char *replace_substring_2(char *original,int   start,int   end,char *replacement);

// char *expand_single_var(char *str, int *i, t_env *env_list, int last_exit_status);
char *expand_env_in_string(char *str, t_env *env_list, int last_exit_status);
void expand_env_variables(t_token *tokens, t_env *env_list, int last_exit_status);
void expand_env_variables_improved(t_token *tokens, t_env *env_list, int last_exit_status);
// the use function
void	expand_env_variables(t_token *tokens, t_env *env_list, int last_exit_status);
char	*extract_and_expand_var(char *var_str, t_env *env_list);
char	*append_remaining_str(char *value, char *remainder);
char	*expand_env_in_string(char *str, t_env *env_list, int last_exit_status);
char	*replace_exit_status(char *str, char *dollar, char *pos, int exit_code);
char	*replace_env_var(char *str, char *dollar, char *pos, t_env *env_list);
char    **env_list_to_envp(t_env *env_list);
void    ft_free_strarr(char **arr);
t_env *sort_env_list(t_env *env_list);
t_env *find_env_var(t_env *env_list, const char *key);
#endif

