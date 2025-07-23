/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 22:05:25 by ral-moha          #+#    #+#             */
/*   Updated: 2025/07/22 23:12:18 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void execute_simple_cmd(t_ast_node *node, t_myenv *env)
{
    int    fd_in = -1, fd_out = -1;
    pid_t  pid;

    if (node->ex_heredoc) handle_heredoc(node);
    if (!setup_redirections(node, &fd_in, &fd_out, env)) return;
    // if (handle_builtins(node, env)) {
    //     if (fd_in  != -1) close(fd_in);
    //     if (fd_out != -1) close(fd_out);
    //     return;
    // }

    pid = fork();
    if (pid < 0)
        perror("fork");
    else if (pid == 0) {
        if (fd_in  != -1) dup2(fd_in,  STDIN_FILENO),  close(fd_in);
        if (fd_out != -1) dup2(fd_out, STDOUT_FILENO), close(fd_out);
        execve(find_path(node->arr[0], env), node->arr, env->env);
        perror("execve");
        exit(127);
    }

    if (fd_in  != -1) close(fd_in);
    if (fd_out != -1) close(fd_out);
    waitpid(pid, &env->exit_code, 0);
    env->exit_code = WEXITSTATUS(env->exit_code);
}



int setup_redirections(t_ast_node *node, int *fd_in, int *fd_out, t_myenv *env)
{
	if (node->in)
		*fd_in = open(node->in, O_RDONLY);
	if (node->out)
		*fd_out = open(node->out, O_WRONLY | O_CREAT |
			(node->app ? O_APPEND : O_TRUNC), 0644);

	if (*fd_in == -1 && node->in)
	{
		perror(node->in);
		env->exit_code = 1;
		return (0);
	}
	if (*fd_out == -1 && node->out)
	{
		perror(node->out);
		env->exit_code = 1;
		if (*fd_in != -1) close(*fd_in);
		return (0);
	}
	return (1);
}
void exec_child_process(t_ast_node *node, t_myenv *env, int fd_in, int fd_out)
{
	if (fd_in != -1)
		dup2(fd_in, STDIN_FILENO);
	if (fd_out != -1)
		dup2(fd_out, STDOUT_FILENO);
	if (fd_in != -1) close(fd_in);
	if (fd_out != -1) close(fd_out);

	execve(find_path(node->arr[0], env), node->arr, env->env);
	perror("execve");
	exit(127);
}

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