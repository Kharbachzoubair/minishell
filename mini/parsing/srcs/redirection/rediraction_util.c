/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 15:39:56 by absaadan          #+#    #+#             */
/*   Updated: 2025/04/15 13:47:31 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	copy_heredoc_delims(char **new_list, char **old_list, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_list[i] = ft_strdup(old_list[i]);
		if (!new_list[i])
		{
			while (--i >= 0)
				free(new_list[i]);
			free(new_list);
			return ;
		}
		i++;
	}
}

static void	free_old_heredoc_delims(char **delims, int count)
{
	int	i;

	if (!delims)
		return ;
	i = 0;
	while (i < count)
	{
		free(delims[i]);
		i++;
	}
	free(delims);
}

static int	setup_new_heredoc_list(t_command *cmd, char **new_list,
			char *new_delim)
{
	copy_heredoc_delims(new_list, cmd->heredoc_delims, cmd->heredoc_count);
	new_list[cmd->heredoc_count] = ft_strdup(new_delim);
	if (!new_list[cmd->heredoc_count])
	{
		copy_heredoc_delims(NULL, new_list, cmd->heredoc_count);
		return (0);
	}
	new_list[cmd->heredoc_count + 1] = NULL;
	return (1);
}

void	set_redirection(t_command *cmd, t_token *type_token,
		t_token *file_token)
{
	t_redirection	*new_redir;
	char			**new_list;

	new_redir = create_redirection(type_token->type, file_token->value);
	if (!new_redir)
		return ;
	add_redirection(cmd, new_redir);
	if (type_token->type == TOKEN_HEREDOC)
	{
		new_list = malloc(sizeof(char *) * (cmd->heredoc_count + 2));
		if (!new_list)
			return ;
		if (!setup_new_heredoc_list(cmd, new_list, file_token->value))
			return ;
		free_old_heredoc_delims(cmd->heredoc_delims, cmd->heredoc_count);
		cmd->heredoc_delims = new_list;
		cmd->heredoc_count++;
	}
}
