/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 17:02:52 by marvin            #+#    #+#             */
/*   Updated: 2025/05/12 17:02:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H
// # include "../libft/libft.h"//
# include "../parsing/includes/minishell.h"
#include <limits.h>
void restore_stdio(int saved_stdin, int saved_stdout);

/* Setter and getter for the global last-exit status */
void    set_last_exit_status(int status);
int     get_last_exit_status(void);

/* Execute a linked list of parsed commands */
void    execute_commands(t_command *cmds, t_env *env_list);

/* Apply redirections to the command */
void apply_redirs(t_command *c, int *saved_stdin, int *saved_stdout);

/* builtin functions */
int     is_builtin(char *cmd);
int     exec_builtin(t_command *c, t_env *env_list);
int     builtin_echo(char **args);
int     builtin_cd(char *path, t_env *env_list);
int     builtin_pwd(void);
int builtin_export(char *arg, t_env **env_list);
int     builtin_unset(char *var, t_env *env_list);
int     builtin_env(t_env *env_list);
int builtin_exit(char **args);
int read_and_create_heredoc(char *delimiter);
int process_command_heredocs(t_command *cmd, t_env *env_list);
char *read_heredoc_content_working(const char *delimiter, t_env *env_list);

int	ft_strcmp(const char *s1, const char *s2);




#endif
