<<<<<<< HEAD
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 15:00:00 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/02 12:09:09 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"


int create_heredoc_tmpfile(const char *content)
{
    int fd;
    char template[] = "/tmp/heredoc_XXXXXX";

    fd = mkstemp(template);
    if (fd == -1)
        return (-1);

    unlink(template);

    if (write(fd, content, strlen(content)) == -1)
    {
        close(fd);
        return (-1);
    }

    lseek(fd, 0, SEEK_SET); 
    return (fd);
}

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
    {
        write(STDOUT_FILENO, "> ", 2);

        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            printf("warning: heredoc delimited by end-of-file (wanted `%s`)\n", delimiter);
            free(content);
            return (NULL);
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, delimiter) == 0)
            break;


        char *expanded_line = expand_env_in_string(buffer, env_list, get_last_exit_status());
        if (!expanded_line)
            expanded_line = strdup(buffer);

        size_t line_len = strlen(expanded_line) + 1;
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

        free(expanded_line);
    }

    return content;
}


int process_command_heredocs(t_command *cmd, t_env *env_list)
{
    int i = 0;

    if (!cmd || cmd->heredoc_count == 0)
        return (0);


    while (i < cmd->heredoc_count)
    {
        char *content = read_heredoc_content_working(cmd->heredoc_delims[i], env_list);
        if (!content)
            return (-1);


        cmd->heredoc_fds[i] = create_heredoc_tmpfile(content);
        free(content);

        if (cmd->heredoc_fds[i] == -1)
            return (-1);

        i++;
    }
    return (0);
}
=======
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkharbac <zkharbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 16:11:20 by zkharbac          #+#    #+#             */
/*   Updated: 2025/05/19 16:14:18 by zkharbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declare your functions here (copy-paste them or #include from execute.c)
// For demo, I’ll just declare prototypes:


char *read_heredoc_content(const char *delimiter)
{
    char buffer[1024];
    size_t capacity = 2048;
    size_t length = 0;
    char *content = malloc(capacity);

    if (!content)
        return (perror("malloc"), NULL);
    
    content[0] = '\0';

    while (1)
    {
        printf("> ");
        if (!fgets(buffer, sizeof(buffer), stdin)) // Ctrl+D
        {
            printf("warning: heredoc delimited by end-of-file (wanted `%s`)\n", delimiter);
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0'; // remove newline

        if (strcmp(buffer, delimiter) == 0)
            break;

        size_t line_len = strlen(buffer) + 1; // +1 for '\n'
        if (length + line_len + 1 >= capacity) // +1 for final '\0'
        {
            capacity *= 2;
            char *new_content = realloc(content, capacity);
            if (!new_content)
            {
                free(content);
                return (perror("realloc"), NULL);
            }
            content = new_content;
        }

        strcat(content, buffer);
        strcat(content, "\n");
        length += line_len;
    }

    return content;
}

FILE *write_herdroc_to_tmpfile(const char *content)
{
    FILE *tmpfile_ptr=tmpfile();
    if(!tmpfile_ptr)
    {
        perror("tmpfile");
        return (NULL);
    }
    size_t bytes_written =fwrite(content, 1, strlen(content), tmpfile_ptr);
    if(bytes_written!=strlen(content))
    {
        perror("fwrite");
        fclose(tmpfile_ptr);
        return NULL;
    }
    rewind (tmpfile_ptr);
    return (tmpfile_ptr);
}
int main(void)
{
    const char *delimiter = "END";
    printf("Enter heredoc content, end with delimiter `%s`:\n", delimiter);

    char *content = read_heredoc_content(delimiter);
    if (!content)
    {
        fprintf(stderr, "Failed to read heredoc content\n");
        return 1;
    }

    FILE *tmpfile_ptr = write_herdroc_to_tmpfile(content);
    free(content);
    if (!tmpfile_ptr)
    {
        fprintf(stderr, "Failed to write heredoc to tmpfile\n");
        return 1;
    }

    printf("Heredoc content written to temp file. Now reading it back:\n\n");

    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), tmpfile_ptr))
    {
        fputs(buffer, stdout);
    }

    fclose(tmpfile_ptr);
    return 0;
}
>>>>>>> 58aedb750c6be8ae3fc66432ab16f261f3caa143
