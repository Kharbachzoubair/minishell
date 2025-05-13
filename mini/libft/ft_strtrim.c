/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 18:05:32 by absaadan          #+#    #+#             */
/*   Updated: 2024/11/09 12:50:46 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	to_check(const char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(const char *s1, const char *set)
{
	size_t	i;
	size_t	len;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	if ((ft_strlen(s1) == 0 && ft_strlen(set) == 0) || ft_strlen(s1) == 0)
		return (ft_strdup(""));
	len = ft_strlen(s1) - 1;
	while (s1[i] && to_check(s1[i], set) == 1)
		i++;
	while (s1[len] && to_check(s1[len], set) == 1)
		len--;
	return (ft_substr(s1, i, len - i + 1));
}
