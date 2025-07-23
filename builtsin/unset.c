/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 22:57:56 by ral-moha          #+#    #+#             */
/*   Updated: 2025/07/15 21:34:51 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

//only unset valid keys 
static int	is_valid_identifier(char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))//not null and start with letter or underscore 
		return (0);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_') //rest of the key must be alpha or _
			return (0);
		i++;
	}
	return (1);//valid
}
//finds and removes matching variables from env->env.
static void	remove_env_varr(char *key, t_myenv *env)
{
	int		i;
	int		j;
	int		key_len;

	i = 0;
	j = 0;
	key_len = ft_strlen(key);
	if (!env || !env->env) //check if env exist 
		return ;
	while (env->env[i])
	{
		if (strcmp(env->env[i], key) == 0
			|| (ft_strncmp(env->env[i], key, key_len) == 0
				&& env->env[i][key_len] == '='))
			free(env->env[i]);
		else
			env->env[j++] = env->env[i];
		i++;
	}
	env->env[j] = NULL;
}
//processes each argument to unset if valid, otherwise prints an error.
int	exec_unset(t_ast_node *node, t_myenv *env)
{
	int	i;

	if (!node || !node->arr || !env)
		return (1);
	i = 1;
	while (node->arr[i])
	{
		if (!is_valid_identifier(node->arr[i]))
		{
			ft_putstr_fd("unset: ", 2);
			ft_putstr_fd(node->arr[i], 2);
			ft_putendl_fd(": not a valid identifier", 2);
			env->exit_code = 1;//failure
		}
		else
		{
			//if valid remove var form env
			remove_env_varr(node->arr[i], env);
			env->exit_code = 0;//success
		}
		i++;
	}
	return (1);
}
