/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: absaadan <absaadan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 19:33:55 by absaadan          #+#    #+#             */
/*   Updated: 2024/11/09 12:48:51 by absaadan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	int_len(long nbr)
{
	int	len;

	len = 0;
	if (nbr <= 0)
		len++;
	while (nbr != 0)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}

static char	*pre_conv(int len)
{
	char	*tmp;

	tmp = malloc((len + 1) * sizeof(char));
	if (!tmp)
		return (NULL);
	return (tmp);
}

char	*ft_itoa(int n)
{
	int		len;
	int		i;
	char	*result;
	long	nbr;

	nbr = n;
	len = int_len(nbr);
	result = pre_conv(len);
	if (!result)
		return (NULL);
	i = len - 1;
	if (nbr < 0)
		nbr = -nbr;
	while (nbr != 0)
	{
		result[i] = (nbr % 10) + '0';
		nbr /= 10;
		i--;
	}
	if (n < 0)
		result[0] = '-';
	else if (n == 0)
		result[0] = '0';
	result[len] = '\0';
	return (result);
}
