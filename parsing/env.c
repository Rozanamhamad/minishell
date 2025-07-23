/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdiab <cdiab@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 23:32:05 by cdiab             #+#    #+#             */
/*   Updated: 2025/06/25 19:42:48 by cdiab            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_size(char **env)
{
	int	i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

char	**copy_env(char **env)
{
	int		i;
	char	**cpyenv;
	int		size;

	i = 0;
	size = env_size(env);
	cpyenv = malloc(sizeof(char *) * (size + 1));
	if (!cpyenv)
		return (NULL);
	while (env[i])
	{
		cpyenv[i] = ft_strdup(env[i]);
		if (!cpyenv[i])
		{
			while (i >= 0)
			{
				free(cpyenv[i]);
				i--;
			}
			free(cpyenv);
			return (NULL);
		}
		i++;
	}
	cpyenv[i] = NULL;
	return (cpyenv);
}

void	free_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	print_double_pointer(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
	{
		printf("%s\n", strs[i]);
		i++;
	}
}

