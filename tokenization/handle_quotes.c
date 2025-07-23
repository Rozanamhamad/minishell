/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 12:30:51 by cdiab             #+#    #+#             */
/*   Updated: 2025/07/21 00:53:27 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_token(const char *input, int *i)
{
	int		start;
	int		sq;
	int		dq;

	sq = 0;
	dq = 0;
	while (input[*i] && (input[*i] == ' '
			|| input[*i] == '\t' || input[*i] == '\n'))
		(*i)++;
	if (!input[*i])
		return (NULL);
	start = *i;
	while (input[*i])
	{
		if (input[*i] == '\'' && !dq)
			sq = !sq;
		else if (input[*i] == '"' && !sq)
			dq = !dq;
		else if (!sq && !dq && (input[*i] == ' ' || input[*i] == '\t'
				|| input[*i] == '\n'))
			break ;
		(*i)++;
	}
	return (ft_strndup(input + start, *i - start));
}

