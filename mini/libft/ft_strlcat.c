/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 17:05:22 by absaadan          #+#    #+#             */
/*   Updated: 2024/11/09 12:50:07 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	ldst;
	size_t	lsrc;
	size_t	i;

	ldst = 0;
	lsrc = 0;
	i = 0;
	while (dst && dst[ldst] != '\0' )
		ldst++;
	while (src[lsrc] != '\0')
		lsrc++;
	if (ldst >= size)
		return (size + lsrc);
	while (src[i] != '\0' && (ldst + i) < (size - 1))
	{
		dst[ldst + i] = src[i];
		i++;
	}
	dst[ldst + i] = '\0';
	return (ldst + lsrc);
}
