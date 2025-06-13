/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 16:21:33 by absaadan          #+#    #+#             */
/*   Updated: 2024/11/09 12:48:30 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_word(char const *s, char c)
{
	int		i;
	size_t	count;
	int		word;

	i = 0;
	count = 0;
	while (s[i])
	{
		word = 0;
		while (s[i] == c && s[i])
			i++;
		while (s[i] != c && s[i])
		{
			if (word == 0)
				count++;
			word = 1;
			i++;
		}
	}
	return (count);
}

static char	**free_s(char **split, size_t j)
{
	size_t	i;

	i = 0;
	if (split)
	{
		while (i <= j)
		{
			free(split[i]);
			i++;
		}
		free(split);
	}
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	start;
	size_t	j;
	char	**split;

	if (!s)
		return (NULL);
	split = (char **)malloc((count_word(s, c) + 1) * sizeof(char *));
	if (!split)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i] && j < count_word(s, c) && i < ft_strlen(s))
	{
		while (s[i] == c && s[i])
			i++;
		start = i;
		while (s[i] != c && s[i])
			i++;
		split[j++] = ft_substr(s, start, i - start);
		if (!split[j - 1])
			return (free_s(split, j - 1));
	}
	split[j] = NULL;
	return (split);
}
