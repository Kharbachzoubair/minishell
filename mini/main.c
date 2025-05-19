/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkharbac <zkharbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 16:12:16 by absaadan          #+#    #+#             */
/*   Updated: 2025/05/19 00:50:14 by zkharbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execution/shell.h"
// #include "parsing/includes/minishell.h"
char    **env_list_to_envp(t_env *env_list)
{
    t_env   *cur;
    int     count;
    char    **envp;
    int     i;

    count = 0;
    cur = env_list;
    while (cur)
    {
        count++;
        cur = cur->next;
    }
    envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return (NULL);
    i = 0;
    cur = env_list;
    while (cur)
    {
        envp[i] = malloc(strlen(cur->key) + 1 + strlen(cur->value) + 1);
        if (!envp[i])
        {
            /* Free already-allocated strings */
            while (i-- > 0)
                free(envp[i]);
            free(envp);
            return (NULL);
        }
        strcpy(envp[i], cur->key);
        strcat(envp[i], "=");
        strcat(envp[i], cur->value);
        i++;
        cur = cur->next;
    }
    envp[i] = NULL;
    return (envp);
}

/*
** Free a NULL-terminated array of strings
*/
void    ft_free_strarr(char **arr)
{
    int i;

    if (!arr)
        return;
    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

// static void lleak(void)
// {
//     system("leaks minishell");
// }

int main(int ac, char **av, char **envp)
{
    char        *input;
    t_token     *tokens;
    t_command   *commands;
    t_env       *env_list;

    (void)ac;
    (void)av;
    // atexit(lleak);
    env_list = init_env(envp);
    set_last_exit_status(0);
    while (1)
    {
        input = readline("minishell> ");
        if (!input)
            break;
        if (*input)
            add_history(input);
        tokens = tokenize(input);
        if (tokens)
            expand_env_variables_improved(tokens,
                env_list, get_last_exit_status());
        // print_tokens(tokens);
        commands = parse_tokens(tokens);
        // print_commands(commands);
        execute_commands(commands, env_list);
        free_commands(commands);
        free_tokens(tokens);
        free(input);
    }
    free_env_list(env_list);
    return (get_last_exit_status());
}
