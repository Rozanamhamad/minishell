/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:08:13 by ral-moha          #+#    #+#             */
/*   Updated: 2025/08/05 19:52:09 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_env(t_ast_node *node, t_myenv *env)
{
	int	i;

	if (node && node->arr && node->arr[1])
	{
		ft_putendl_fd("env: too many arguments", STDERR_FILENO);
		if	(env)env->exit_code = 127;
		return (127);
	}
	//Null‑safety: empty env list ⇒ nothing to print
	if (!env || !env->env)
		return (0);
	/* 3‒ Main loop: print only entries that contain '='                 */
	i = 0;
	while (env->env[i])
	{
		if (ft_strchr(env->env[i], '='))          /* skip stray keys      */
			ft_putendl_fd(env->env[i], STDOUT_FILENO);
		i++;
	}
	if (env) env->exit_code = 0;
	return (1);
}