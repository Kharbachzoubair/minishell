/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:00:00 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/01 11:30:17 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

// Create temporary file with heredoc content
int create_heredoc_tmpfile(const char *content)
{
    int fd;
    char template[] = "/tmp/heredoc_XXXXXX";

    fd = mkstemp(template);
    if (fd == -1)
        return (-1);

    unlink(template); // Auto-cleanup when fd closed

    if (write(fd, content, strlen(content)) == -1)
    {
        close(fd);
        return (-1);
    }

    lseek(fd, 0, SEEK_SET); // Reset to beginning for reading
    return (fd);
}

// Updated heredoc reader with environment variable expansion
char *read_heredoc_content_working(const char *delimiter, t_env *env_list)
{
    char buffer[1024];
    size_t capacity = 2048;
    size_t length = 0;
    char *content = malloc(capacity);

    if (!content)
        return (NULL);

    content[0] = '\0';

    while (1)
    {       // Your working prompt
        write(STDOUT_FILENO, "> ", 2);        // Make sure it shows

        if (!fgets(buffer, sizeof(buffer), stdin)) // Your working input method
        {
            printf("warning: heredoc delimited by end-of-file (wanted `%s`)\n", delimiter);
            free(content);
            return (NULL);
        }

        buffer[strcspn(buffer, "\n")] = '\0'; // remove newline - your working method

        if (strcmp(buffer, delimiter) == 0)   // Your working comparison
            break;

        // NEW: Expand environment variables in the line
        char *expanded_line = expand_env_in_string(buffer, env_list, get_last_exit_status());
        if (!expanded_line)
            expanded_line = strdup(buffer); // Fallback to original if expansion fails

        // Your working append logic (but use expanded_line instead of buffer)
        size_t line_len = strlen(expanded_line) + 1; // +1 for '\n'
        if (length + line_len + 1 >= capacity)
        {
            capacity *= 2;
            char *new_content = realloc(content, capacity);
            if (!new_content)
            {
                free(expanded_line);
                free(content);
                return (NULL);
            }
            content = new_content;
        }

        strcat(content, expanded_line);
        strcat(content, "\n");
        length += line_len;

        free(expanded_line); // Clean up the expanded line
    }

    return content;
}

// Updated process function to accept env_list parameter
int process_command_heredocs(t_command *cmd, t_env *env_list)
{
    int i = 0;

    if (!cmd || cmd->heredoc_count == 0)
        return (0);

    // Process each heredoc in order
    while (i < cmd->heredoc_count)
    {
        char *content = read_heredoc_content_working(cmd->heredoc_delims[i], env_list);
        if (!content)
            return (-1); // EOF hit, abort command

        // Create temp file for this heredoc
        cmd->heredoc_fds[i] = create_heredoc_tmpfile(content);
        free(content);

        if (cmd->heredoc_fds[i] == -1)
            return (-1);

        i++;
    }
    return (0);
}
