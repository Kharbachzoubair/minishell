/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:12:16 by absaadan          #+#    #+#             */
/*   Updated: 2025/06/15 17:10:53 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution/shell.h"

static int	count_env_vars(t_env *env_list)
{
	t_env	*cur;
	int		count;

	count = 0;
	cur = env_list;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	return (count);
}

static void	cleanup_envp_on_error(char **envp, int i)
{
	while (i-- > 0)
		free(envp[i]);
	free(envp);
}

static int	create_env_string(char **envp, int i, t_env *cur)
{
	envp[i] = malloc(strlen(cur->key) + 1 + strlen(cur->value) + 1);
	if (!envp[i])
		return (0);
	ft_strcpy(envp[i], cur->key);
	ft_strcat(envp[i], "=");
	ft_strcat(envp[i], cur->value);
	return (1);
}

char	**env_list_to_envp(t_env *env_list)
{
	t_env	*cur;
	int		count;
	char	**envp;
	int		i;

	count = count_env_vars(env_list);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	i = 0;
	cur = env_list;
	while (cur)
	{
		if (!create_env_string(envp, i, cur))
		{
			cleanup_envp_on_error(envp, i);
			return (NULL);
		}
		i++;
		cur = cur->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	ft_free_strarr(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

// static void	lleak(void)
// {
// 	system("leaks minishell");
// }

static void	process_input_line(char *input, t_env *env_list)
{
	t_token		*tokens;
	t_command	*commands;

	tokens = tokenize(input);
	if (tokens)
		expand_env_variables_improved(tokens, env_list,
			get_last_exit_status());
	commands = parse_tokens(tokens);
	if (commands && process_command_heredocs(commands, env_list) == -1)
	{
		free_commands(commands);
		free_tokens(tokens);
		return ;
	}
	execute_commands(commands, env_list);
	free_commands(commands);
	free_tokens(tokens);
}

int	main(int ac, char **av, char **envp)
{
	char		*input;
	t_env		*env_list;

	// atexit(lleak);
	(void)ac;
	(void)av;
	env_list = init_env(envp);
	set_last_exit_status(0);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break ;
		if (*input)
			add_history(input);
		process_input_line(input, env_list);
		free(input);
	}
	free_env_list(env_list);
	return (get_last_exit_status());
}
