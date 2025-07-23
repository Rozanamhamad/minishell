/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 12:14:50 by ral-moha          #+#    #+#             */
/*   Updated: 2025/07/22 23:27:50 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"    

int	exec_pwd(t_ast_node *node, t_myenv *env)
{
	char	cwd[1024];

	(void)node;
	if (getcwd(cwd, sizeof(cwd)))
	{//if successful print the curret directory followe d'\n'
		
		ft_putendl_fd(cwd, 1);
		env->exit_code = 0; //success
		return (1);
	}
	else
	{
		perror("pwd");
		env->exit_code = 1;//failure
		return (1);
	}
}

void	ft_putendl_fd(char *s, int fd)
{
	if (s)//not null
	{
		while (*s)//each char
		{
			write(fd, s, 1);
			s++;
		}
		write(fd, "\n", 1);
	}
}
