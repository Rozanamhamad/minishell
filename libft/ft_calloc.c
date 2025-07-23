/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdiab <cdiab@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 12:57:28 by cdiab             #+#    #+#             */
/*   Updated: 2024/06/23 13:11:59 by cdiab            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t number, size_t size)
{
	void	*array;

	if (size != 0 && number > 2147483647 / size)
		return (0);
	array = (void *)malloc(size * number);
	if (!array)
		return (NULL);
	ft_bzero(array, size * number);
	return ((void *)array);
}
