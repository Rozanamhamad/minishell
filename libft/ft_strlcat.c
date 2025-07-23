/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdiab <cdiab@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 14:37:00 by cdiab             #+#    #+#             */
/*   Updated: 2024/06/17 18:09:44 by cdiab            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dst_len;
	size_t	src_len;
	size_t	concat_len;

	i = 0;
	dst_len = 0;
	src_len = 0;
	while (src[src_len] != '\0')
		src_len++;
	while (dst[dst_len] != '\0' && dst_len < size)
		dst_len++;
	if (dst_len == size)
		return (src_len + size);
	concat_len = size - dst_len - 1;
	while (i < concat_len && src[i] != '\0')
	{
		dst[dst_len + i] = src[i];
		i++;
	}
	dst[dst_len + i] = '\0';
	return (dst_len + src_len);
}
