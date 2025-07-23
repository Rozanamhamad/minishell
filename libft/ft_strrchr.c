/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdiab <cdiab@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 17:17:51 by cdiab             #+#    #+#             */
/*   Updated: 2024/06/24 16:27:50 by cdiab            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, int c)
{
	char	*p;
	char	*s;

	s = (char *) str;
	p = NULL;
	while (*s)
	{
		if (*s == (unsigned char)c)
			p = ((char *)s);
		s++;
	}
	if ((unsigned char)c == 0)
	{
		return ((char *)s);
	}
	return (p);
}
