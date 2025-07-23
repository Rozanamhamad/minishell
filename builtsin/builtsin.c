/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtsin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:24:43 by ral-moha          #+#    #+#             */
/*   Updated: 2025/07/22 23:24:10 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <string.h>


int	is_builtin(char *cmd)
{
	if (!cmd)
		return 0;
	return (
		strcmp(cmd, "cd") == 0 ||
		strcmp(cmd, "echo") == 0 ||
		strcmp(cmd, "exit") == 0 ||
		strcmp(cmd, "export") == 0 ||
		strcmp(cmd, "pwd") == 0 ||
		strcmp(cmd, "unset") == 0 ||
		strcmp(cmd, "env") == 0
	);
}


int exec_builtin(t_ast_node *node, t_myenv *env)
{
    if (!node || !node->arr || !node->arr[0])
        return (0);

    if (strcmp(node->arr[0], "exit") == 0)
	{
        return exec_exit(node, env);
	}
	else if (strcmp(node->arr[0], "pwd") == 0)
	{
		return (exec_pwd(node, env));
	}
	else if (strcmp(node->arr[0], "env") == 0)
	{
		return (exec_env(node, env)); // TO CHECK env asd for example
	}
	else if (strcmp(node->arr[0], "echo") == 0)
	{
		return (exec_echo(node, env));
	}
	else if (strcmp(node->arr[0], "unset") == 0)
	{
		return (exec_unset(node, env));
	}
	else if (strcmp(node->arr[0], "cd") == 0)
	{
		return (exec_cd(node, env));
	}
	else if (strcmp(node->arr[0], "export") == 0)
	{
		return (exec_export(node, env));
	}
	return 0;
}

// void set_env_var(const char *key, const char *value, t_myenv *env)
// {
// 	int i = 0;
// 	size_t len = strlen(key);
// 	char *new_var = malloc(strlen(key) + strlen(value) + 2);
// 	sprintf(new_var, "%s=%s", key, value);

// 	while (env->env[i]) {
// 		if (strncmp(env->env[i], key, len) == 0 && env->env[i][len] == '=') {
// 			free(env->env[i]);
// 			env->env[i] = new_var;
// 			return;
// 		}
// 		i++;
// 	}

// 	env->env = realloc(env->env, sizeof(char *) * (i + 2));
// 	env->env[i] = new_var;
// 	env->env[i + 1] = NULL;
// }
// int handle_builtins(t_ast_node *node, t_myenv *env)
// {
//     (void)env;  // Prevent unused‐parameter warning

//     if (!node || !node->arr || !node->arr[0])
//         return 0;

//     // Example: handle "echo" builtin
//     if (strcmp(node->arr[0], "echo") == 0)
//     {
//         int i = 1;
//         while (node->arr[i])
//         {
//             ft_putstr_fd(node->arr[i], 1);
//             if (node->arr[i + 1])
//                 write(1, " ", 1);
//             i++;
//         }
//         write(1, "\n", 1);
//         return 1;
//     }
//     return 0; // Not a builtin
// }

char *find_path(const char *cmd, t_myenv *env)
{
    char **paths;
    char *tmp;
    char *full_path;
    int i = 0;

    if (!cmd || ft_strchr(cmd, '/'))
        return (ft_strdup(cmd));

    char *path_val = get_env_value(env, "PATH"); // your own function
    if (!path_val)
        return (NULL);
    
    paths = ft_split(path_val, ':');
    while (paths && paths[i])
    {
        tmp = ft_strjoin(paths[i], "/");
        full_path = ft_strjoin(tmp, cmd);
        free(tmp);
        if (access(full_path, X_OK) == 0)
            return (full_path);
        free(full_path);
        i++;
    }
    free_split(paths);
    return (NULL);
}
void free_split(char **arr)
{
    int i = 0;

    while (arr && arr[i])
        free(arr[i++]);
    free(arr);
}
char *get_env_value(t_myenv *env, const char *key)
{
    int i = 0;
    size_t len = ft_strlen(key);

    while (env->env[i])
    {
        if (ft_strncmp(env->env[i], key, len) == 0 && env->env[i][len] == '=')
            return (env->env[i] + len + 1);
        i++;
    }
    return (NULL);
}
