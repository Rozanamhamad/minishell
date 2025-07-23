/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdiab <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 12:52:46 by cdiab             #+#    #+#             */
/*   Updated: 2024/06/23 13:10:06 by cdiab            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_substrings(const char *s, char c)
{
	int	count;
	int	in_substring;

	count = 0;
	in_substring = 0;
	while (*s)
	{
		if (*s != c && !in_substring)
		{
			in_substring = 1;
			count++;
		}
		else if (*s == c)
		{
			in_substring = 0;
		}
		s++;
	}
	return (count);
}

static void	set(const char *s, const char c, char **result)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	k = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			k = i;
			while (s[k] && s[k] != c)
				k++;
			result[j] = (char *) malloc((k - i + 1) * sizeof(char));
			k = 0;
			while (s[i] && s[i] != c)
				result[j][k++] = s[i++];
			result[j++][k] = '\0';
		}
	}
	result[j] = NULL;
}

char	**ft_split(const char *s, const char c)
{
	char	**result;
	int		count;

	count = count_substrings(s, c);
	result = (char **) malloc((count + 1) * sizeof(char *));
	if (result == NULL)
		return (NULL);
	set(s, c, result);
	return (result);
}
