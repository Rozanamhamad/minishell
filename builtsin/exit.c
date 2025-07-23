/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 22:00:53 by ral-moha          #+#    #+#             */
/*   Updated: 2025/07/16 20:04:27 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <ctype.h>

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	while (isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!isdigit(str[i]))
		return (0);
	while (isdigit(str[i]))
		i++;
	while (isspace(str[i]))
		i++;
	return (str[i] == '\0');
}
//Convert a string to a long long integer, respecting whitespace and signs
static long long	ft_atoll(const char *str)
{
	int			sign;
	long long	res;

	sign = 1;
	res = 0;
	while (isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
		if (*str++ == '-')
			sign = -1;
	while (isdigit(*str))
		res = res * 10 + (*str++ - '0');
	return (res * sign);
}

static int	handle_errors(t_ast_node *node)
{
	if (!is_numeric(node->arr[1]) || strlen(node->arr[1]) > 20)
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", node->arr[1]);
		exit(2);
	}
	if (node->arr[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		return (1);
	}
	return (0);
}

int	exec_exit(t_ast_node *node, t_myenv *env)
{
	long long	num;

	printf("exit\n");
	if (!node->arr[1])
		exit(env->exit_code);
	if (handle_errors(node))
		return (env->exit_code = 1);
	num = ft_atoll(node->arr[1]);
	if (num > LLONG_MAX || num < LLONG_MIN)
	{
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", node->arr[1]);
		exit(2);
	}
	exit((unsigned char)(num % 256));
}
