/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/28 17:28:00 by absaadan          #+#    #+#             */
/*   Updated: 2024/11/03 14:07:04 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*p;
	unsigned char	*d;
	size_t			i;

	p = (unsigned char *)s1;
	d = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (p[i] != d[i])
			return (p[i] - d[i]);
		i++;
	}
	return (0);
}
