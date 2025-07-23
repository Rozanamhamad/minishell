/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 13:14:37 by ral-moha          #+#    #+#             */
/*   Updated: 2025/07/22 23:30:54 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	remove_env_var(const char *key, t_myenv *e)
{
	int		i;
	size_t	key_len;

	if (!e || !e->env || !key)
		return ;
	key_len = ft_strlen(key);
	i = 0;
	while (e->env[i])
	{
		if (ft_strncmp(e->env[i], key, key_len) == 0 && e->env[i][key_len] == '=')
		{
			free(e->env[i]);
			while (e->env[i + 1])
			{
				e->env[i] = e->env[i + 1];
				i++;
			}
			e->env[i] = NULL;
			return ;
		}
		i++;
	}
}

int	go_env(int opt, t_myenv *e)
{
	const char	*dir = NULL;

	if (opt == 0)
		dir = ft_getenv("HOME", e);
	if (opt == 1)
		dir = ft_getenv("OLDPWD", e);
	if (!dir)
	{
		if (opt == 0)
			ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
		else
			ft_putendl_fd("cd: OLDPWD not set", STDERR_FILENO);
		e->exit_code = 1;
		return (1);
	}
	if (chdir(dir) == -1)
	{
		perror("cd");
		e->exit_code = 1;
		return (1);
	}
	if (opt == 1)
		ft_putendl_fd((char *)dir, STDOUT_FILENO);
	e->exit_code = 0;
	return (1);
}

void	set_env_var(const char *key, const char *value, t_myenv *e)
{
	int		i;
	char	*new_var;
	size_t	key_len;

	if (!key || !value || !e || !e->env)
		return ;
	key_len = ft_strlen(key);
	new_var = str_append(ft_strjoin(key, "="), value);
	if (!new_var)
		return ;
	i = 0;
	while (e->env[i])
	{
		if (ft_strncmp(e->env[i], key, key_len) == 0 && e->env[i][key_len] == '=')
		{
			free(e->env[i]);
			e->env[i] = new_var;
			return ;
		}
		i++;
	}
	// Key not found: add to env
	char **new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return ;
	for (int j = 0; j < i; j++)
		new_env[j] = e->env[j];
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(e->env);
	e->env = new_env;
}
