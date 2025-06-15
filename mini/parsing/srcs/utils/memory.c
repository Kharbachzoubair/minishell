/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:40:09 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/15 12:47:16 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_heredoc_delims(char **heredoc_delims, int heredoc_count)
{
	int	i;

	if (!heredoc_delims)
		return ;
	i = 0;
	while (i < heredoc_count)
	{
		free(heredoc_delims[i]);
		i++;
	}
	free(heredoc_delims);
}

static void	free_command_args(t_command *cmd)
{
	int	i;

	if (!cmd->args)
		return ;
	i = 0;
	while (i < cmd->arg_count)
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
}

static void	free_command_data(t_command *cmd)
{
	if (cmd->name)
		free(cmd->name);
	free_command_args(cmd);
	free_redirections(cmd->redirections);
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	if (cmd->heredoc_delims)
		free_heredoc_delims(cmd->heredoc_delims, cmd->heredoc_count);
}

void	free_commands(t_command *head)
{
	t_command	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free_command_data(temp);
		free(temp);
	}
}

void	free_env_list(t_env *head)
{
	t_env	*temp;

	while (head)
	{
		temp = head;
		head = head->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}
