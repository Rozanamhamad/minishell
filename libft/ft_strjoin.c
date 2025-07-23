/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdiab <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:18:34 by cdiab             #+#    #+#             */
/*   Updated: 2024/06/21 10:55:06 by cdiab            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*s;
	size_t	ss1;
	size_t	ss2;

	ss1 = ft_strlen(s1);
	ss2 = ft_strlen(s2);
	if (s1 == 0 || s2 == 0)
		return (NULL);
	s = (char *)malloc(ss1 + ss2 + 1);
	if (!s)
		return (NULL);
	ft_memcpy(s, s1, ss1);
	ft_memcpy(s + ss1, s2, ss2 + 1);
	return (s);
}
