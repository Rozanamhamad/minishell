/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdiab <cdiab@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:37:39 by cdiab             #+#    #+#             */
/*   Updated: 2025/06/23 18:04:27 by cdiab            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	find_pipe_index(t_tokenlist *list)
{
	t_tokennode	*cur;
	int			i;

	cur = list->head;
	i = 0;
	while (cur)
	{
		if (strcmp(cur->token, "|") == 0)
			return (i);
		cur = cur->next;
		i++;
	}
	return (-1);
}

t_tokenlist	*copy_range(t_tokenlist *original, int start, int end)
{
	t_tokenlist	*new_list;
	t_tokennode	*cur;
	int			i;

	new_list = create_tokenlist();
	if (!new_list)
		return (NULL);
	cur = original->head;
	i = 0;
	while (cur && i <= end)
	{
		if (i >= start)
			addtoken(new_list, cur->token, T_SCMD);
		cur = cur->next;
		i++;
	}
	return (new_list);
}

void	split_by_pipe(t_tokenlist *original, t_tokenlist **left,
		t_tokenlist **right)
{
	int	pipe_index;
	int	listsize;

	listsize = list_size(original);
	pipe_index = find_pipe_index(original);
	if (pipe_index == -1)
	{
		*left = copy_range(original, 0, list_size(original) - 1);
		*right = NULL;
	}
	else
	{
		*left = copy_range(original, 0, pipe_index - 1);
		*right = copy_range(original, pipe_index + 1, listsize - 1);
	}
}
