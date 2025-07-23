/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:44:49 by ral-moha          #+#    #+#             */
/*   Updated: 2025/07/22 23:32:14 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_export_error(const char *arg)
{
	ft_putstr_fd("export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}

static int	valid_key(const char *s)
{
	int	i;

	if (!s || s[0] == '\0') // empty string
		return (0);
	if (!ft_isalpha(s[0]) && s[0] != '_') // must start with letter or _
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_'  && s[i] != '=')
			return (0);
		i++;
	}
	return (1);
}

int	exec_export(t_ast_node *node, t_myenv *env)
{
	int		i;
	char	*eq;
	char	*key;
    //printf("Export args:\n");
	// for (int j = 0; node->arr[j]; j++)
	// 	printf("[%s]\n", node->arr[j]);
	if (!node->arr[1])
		return (exec_env(node, env)); // No args: print env
	i = 1;
	while (node->arr[i])
	{
		eq = ft_strchr(node->arr[i], '='); // check =
		if (eq)
		{
			*eq = '\0'; //if = exist
			key = node->arr[i];
			if (!valid_key(key))
				env->exit_code = print_export_error(key);
			else
				set_env_var(key, eq + 1, env);
			*eq = '='; // Restore original string key = value
		}
		else // if no = ( export with empty value)
		{
			if (!valid_key(node->arr[i])) //full arg as key
				env->exit_code = print_export_error(node->arr[i]);
			else
				set_env_var(node->arr[i], "", env); //set key with empty string as value
		}
		i++;
	}
	return (1);
}
