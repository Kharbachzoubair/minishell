/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 01:42:59 by marvin            #+#    #+#             */
/*   Updated: 2025/05/10 01:42:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // For fork(), execvp(), getpid()
#include <sys/wait.h> // For waitpid()

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