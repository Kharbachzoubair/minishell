/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 14:06:21 by absaadan          #+#    #+#             */
/*   Updated: 2024/11/09 12:50:33 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(s);
	i = len;
	while (i > 0)
	{
		if (s[i - 1] == (char)c)
		{
			return ((char *)&s[i - 1]);
		}
		i--;
	}
	if (s[len] == (char)c)
	{
		return ((char *)&s[len]);
	}
	return (NULL);
}
