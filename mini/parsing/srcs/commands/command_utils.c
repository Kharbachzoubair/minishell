/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:43 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/02 16:39:45 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	new_args = malloc((cmd->arg_count + 2) * sizeof(*new_args));
	if (!new_args)
		return (0);
	i = 0;
	while (i < cmd->arg_count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
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
