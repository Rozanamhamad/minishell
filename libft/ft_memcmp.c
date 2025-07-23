/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdiab <cdiab@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 10:45:32 by cdiab             #+#    #+#             */
/*   Updated: 2024/06/17 18:09:50 by cdiab            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t					i;
	const unsigned char		*p;
	const unsigned char		*k;

	i = 0;
	p = ((const unsigned char *)s1);
	k = ((const unsigned char *)s2);
	while (i < n)
	{
		if (p[i] != k[i])
			return (p[i] - k[i]);
		i++;
	}
	return (0);
}
