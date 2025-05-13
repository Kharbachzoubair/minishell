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

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For fork(), execvp(), chdir(), etc.

// Declare the environ array (externally defined by the system)
extern char **environ;

/* ------------------------------------------------------------------------- */
/*                               DATA STRUCTURES                             */
/* ------------------------------------------------------------------------- */

/**
 * @brief Struct to hold parsed command arguments.
 *
 * This struct is used to store the arguments of a command after parsing.
 */
typedef struct {
    char *args[10]; // Array to store up to 10 arguments (null-terminated)
} t_cmd;

/* ------------------------------------------------------------------------- */
/*                                BUILTINS                                   */
/* ------------------------------------------------------------------------- */

/**
 * @brief Prints the current working directory.
 *
 * @param cmd Pointer to the t_cmd struct containing the command arguments.
 * @return int Returns 0 on success, non-zero on failure.
 */
int builtin_pwd(t_cmd *cmd);

/**
 * @brief Implements the echo command.
 *
 * Prints the arguments passed to it. Supports the `-n` flag to suppress the
 * trailing newline.
 *
 * @param cmd Pointer to the t_cmd struct containing the command arguments.
 * @return int Returns 0 on success, non-zero on failure.
 */
int builtin_echo(t_cmd *cmd);

/**
 * @brief Prints all environment variables.
 *
 * Iterates through the `environ` array and prints each variable in the format:
 * NAME=VALUE.
 */
void builtin_env();

/**
 * @brief Adds or modifies an environment variable.
 *
 * Accepts input in the format `export VAR_NAME=VALUE`.
 *
 * @param arg The argument passed to the export command.
 * @return int Returns 0 on success, non-zero on failure.
 */
int builtin_export(char *arg);

/**
 * @brief Removes an environment variable.
 *
 * Accepts the name of the variable to unset.
 *
 * @param var_name The name of the variable to remove.
 * @return int Returns 0 on success, non-zero on failure.
 */
int builtin_unset(char *var_name);

/**
 * @brief Changes the current working directory.
 *
 * Supports changing to the home directory (`cd`), previous directory (`cd -`),
 * or a specific path.
 *
 * @param path The target directory path.
 * @return int Returns 0 on success, non-zero on failure.
 */
int builtin_cd(char *path);

/* ------------------------------------------------------------------------- */
/*                              EXTERNAL COMMANDS                            */
/* ------------------------------------------------------------------------- */

/**
 * @brief Executes an external command.
 *
 * Forks a child process and executes the specified command using `execvp`.
 *
 * @param cmd The name of the command to execute.
 * @param args Array of arguments for the command (null-terminated).
 * @return int Returns 0 on success, non-zero on failure.
 */
int execute_external_command(char *cmd, char *args[]);

/* ------------------------------------------------------------------------- */
/*                               HELPER FUNCTIONS                            */
/* ------------------------------------------------------------------------- */

/**
 * @brief Splits a string into arguments.
 *
 * Tokenizes the input string by spaces and stores the tokens in the `args` array.
 *
 * @param input The input string to split.
 * @param args Array to store the resulting tokens (null-terminated).
 * @return int Returns the number of arguments parsed.
 */
int split_args(char *input, char *args[]);

/**
 * @brief Updates the PWD and OLDPWD environment variables.
 *
 * Called after changing directories to reflect the new working directory.
 */
void update_env_vars();

/* ------------------------------------------------------------------------- */
/*                                 ERROR CODES                               */
/* ------------------------------------------------------------------------- */

/**
 * @brief Common error codes used throughout the shell.
 */
void printenv() ;
int my_unsetenv(const char *name) ;
size_t ftstrlen(const char *s) ;
int ftstrcmp(const char *s1, const char *s2);
char *ft_strtok(char *str, const char *delim) ;
int main(void);
#define SUCCESS 0
#define FAILURE 1