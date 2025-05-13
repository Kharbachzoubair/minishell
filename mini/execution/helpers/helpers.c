/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 19:00:42 by marvin            #+#    #+#             */
/*   Updated: 2025/05/12 19:00:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
// Function to update PWD and OLDPWD environment variables
void update_env_vars() {
    char cwd[1024]; // Buffer to store the current working directory

    // Get the current working directory
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("cd: getcwd");
        return;
    }

    // Update PWD with the new directory
    setenv("PWD", cwd, 1);

    // Update OLDPWD with the previous directory
    setenv("OLDPWD", getenv("PWD"), 1);
}
void printenv()
{
    char **env = environ;
    while (*env) {
        printf("%s\n", *env);
        env++;
    }
}
int ftstrcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}


static char *ft_strtok_state = NULL; // Static variable to store state between calls

char *ft_strtok(char *str, const char *delim)
{
    if (str != NULL) {
        ft_strtok_state = str; // Initialize state with the input string
    }

    if (ft_strtok_state == NULL || *ft_strtok_state == '\0') {
        return NULL; // No more tokens
    }

    // Skip leading delimiters
    while (*ft_strtok_state && strchr(delim, *ft_strtok_state)) {
        ft_strtok_state++;
    }

    if (*ft_strtok_state == '\0') {
        return NULL; // No more tokens
    }

    // Start of the token
    char *token_start = ft_strtok_state;

    // Find the end of the token
    while (*ft_strtok_state && !strchr(delim, *ft_strtok_state)) {
        ft_strtok_state++;
    }

    // If we found a delimiter, terminate the token
    if (*ft_strtok_state) {
        *ft_strtok_state = '\0'; // Insert null terminator
        ft_strtok_state++;       // Move state to the next character
    } else {
        ft_strtok_state = NULL; // End of string
    }

    return token_start;
}
size_t ftstrlen(const char *s) {
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

int my_unsetenv(const char *name)
{
    // Validate input
    if (!name || strlen(name) == 0 || strchr(name, '=') != NULL) {
        fprintf(stderr, "unset: invalid variable name\n");
        return -1; // Invalid variable name
    }

    int found = 0; // Flag to track if the variable was found

    // Iterate through the environ array using a while loop
    char **env = environ;
    while (*env) {
        // Check if the current entry matches the variable name
        if (strncmp(*env, name, strlen(name)) == 0 && (*env)[strlen(name)] == '=') {
            found = 1;

            // Shift all subsequent entries one position up using a while loop
            char **p = env;
            while (*p) {
                *p = *(p + 1);
                p++;
            }

            break; // Stop after removing the variable
        }
        env++;
    }

    if (!found) {
        // Variable not found; do nothing (silent behavior)
        return 0;
    }

    return 0; // Success
}
