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
