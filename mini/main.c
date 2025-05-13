/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:12:16 by absaadan          #+#    #+#             */
<<<<<<< Updated upstream
/*   Updated: 2025/05/13 17:32:14 by absaadan         ###   ########.fr       */
=======
/*   Updated: 2025/05/13 16:46:06 by zkharbac         ###   ########.fr       */
>>>>>>> Stashed changes
/*                                                                            */
/* ************************************************************************** */

#include "parsing/includes/minishell.h"

void lleak(void)
{
    system("leaks minishell");
}

int g_last_exit_status = 0;
extern char **environ;

int main(void)
{
	atexit(lleak);
	char *input;
	t_token *tokens;
	t_command *commands;
	t_env *env_list = init_env(environ);
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break;
		if (*input)
			add_history(input);

		tokens = tokenize(input);

		/* Expand environment variables */
		if (tokens)
		expand_env_variables_improved(tokens, env_list, g_last_exit_status);

		printf("Tokenized input:\n");
		print_tokens(tokens);

		commands = parse_tokens(tokens);
		printf("\nParsed commands:\n");
		print_commands(commands);

		/* For testing $?, set a different exit status each time */
		g_last_exit_status = (g_last_exit_status + 1) % 256;

		free_commands(commands);
		free_tokens(tokens);
		free(input);
	}
	return (0);
}
