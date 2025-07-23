/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 23:48:32 by ral-moha          #+#    #+#             */
/*   Updated: 2025/07/23 15:11:37 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// -n , -nn, -nnnn
static int	is_valid_n(const char *str)
{
	int	i;

	if (!str || str[0] != '-' || str[1] != 'n')
		return (0);
	i = 2;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	exec_echo(t_ast_node *node, t_myenv *env)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	// /-n /-nn / -nnn
	while (node->arr[i] && is_valid_n(node->arr[i]))
	{
		newline = 0;
		i++;
	}
	if (!newline && node->arr[i] && ft_strncmp(node->arr[i], "--", 3) == 0)
		i++;
	while (node->arr[i])
	{
		ft_putstr_fd(node->arr[i], 1);
		if (node->arr[i + 1])
			write(1," ",1);
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	env->exit_code = 0;
	return (1);
}


int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n && s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	if (i < n)
		return ((unsigned char)s1[i] - (unsigned char)s2[i]);
	return (0);
}
