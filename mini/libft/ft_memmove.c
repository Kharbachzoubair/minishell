/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 17:22:23 by absaadan          #+#    #+#             */
/*   Updated: 2024/11/09 12:49:26 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char	*pdst;
	char	*psrc;

	pdst = (char *)dst;
	psrc = (char *)src;
	if (!len)
		return (dst);
	if (src == NULL && dst == NULL)
		return (NULL);
	if (dst < src)
	{
		ft_memcpy(dst, src, len);
	}
	else
	{
		while (len)
		{
			len--;
			pdst[len] = psrc[len];
		}
	}
	return (dst);
}
