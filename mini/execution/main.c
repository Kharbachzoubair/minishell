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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../parsing/includes/minishell.h"

// Define constants
#define SUCCESS 0
#define FAILURE 1

// Declare the environ array (externally defined by the system)
extern char **environ;

// Struct to hold parsed command arguments
typedef struct {
    char *args[10];
} t_cmd;

void update_env_vars()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("cd: getcwd");
        return;
    }
    setenv("PWD", cwd, 1);
    setenv("OLDPWD", getenv("PWD"), 1);
}

// Built-in Commands
int builtin_pwd(t_cmd *cmd)
{
    char cwd[4096];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
        perror("pwd: getcwd");
        return FAILURE;
    }
    printf("%s\n", cwd);
    return SUCCESS;
}

int builtin_echo(t_cmd *cmd)
{
    int i = 1;
    int no_newline = 0;

    if (cmd->args[1] && strcmp(cmd->args[1], "-n") == 0)
	{
        no_newline = 1;
        i = 2;
    }
    while (cmd->args[i])
	{
        printf("%s", cmd->args[i]);
        if (cmd->args[i + 1])
		{
            printf(" ");
        }
        i++;
    }
    if (!no_newline)
	{
        printf("\n");
    }
    return SUCCESS;
}

void builtin_env()
{
    char **env = environ;
    while (*env) {
        printf("%s\n", *env);
        env++;
    }
}

int builtin_export(char *arg)
{
    if (!arg || strlen(arg) == 0)
	{
        fprintf(stderr, "export: missing argument\n");
        return FAILURE;
    }

    char *name = strtok(arg, "=");
    char *value = strtok(NULL, "");

    if (!name || strlen(name) == 0)
	{
        fprintf(stderr, "export: invalid variable name\n");
        return FAILURE;
    }

    if (!value) {
        value = "";
    }

    if (setenv(name, value, 1) == -1) {
        perror("export");
        return FAILURE;
    }

    return SUCCESS;
}

int builtin_unset(char *var_name) {
    if (!var_name || strlen(var_name) == 0) {
        fprintf(stderr, "unset: missing variable name\n");
        return FAILURE;
    }

    if (unsetenv(var_name) == -1) {
        perror("unset");
        return FAILURE;
    }

    return SUCCESS;
}

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

// External Command Execution
int execute_external_command(char *cmd, char **args)
{
    pid_t pid = fork();

    if (pid == -1)
	{
        perror("fork");
        return FAILURE;
    }
	else if (pid == 0)
	{
        // Child process
        if (execvp(cmd, args) == -1)
		{
            perror("execvp");
            exit(FAILURE);
        }
    }
	else
	{
        // Parent process
        int status;
        if (waitpid(pid, &status, 0) == -1)
		{
            perror("waitpid");
            return FAILURE;
        }

        if (WIFEXITED(status))
		{
            return WEXITSTATUS(status);
        }
		else
		{
            fprintf(stderr, "Child process terminated abnormally\n");
            return FAILURE;
        }
    }

    return SUCCESS;
}

// Main Function
int mini(t_cmd *cmd)
{
        // Check for builtins
        if (strcmp(args[0], "pwd") == 0)
		{
            builtin_pwd(&cmd);
        }
		else if (strcmp(args[0], "echo") == 0)
		{
            builtin_echo(&cmd);
        }
		else if (strcmp(args[0], "env") == 0)
		{
            builtin_env();
        }
		else if (strcmp(args[0], "export") == 0)
		{
            builtin_export(args[1]);
        }
		else if (strcmp(args[0], "unset") == 0)
		{
            builtin_unset(args[1]);
        }
		else if (strcmp(args[0], "cd") == 0)
		{
            builtin_cd(args[1]);
        }
		else
		{
            // Execute external commands
            int result = execute_external_command(args[0], args);
            if (result != SUCCESS)
			{
                fprintf(stderr, "%s: command not found\n", args[0]);
            }
        }

    printf("Exiting...\n");
    return SUCCESS;
}
