
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:43 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/02 13:44:40 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//  Environment variable expansions

#include "../includes/minishell.h"

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->name = NULL;
	cmd->args = NULL;
	cmd->arg_count = 0;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->heredoc_delims = NULL;
	cmd->heredoc_count = 0;
	cmd->next = NULL;
	cmd->redirections = NULL;
	return (cmd);
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

int	add_argument(t_command *cmd, char *arg)
{
	char	**new_args;
	int		i;

	if (!cmd || !arg)
		return (0);
	new_args = malloc((cmd->arg_count + 2) * sizeof *new_args);
	if (!new_args)
		return (0);
	for (i = 0; i < cmd->arg_count; i++)
		new_args[i] = cmd->args[i];
	new_args[cmd->arg_count] = ft_strdup(arg);
	if (!new_args[cmd->arg_count])
	{
		free(new_args);
		return (0);
	}
	new_args[cmd->arg_count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	cmd->arg_count++;
	return (1);
}

void	set_redirection(t_command *cmd, t_token *type_token,
		t_token *file_token)
{
	t_redirection	*new_redir;
	char			**new_list;
	int				i;

	new_redir = create_redirection(type_token->type, file_token->value);
	if (!new_redir)
		return ;
	add_redirection(cmd, new_redir);
	if (type_token->type == TOKEN_HEREDOC)
	{
		new_list = malloc(sizeof(char *) * (cmd->heredoc_count + 2));
		if (!new_list)
			return ;
		for (i = 0; i < cmd->heredoc_count; i++)
		{
			new_list[i] = ft_strdup(cmd->heredoc_delims[i]);
			if (!new_list[i])
			{
				while (--i >= 0)
					free(new_list[i]);
				free(new_list);
				return ;
			}
		}
		new_list[cmd->heredoc_count] = ft_strdup(file_token->value);
		if (!new_list[cmd->heredoc_count])
		{
			for (i = 0; i < cmd->heredoc_count; i++)
				free(new_list[i]);
			free(new_list);
			return ;
		}
		new_list[cmd->heredoc_count + 1] = NULL;

		// FREE OLD STRINGS BEFORE REPLACING THE ARRAY
		if (cmd->heredoc_delims)
		{
			for (int j = 0; j < cmd->heredoc_count; j++)
				free(cmd->heredoc_delims[j]);
		}
		free(cmd->heredoc_delims);

		cmd->heredoc_delims = new_list;
		cmd->heredoc_count++;
	}
}

char	*merge_token_values(const char *first, const char *second)
{
	size_t	len1;
	size_t	len2;
	char	*merged;

	if (!first || !second)
		return (NULL);
	len1 = strlen(first);
	len2 = strlen(second);
	merged = malloc(len1 + len2 + 1);
	if (!merged)
	{
		perror("malloc in merge_token_values");
		return (NULL);
	}
	// Use memcpy for efficiency instead of strcpy/strcat
	ft_memcpy(merged, first, len1);
	ft_memcpy(merged + len1, second, len2);
	merged[len1 + len2] = '\0';
	return (merged);
}
int	update_command_arg(t_command *cmd, int arg_index, char *new_value)
{
	if (!cmd || arg_index < 0 || arg_index >= cmd->arg_count || !new_value)
		return (0);
	free(cmd->args[arg_index]);
	cmd->args[arg_index] = strdup(new_value);
	if (!cmd->args[arg_index])
		return (0);
	// If this is the first argument, also update command name
	if (arg_index == 0)
	{
		free(cmd->name);
		cmd->name = strdup(new_value);
		if (!cmd->name)
			return (0);
	}
	return (1);
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command		*head;
	t_command		*current;
	t_token			*tok;
	int				last_arg_index;
	char			*merged;
	t_redirection	*redir;

	head = NULL;
	current = NULL;
	tok = tokens;
	while (tok)
	{
		if (tok->type == TOKEN_WORD)
		{
			if (!current)
				current = create_command();
			if (tok->has_space == 0 && current->arg_count > 0)
			{
				last_arg_index = current->arg_count - 1;
				merged = merge_token_values(current->args[last_arg_index],
						tok->value);
				if (!merged)
				{
					fprintf(stderr,
						"minishell: memory allocation error during token merging\n");
					free_commands(head);
					return (NULL);
				}
				if (!update_command_arg(current, last_arg_index, merged))
				{
					fprintf(stderr,
						"minishell: failed to update argument during token merging\n");
					free(merged);
					free_commands(head);
					return (NULL);
				}
				free(merged);
			}
			else
			{
				add_argument(current, tok->value);
				if (current->arg_count == 1)
					current->name = strdup(tok->value);
			}
		}
		else if (tok->type == TOKEN_PIPE)
		{
			if (!current || !tok->next || tok->next->type == TOKEN_PIPE)
			{
				fprintf(stderr,
					"minishell: syntax error near unexpected token `|`\n");
				free_commands(head);
				return (NULL);
			}
			add_command(&head, current);
			current = NULL;
		}
		else if (tok->type == TOKEN_REDIR_IN || tok->type == TOKEN_REDIR_OUT
			|| tok->type == TOKEN_APPEND)
		{
			if (!current)
				current = create_command();
			redir = current->redirections;
			while (redir)
			{
				if ((redir->type == TOKEN_REDIR_IN
						&& tok->type == TOKEN_REDIR_IN)
					|| (redir->type == TOKEN_REDIR_OUT
						&& tok->type == TOKEN_REDIR_OUT)
					|| (redir->type == TOKEN_APPEND
						&& tok->type == TOKEN_APPEND))
				{
					fprintf(stderr, "minishell: ambiguous redirect\n");
					free_commands(head);
					return (NULL);
				}
				redir = redir->next;
			}
			if (tok->next && tok->next->type == TOKEN_WORD)
			{
				set_redirection(current, tok, tok->next);
				tok = tok->next;
			}
			else
			{
				fprintf(stderr,
					"minishell: syntax error near unexpected token `%s`\n",
					tok->type == TOKEN_APPEND ? ">>" : tok->type == TOKEN_REDIR_IN ? "<" : ">");
				free_commands(head);
				return (NULL);
			}
		}
		else if (tok->type == TOKEN_HEREDOC)
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
					"minishell: syntax error: missing heredoc delimiter\n");
				free_commands(head);
				return NULL;
			}
		}
		tok = tok->next;
	}
	if (current && (current->name || current->heredoc_count > 0))
	{
		if (!current->name && current->heredoc_count > 0)
		{
			current->name = ft_strdup("cat");
			add_argument(current, "cat");
		}
		add_command(&head, current);
	}
	else
		free(current);
	return head;
}

void	print_commands(t_command *head)
{
	t_command		*current;
	t_redirection	*redir;
	int				i;

	current = head;
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
void	free_commands(t_command *head)
{
	t_command	*temp;
	int			i;

	while (head)
	{
		temp = head;
		head = head->next;
		/* Free the command name */
		if (temp->name)
			free(temp->name);
		/* Free all arguments */
		if (temp->args)
		{
			i = 0;
			while (i < temp->arg_count)
			{
				free(temp->args[i]);
				i++;
			}
			free(temp->args);
		}

		/* FREE THE REDIRECTION LIST - ADD THIS LINE */
		free_redirections(temp->redirections);

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
