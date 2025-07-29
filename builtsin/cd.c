/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 20:42:45 by ral-moha          #+#    #+#             */
/*   Updated: 2025/07/29 13:23:49 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h> 

void	cd_perr(const char *msg, const char *arg, t_myenv *e)
{
	ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
	ft_putstr_fd((char *)msg, STDERR_FILENO); //error mssg
	if (arg)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putendl_fd((char *)arg, STDERR_FILENO);
	}
	e->exit_code = 1;
}

//store current directory in OLDPWD
void	save_oldpwd(t_myenv *e)
{
	char	buf[1024];

	if (!getcwd(buf, 1024))
		return ;
	remove_env_var("OLDPWD", e);
	set_env_var("OLDPWD", buf, e);
}

int	get_env_key_and_path(int opt, t_myenv *e, const char **key, char **path)
{
	if (opt == 1) //cd -
		*key = "OLDPWD";
	else
		*key = "HOME";
	*path = ft_getenv(*key, e);//value from env
	if (!*path)
	{
		if (opt == 1)
			cd_perr("OLDPWD not set", NULL, e);
		else
			cd_perr("HOME not set", NULL, e);
		return (1);
	}
	return (0);
}

int	exec_cd(t_ast_node *n, t_myenv *e)
{
	char	**a;

	a = n->arr;
	//cd  or cd ~ go to home
	//a[1] no arg cd 
	if (!a[1] || !ft_strncmp(a[1], "~", 2))
		return (go_env(0, e));
	if (a[2])
		return (cd_perr("too many arguments", NULL, e), 1);
	if (!ft_strncmp(a[1], "-", 2))
		return (go_env(1, e));
	save_oldpwd(e);
	if (chdir(a[1]) == -1)
		return (cd_perr(a[1], strerror(errno), e), 1);
	e->exit_code = 0;
	return (1);
}
