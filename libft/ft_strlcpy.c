/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdiab <cdiab@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 13:59:56 by cdiab             #+#    #+#             */
/*   Updated: 2024/06/23 11:21:10 by cdiab            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dest, const char *src, size_t l)
{
	size_t	i;

	i = 0;
	while (src[i])
		i++;
	if (!l)
		return (i);
	while (--l && *src)
		*dest++ = *src++;
	*dest = '\0';
	return (i);
}
