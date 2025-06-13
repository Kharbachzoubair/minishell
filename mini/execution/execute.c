/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkharbac <zkharbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 18:04:50 by zkharbac          #+#    #+#             */
/*   Updated: 2025/05/19 18:04:50 by zkharbac         ###   ########.fr       */
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


// Process all heredocs for a command (call this after parsing, before execution)


// Read heredoc content and create temp file



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
    if (strcmp(cmd, "exit") == 0)
        return (1);
    return (0);
}
int exec_builtin(t_command *c, t_env *env_list)
{
    int exit_code = 1;
    int saved_stdin, saved_stdout;

    apply_redirs(c, &saved_stdin, &saved_stdout);

    if (strcmp(c->name, "echo") == 0)
        exit_code = builtin_echo(c->args);
    else if (strcmp(c->name, "cd") == 0)
        exit_code = builtin_cd(c->args[1], env_list);
    else if (strcmp(c->name, "pwd") == 0)
        exit_code = builtin_pwd();
    else if (strcmp(c->name, "export") == 0)
        exit_code = builtin_export(c->args[1], &env_list);
    else if (strcmp(c->name, "unset") == 0)
        exit_code = builtin_unset(c->args[1], env_list);
    else if (strcmp(c->name, "env") == 0)
        exit_code = builtin_env(env_list);
    else if (strcmp(c->name, "exit") == 0)
    {
    int result = builtin_exit(c->args);
    if (result == -2)
        return 1;  // Too many arguments, do not exit!
    exit(result);  // Only exit if it was allowed
    }


    restore_stdio(saved_stdin, saved_stdout);
    return (exit_code);
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

void apply_redirs(t_command *cmd, int *saved_stdin, int *saved_stdout)
{
    *saved_stdin = dup(STDIN_FILENO);
    *saved_stdout = dup(STDOUT_FILENO);

    // Apply heredocs FIRST - use the LAST one as stdin (bash behavior)
    if (cmd->heredoc_count > 0)
    {
        int last_heredoc_fd = cmd->heredoc_fds[cmd->heredoc_count - 1];
        if (last_heredoc_fd >= 0)
            dup2(last_heredoc_fd, STDIN_FILENO);
    }

    // Then apply other redirections (can override heredoc)
    t_redirection *r = cmd->redirections;
    while (r)
    {
        int fd = -1;

        if (r->type == TOKEN_REDIR_IN)
            fd = open(r->file, O_RDONLY);
        else if (r->type == TOKEN_REDIR_OUT)
            fd = open(r->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        else if (r->type == TOKEN_APPEND)
            fd = open(r->file, O_CREAT | O_WRONLY | O_APPEND, 0644);

        if (fd >= 0)
        {
            if (r->type == TOKEN_REDIR_IN)
                dup2(fd, STDIN_FILENO);
            else
                dup2(fd, STDOUT_FILENO);
            close(fd);
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


//zoubaaaaaaair
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

void execute_commands(t_command *cmds, t_env *env_list)
{
    int pipefd[2];
    int prev_fd = -1;
    pid_t pid;
    int status;

    while (cmds)
    {
        int is_last = (cmds->next == NULL);

        if (!is_last)
            pipe(pipefd);

        // Run built-in without fork if it is the only command and no pipe
        if (is_builtin(cmds->name) && is_last && prev_fd == -1)
        {
            int saved_stdin, saved_stdout;
            apply_redirs(cmds, &saved_stdin, &saved_stdout);
            int exit_status = exec_builtin(cmds, env_list);
            restore_stdio(saved_stdin, saved_stdout);
            set_last_exit_status(exit_status);
            return;
        }

        pid = fork();
        if (pid == 0)
        {
            int saved_stdin, saved_stdout;
            apply_redirs(cmds, &saved_stdin, &saved_stdout);

            if (prev_fd != -1)
            {
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (!is_last)
            {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]);
                close(pipefd[1]);
            }

            if (is_builtin(cmds->name))
                exit(exec_builtin(cmds, env_list));

            char *path = strchr(cmds->name, '/') ? cmds->name : find_executable(cmds->name, env_list);
            if (!path)
            {
                fprintf(stderr, "%s: command not found\n", cmds->name);
                exit(127);
            }

            if (check_if_folder(path))
                exit(126);

            char **envp = env_list_to_envp(env_list);
            execve(path, cmds->args, envp);
            perror(path);
            ft_free_strarr(envp);
            if (path != cmds->name)
                free(path);
            exit(1);
        }

        // Parent
        if (prev_fd != -1)
            close(prev_fd);
        if (!is_last)
        {
            close(pipefd[1]);
            prev_fd = pipefd[0];
        }

        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            set_last_exit_status(WEXITSTATUS(status));

        cmds = cmds->next;
    }
}
