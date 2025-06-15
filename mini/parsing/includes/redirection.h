/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 09:31:45 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/14 20:33:20 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_H
# define REDIRECTION_H
# include "minishell.h"

void	print_commands(t_command *head);
int	check_redirection_conflict(t_command *current, t_token *tok);
t_token	*handle_redirection_token(t_command **current, t_token *tok);
t_token	*handle_heredoc_token(t_command **current, t_token *tok);
char	*ft_strcat(char *dest, const char *src);
char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*ft_strcpy(char *dest, const char *src);
#endif
