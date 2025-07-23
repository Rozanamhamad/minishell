/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 00:29:11 by cdiab             #+#    #+#             */
/*   Updated: 2025/07/22 23:33:02 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// /**
//  * Recursively preprocess AST nodes:
//  * 1. Expand environment variables
//  * 2. Remove quotations
//  */
void preprocess_ast(t_ast_node *node, t_myenv *my_env)
{
	int i = 0;
	char *exp, *noq;

	if (!node)
		return;

	preprocess_ast(node->left, my_env);

	if (node->arr)
	{
		while (node->arr[i])
		{
			exp = expand_string(node->arr[i], my_env); // Expand $VARs
			if (!exp)
				return;
			noq = clean_quotations(exp); // Remove quotes
			free(exp);
			free(node->arr[i]);
			node->arr[i++] = noq;
		}
	}
	preprocess_ast(node->right, my_env);
}


// /**
//  * Executes a builtin if the root node is a simple command.
//  */
int handle_builtins(t_ast_node *node, t_myenv *env)
{
	if (!node || !node->arr || !node->arr[0])
		return (0);

	// print_ast(node, 0);
	if (is_builtin(node->arr[0]))
		return (exec_builtin(node, env));
	return (0);
}


void execute_ast(t_ast_node *node, t_myenv *env)
{
    if (!node)
        return;

    // Only treat as a pipe if both children exist
    preprocess_ast(node, env);
    if (handle_builtins(node, env))
        return ;
    if (node->type == T_PIPE && node->left && node->right)
        execute_pipe(node, env);
    else
        execute_simple_cmd(node, env);
}

// void execute_pipe(t_ast_node *node, t_myenv *env)
// {
// 	int pipefd[2];                  // Array to store pipe fd: pipefd[0] = read end, pipefd[1] = write end
// 	pid_t left_pid, right_pid;     //ID

// 	if (pipe(pipefd) == -1) 
// 	{
// 		perror("pipe");            // If pipe creation fails,, error
// 		env->exit_code = 1;        // Set exit code to failure
// 		return;
// 	}

// 	left_pid = fork();             // Fork the process to create the left child
// 	if (left_pid == -1)            //if fork failed
// 	{
// 		perror("fork");
// 		env->exit_code = 1;        // Set failure code
// 		return;
// 	}

// 	if (left_pid == 0)
// 	{
// 		close(pipefd[0]);                         // Close unused read end of the pipe
// 		dup2(pipefd[1], STDOUT_FILENO);           // Redirect STDOUT to pipe's write end
// 		close(pipefd[1]);                         // Close original write end after duplication
// 		execute_ast(node->left, env);             // Recursively execute left side of the pipe
// 		exit(env->exit_code);                     // Exit with the last known exit code
// 	}

// 	right_pid = fork();            // Fork again to create the right child
// 	if (right_pid == -1)           // Check for fork error
// 	{
// 		perror("fork");
// 		env->exit_code = 1;        // Set failure code
// 		return;
// 	}

// 	if (right_pid == 0)
// 	{ //RIGHT CHILD PROCESS
// 		close(pipefd[1]);                         // Close unused write end of the pipe
// 		dup2(pipefd[0], STDIN_FILENO);
// 		close(pipefd[0]);                         // Close original read end after duplication
// 		execute_ast(node->right, env);            // Recursively execute right side of the pipe
// 		exit(env->exit_code);
// 	}

// 	close(pipefd[0]);             // Parent process: close both ends of pipe
// 	close(pipefd[1]);             // (no longer needed after forking)

// 	waitpid(left_pid, NULL, 0);   // Wait for the left child to finish
// 	waitpid(right_pid, NULL, 0);  // Wait for the right child to finish
// }

void execute_pipe(t_ast_node *node, t_myenv *env)
{
    int    fd[2], saved = dup(STDOUT_FILENO), bi, l, r;
    if (pipe(fd) == -1) { perror("pipe"); env->exit_code = 1; return; }

    bi = node->left->type==T_SCMD; // && handle_builtins(node->left, env)
    if (bi) {
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        dup2(saved, STDOUT_FILENO);
        close(saved);
    } else {
        l = fork();
        if (l == 0) {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);
            execute_ast(node->left, env);
            exit(env->exit_code);
        }
        dup2(saved, STDOUT_FILENO);
        close(saved);
    }

    r = fork();
    if (r == 0) {
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        execute_ast(node->right, env);
        exit(env->exit_code);
    }

    close(fd[0]);
    close(fd[1]);
    if (!bi) waitpid(l, NULL, 0);
    waitpid(r, NULL, 0);
}


