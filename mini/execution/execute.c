/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:35:07 by marvin            #+#    #+#             */
/*   Updated: 2025/04/26 13:35:07 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

void restore_stdio(int saved_stdin, int saved_stdout);
static int  g_last_exit_status;

void    set_last_exit_status(int status)
{
    g_last_exit_status = status;
}

int     get_last_exit_status(void)
{
    return (g_last_exit_status);
}

 int  is_builtin(char *cmd)
{
    if (strcmp(cmd, "echo") == 0)
        return (1);
    if (strcmp(cmd, "cd") == 0)
        return (1);
    if (strcmp(cmd, "pwd") == 0)
        return (1);
    if (strcmp(cmd, "export") == 0)
        return (1);
    if (strcmp(cmd, "unset") == 0)
        return (1);
    if (strcmp(cmd, "env") == 0)
        return (1);
    return (0);
}

int  exec_builtin(t_command *c, t_env *env_list)
{
    int saved_stdin, saved_stdout;

    apply_redirs(c, &saved_stdin, &saved_stdout);
    if (strcmp(c->name, "echo") == 0)
        return (builtin_echo(c->args));
    if (strcmp(c->name, "cd") == 0)
        return (builtin_cd(c->args[1], env_list));
    if (strcmp(c->name, "pwd") == 0)
        return (builtin_pwd());
    if (strcmp(c->name, "export") == 0)
        return (builtin_export(c->args[1], env_list));
    if (strcmp(c->name, "unset") == 0)
        return (builtin_unset(c->args[1], env_list));
    if (strcmp(c->name, "env") == 0)
        return (builtin_env(env_list));
    restore_stdio(saved_stdin, saved_stdout);
    return (1);
}


/* Search PATH for executable; returns malloc'd path or NULL */
 char *find_executable(char *cmd, t_env *env_list)
{
    char    *path_env;
    char    **dirs;
    char    *full;
    int     i;

    path_env = get_env_value(env_list, "PATH");
    if (!path_env)
        return (NULL);
    dirs = ft_split(path_env, ':');
    free(path_env);
    if (!dirs)
        return (NULL);
    i = 0;
    while (dirs[i])
    {
        full = malloc(strlen(dirs[i]) + 1 + strlen(cmd) + 1);
        if (!full)
            break;
        strcpy(full, dirs[i]);
        strcat(full, "/");
        strcat(full, cmd);
        if (access(full, X_OK) == 0)
        {
            ft_free_strarr(dirs);
            return (full);
        }
        free(full);
        i++;
    }
    ft_free_strarr(dirs);
    return (NULL);
}

void apply_redirs(t_command *c, int *saved_stdin, int *saved_stdout)
{
    t_redirection *r = c->redirections;

    *saved_stdin=dup(STDIN_FILENO);
    *saved_stdout=dup(STDOUT_FILENO);

    while(r)
    {
        int fd =-1;

        if(r->type == TOKEN_REDIR_IN)
            fd=open(r->file, O_RDONLY);
        else if(r->type == TOKEN_REDIR_OUT)
            fd=open(r->file,O_CREAT | O_WRONLY | O_TRUNC, 0644 );
        else if(r->type == TOKEN_APPEND)
            fd=open(r->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
        
        if(fd>= 0)
        {
            if(r ->type ==TOKEN_REDIR_IN)
                dup2(fd, STDIN_FILENO);
            else
                dup2(fd, STDOUT_FILENO);
            close (fd);
        }
        r = r->next;
    }
}
void restore_stdio(int saved_stdin, int saved_stdout)
{
    if (saved_stdin != -1)
    {
        dup2(saved_stdin, STDIN_FILENO);
        close(saved_stdin);
    }
    if (saved_stdout != -1)
    {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
}

int	check_if_folder(char *cmd)
{
	struct stat	statbuf;

	if (stat(cmd, &statbuf) == -1)
	{
		set_last_exit_status(127);
		return (2);
	}
	if (statbuf.st_mode & S_IFDIR)
	{
		printf("%s: Is a directory\n", cmd);
		set_last_exit_status(126);
		return (1);
	}
	return (0);
}

 int  exec_node(t_command *c, t_env *env_list)
{
    pid_t       pid;
    int         status;
    char      **envp;
    char        *path;
    int         saved_stdin;
    int         saved_stdout;

    envp = env_list_to_envp(env_list);
    pid = fork();
    if (pid == 0)
    {
        apply_redirs(c, &saved_stdin, &saved_stdout);
        if (strchr(c->name, '/'))
            path = c->name;
        else
            path = find_executable(c->name, env_list);
        if (!path)
        {
            fprintf(stderr, "%s: command not found\n", c->name);
            exit(127);
        }
        if (check_if_folder(path))
        {
            free(path);
            exit(126);
        }
        execve(path, c->args, envp);
        perror(path);
        exit(EXIT_FAILURE);
    }
    waitpid(pid, &status, 0);
    ft_free_strarr(envp);
    if (WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (1);
}

void    execute_commands(t_command *cmds, t_env *env_list)
{
    int status;

    while (cmds)
    {
        if (is_builtin(cmds->name))
            status = exec_builtin(cmds, env_list);
        else
            status = exec_node(cmds, env_list);
        set_last_exit_status(status);
        cmds = cmds->next;
    }
}
