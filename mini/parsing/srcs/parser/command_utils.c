/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:43 by absaadan          #+#    #+#             */
/*   Updated: 2025/05/06 10:23:37 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//  Environment variable expansions

#include "../includes/minishell.h"

t_command *create_command(void)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd) return NULL;

    cmd->name           = NULL;
    cmd->args           = NULL;
    cmd->arg_count      = 0;
    cmd->input_file     = NULL;
    cmd->output_file    = NULL;
    cmd->append_mode    = 0;
    cmd->heredoc_delims = NULL;      // new
    cmd->heredoc_count  = 0;         // new
    cmd->next           = NULL;
	cmd->redirections = NULL;
    return cmd;
}


void	add_command(t_command **head, t_command *new_command)
{
	t_command	*current;

	if (!*head)
	{
		*head = new_command;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_command;
}

int add_argument(t_command *cmd, char *arg) {
    char **new_args;
    int i;

    if (!cmd || !arg)
        return (0);

    // Allocate space for existing args + new arg + NULL terminator (if needed)
    new_args = malloc(sizeof(char *) * (cmd->arg_count + 1)); // +1 for new arg
    if (!new_args)
        return (0);

    // Copy existing args
    i = 0;
    while (i < cmd->arg_count) {
        new_args[i] = cmd->args[i]; // Transfer ownership of pointers
        i++;
    }

    // Add new argument (duplicate the string)
    new_args[cmd->arg_count] = ft_strdup(arg);
    if (!new_args[cmd->arg_count]) {
        free(new_args); // Cleanup on strdup failure
        return (0);
    }

    // Free old args array (if it existed)
    if (cmd->args)
        free(cmd->args);

    // Update command structure
    cmd->args = new_args;
    cmd->arg_count++;
    return (1);
}

void set_redirection(t_command *cmd, t_token *type_token, t_token *file_token)
{
    t_redirection *new_redir;

    // Create redirection and add to list
    new_redir = create_redirection(type_token->type, file_token->value);
    if (!new_redir)
        return;

    add_redirection(cmd, new_redir);

    // For backward compatibility, maintain heredoc array
    if (type_token->type == TOKEN_HEREDOC)
    {
        char **new_list;
        int i;

        new_list = malloc(sizeof(char *) * (cmd->heredoc_count + 2));
        if (!new_list)
            return;
		printf("heredoc count: %d\n", cmd->heredoc_count);
        for (i = 0; i < cmd->heredoc_count; i++)
		{
            new_list[i] = cmd->heredoc_delims[i];
			printf("heredoc: %s\n", new_list[i]);
		}

        new_list[cmd->heredoc_count] = strdup(file_token->value);
        new_list[cmd->heredoc_count + 1] = NULL;

        free(cmd->heredoc_delims);
        cmd->heredoc_delims = new_list;
        cmd->heredoc_count++;
    }
}


// Helper: append one delimiter into cmd->heredoc_delims[]
// static int push_heredoc_delim(t_command *cmd, const char *delim)
// {
//     char **new_list;
//     int     i;

//     // allocate space for old_count+1 items + NULL
//     new_list = malloc(sizeof(char *) * (cmd->heredoc_count + 2));
//     if (!new_list)
//         return (0);

//     // copy over existing pointers
//     for (i = 0; i < cmd->heredoc_count; i++)
//         new_list[i] = cmd->heredoc_delims[i];

//     // add the new one
//     new_list[cmd->heredoc_count] = strdup(delim);
//     new_list[cmd->heredoc_count + 1] = NULL;

//     // free old array (not the strings)
//     free(cmd->heredoc_delims);

//     cmd->heredoc_delims = new_list;
//     cmd->heredoc_count++;
//     return (1);
// }
char *collect_word_argument(t_token **tok_ptr)
{
    t_token *tok = *tok_ptr;
    size_t total_len = 0;
    t_token *cursor = tok;

    // First pass: calculate total length
    while (cursor && cursor->type == TOKEN_WORD)
    {
        total_len += strlen(cursor->value);
        if (cursor->has_space)
            break;
        cursor = cursor->next;
    }

    // Allocate and build the merged string
    char *merged = malloc(total_len + 1);
    if (!merged)
    {
        perror("malloc");
        return NULL;
    }
    merged[0] = '\0';

    // Second pass: concatenate values
    while (*tok_ptr && (*tok_ptr)->type == TOKEN_WORD)
    {
        strcat(merged, (*tok_ptr)->value);
        if ((*tok_ptr)->has_space)
        {
            *tok_ptr = (*tok_ptr)->next;
            break;
        }
        *tok_ptr = (*tok_ptr)->next;
    }

    return merged;
}
char *merge_token_values(const char *first, const char *second)
{
    if (!first || !second)
        return NULL;

    size_t len1 = strlen(first);
    size_t len2 = strlen(second);
    char *merged = malloc(len1 + len2 + 1);

    if (!merged) {
        perror("malloc in merge_token_values");
        return NULL;
    }

    // Use memcpy for efficiency instead of strcpy/strcat
    ft_memcpy(merged, first, len1);
    ft_memcpy(merged + len1, second, len2);
    merged[len1 + len2] = '\0';

    return merged;
}
int update_command_arg(t_command *cmd, int arg_index, char *new_value)
{
    if (!cmd || arg_index < 0 || arg_index >= cmd->arg_count || !new_value)
        return 0;

    free(cmd->args[arg_index]);
    cmd->args[arg_index] = strdup(new_value);

    if (!cmd->args[arg_index])
        return 0;

    // If this is the first argument, also update command name
    if (arg_index == 0) {
        free(cmd->name);
        cmd->name = strdup(new_value);
        if (!cmd->name)
            return 0;
    }

    return 1;
}
t_command *parse_tokens(t_token *tokens)
{
    t_command *head    = NULL;
    t_command *current = NULL;
    t_token   *tok     = tokens;

    while (tok)
    {
        if (tok->type == TOKEN_WORD)
{
    if (!current)
        current = create_command();

   if (tok->has_space == 0 && current->arg_count > 0)
{
    // Get index of the last argument
    int last_arg_index = current->arg_count - 1;

    // Merge with previous argument
    char *merged = merge_token_values(current->args[last_arg_index], tok->value);
    if (!merged) {
        fprintf(stderr, "minishell: memory allocation error during token merging\n");
        free_commands(head);
        return NULL;
    }

    // Update the argument in the command
    if (!update_command_arg(current, last_arg_index, merged)) {
        fprintf(stderr, "minishell: failed to update argument during token merging\n");
        free(merged);
        free_commands(head);
        return NULL;
    }

    free(merged);
}
    else
    {
        // First word becomes name
        add_argument(current, tok->value);
        if (current->arg_count == 1)
            current->name = strdup(tok->value);
    }
}
        else if (tok->type == TOKEN_PIPE)
        {
            if (current)
                add_command(&head, current);
            current = NULL;
        }
        else if (tok->type == TOKEN_REDIR_IN
              || tok->type == TOKEN_REDIR_OUT
              || tok->type == TOKEN_APPEND)
        {
            if (!current)
                current = create_command();
            if (tok->next && tok->next->type == TOKEN_WORD)
            {
                set_redirection(current, tok, tok->next);
                tok = tok->next;
            }
            else
            {
                fprintf(stderr,
                    "minishell: syntax error near unexpected token `%s`\n",
                    tok->type == TOKEN_APPEND ? ">>" :
                    tok->type == TOKEN_REDIR_IN ? "<" : ">");
                free_commands(head);
                return NULL;
            }
        }
       else if (tok->type == TOKEN_HEREDOC)
{
    if (!current)
        current = create_command();
    if (tok->next && tok->next->type == TOKEN_WORD)
    {
        // Call set_redirection instead of push_heredoc_delim
        set_redirection(current, tok, tok->next);
        tok = tok->next;
    }
    else
    {
        fprintf(stderr,
            "minishell: syntax error: no heredoc delimiter\n");
        free_commands(head);
        return NULL;
    }
}
        tok = tok->next;
    }

    if (current && (current->name || current->heredoc_count > 0))
        add_command(&head, current);
    else
        free(current);

    return head;
}



void print_commands(t_command *head)
{
    t_command *current = head;
    t_redirection *redir;
    int i;

    while (current)
    {
        printf("Command: %s\n", current->name);

        printf("Arguments (%d): ", current->arg_count);
        for (i = 0; i < current->arg_count; i++)
            printf("[%s] ", current->args[i]);
        printf("\n");

        /* Print all redirections */
        redir = current->redirections;
        while (redir)
        {
            if (redir->type == TOKEN_REDIR_IN)
                printf("Input: %s\n", redir->file);
            else if (redir->type == TOKEN_REDIR_OUT)
                printf("Output: %s (append: 0)\n", redir->file);
            else if (redir->type == TOKEN_APPEND)
                printf("Output: %s (append: 1)\n", redir->file);
            else if (redir->type == TOKEN_HEREDOC)
                printf("Heredoc delimiter: %s\n", redir->file);

            redir = redir->next;
        }

        if (current->next)
            printf("| (pipe)\n");

        current = current->next;
    }
}
void free_commands(t_command *head)
{
    t_command *temp;
    int i;

    while (head)
    {
        temp = head;          // Store the current node
        head = head->next;    // Move to the next node

        /* Free the command name */
        if (temp->name)
            free(temp->name);

        /* Free all arguments */
        if (temp->args)
        {
            i = 0;
            while (i < temp->arg_count)
            {
                free(temp->args[i]);    // Free each argument
                i++;
            }
            free(temp->args);           // Free the array of arguments
        }

        /* Free redirection filenames */
        if (temp->input_file)
            free(temp->input_file);
        if (temp->output_file)
            free(temp->output_file);
        if (temp->heredoc_delims)
		{
			 for (int i = 0; i < temp->heredoc_count; i++)
        free(temp->heredoc_delims[i]);
    free(temp->heredoc_delims);
		}


        /* Free the current node */
        free(temp);
    }
}
