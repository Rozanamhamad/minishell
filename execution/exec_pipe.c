/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 00:29:11 by cdiab             #+#    #+#             */
/*   Updated: 2025/07/29 00:10:39 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// /**
//  * Recursively preprocess AST nodes:
//  * 1. Expand environment variables
//  * 2. Remove quotations
//  */
void preprocess_ast_with_context(t_ast_node *node, t_myenv *my_env, int is_in_pipe)
{
	int i = 0;
	char *exp, *noq;

	if (!node)
		return;

	// For PIPE nodes, both children are in pipe context
	if (node->type == T_PIPE)
	{
		preprocess_ast_with_context(node->left, my_env, 1);
		preprocess_ast_with_context(node->right, my_env, 1);
	}
	else
	{
		// Handle heredoc preprocessing for this node with correct context
		if (node->ex_heredoc)
		{
			set_pipe_context(is_in_pipe);
			handle_heredoc(node);
			set_pipe_context(0);
		}

		if (node->arr)
		{
			while (node->arr[i])
			{
				exp = expand_string(node->arr[i], my_env);
				if (!exp)
					return;
				noq = clean_quotations(exp);
				free(exp);
				free(node->arr[i]);
				node->arr[i++] = noq;
			}
		}
	}
}

void preprocess_ast(t_ast_node *node, t_myenv *my_env)
{
	preprocess_ast_with_context(node, my_env, 0);
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
    // if (handle_builtins(node, env))
    //     return ;
    if (node->type == T_PIPE && node->left && node->right)
	{
		
        execute_pipe(node, env);
	}
    else
	{
		
        execute_simple_cmd(node, env);
	}
}

// void execute_pipe(t_ast_node *node, t_myenv *env)
// {
//     int    fd[2], saved = dup(STDOUT_FILENO), bi, l, r;
//     if (pipe(fd) == -1) { perror("pipe"); env->exit_code = 1; return; }

//     bi = node->left->type==T_SCMD; // && handle_builtins(node->left, env)
//     if (bi) {
//         close(fd[0]);
//         dup2(fd[1], STDOUT_FILENO);
//         close(fd[1]);
//         dup2(saved, STDOUT_FILENO);
//         close(saved);
//     } else {
//         l = fork();
//         if (l == 0) {
//             close(fd[0]);
//             dup2(fd[1], STDOUT_FILENO);
//             close(fd[1]);
//             execute_ast(node->left, env);
//             exit(env->exit_code);
//         }
//         dup2(saved, STDOUT_FILENO);
//         close(saved);
//     }

//     r = fork();
//     if (r == 0) {
//         close(fd[1]);
//         dup2(fd[0], STDIN_FILENO);
//         close(fd[0]);
//         execute_ast(node->right, env);
//         exit(env->exit_code);
//     }

//     close(fd[0]);
//     close(fd[1]);
//     if (!bi) waitpid(l, NULL, 0);
//     waitpid(r, NULL, 0);
// }


void execute_pipe(t_ast_node *node, t_myenv *env)
{
    int   fd[2];
    pid_t l, r;
    int   s_left = 0, s_right = 0;

    if (pipe(fd) == -1) {
        perror("pipe");
        env->exit_code = 1;
        return;
    }

    /* ----- LEFT CHILD ----- */
    l = fork();
    if (l == 0) {
        /* child: left writer */
        close(fd[0]);                                /* no read */
        if (dup2(fd[1], STDOUT_FILENO) == -1) {      /* stdout -> pipe */
            perror("dup2");
            _exit(1);
        }
        close(fd[1]);

        execute_ast(node->left, env);                /* <-- always run node */
        _exit(env->exit_code);
    } else if (l < 0) {
        perror("fork");
        close(fd[0]); close(fd[1]);
        env->exit_code = 1;
        return;
    }

    /* ----- RIGHT CHILD ----- */
    r = fork();
    if (r == 0) {
        /* child: right reader */
        close(fd[1]);                                /* no write */
        if (dup2(fd[0], STDIN_FILENO) == -1) {       /* stdin <- pipe */
            perror("dup2");
            _exit(1);
        }
        close(fd[0]);

        execute_ast(node->right, env);               /* <-- always run node */
        _exit(env->exit_code);
    } else if (r < 0) {
        perror("fork");
        close(fd[0]); close(fd[1]);
        waitpid(l, NULL, 0);                         /* reap left to avoid zombie */
        env->exit_code = 1;
        return;
    }

    /* parent: close both ends and wait */
    close(fd[0]);
    close(fd[1]);

    /* Close any heredoc fds in parent to ensure proper EOF signaling */
    if (node->left && node->left->heredoc_fd != -1) {
        close(node->left->heredoc_fd);
        node->left->heredoc_fd = -1;
    }
    if (node->right && node->right->heredoc_fd != -1) {
        close(node->right->heredoc_fd);
        node->right->heredoc_fd = -1;
    }

    waitpid(l, &s_left, 0);
    waitpid(r, &s_right, 0);

    /* Shell exit code = last command’s status (bash behavior) */
    if (WIFEXITED(s_right))
        env->exit_code = WEXITSTATUS(s_right);
    else if (WIFSIGNALED(s_right))
        env->exit_code = 128 + WTERMSIG(s_right);
    else
        env->exit_code = 1;
}

void add_or_update_env_var(t_myenv *env, const char *key, const char *value)
{
	int i = 0;
	size_t len = strlen(key);
	char *new_var;
	
	new_var = malloc(strlen(key) + strlen(value) + 2); // "KEY=VALUE\0"
	if (!new_var) return;
	sprintf(new_var, "%s=%s", key, value);

	while (env->env && env->env[i])
	{
		if (strncmp(env->env[i], key, len) == 0 && env->env[i][len] == '=')
		{
			free(env->env[i]);
			env->env[i] = new_var;
			return;
		}
		i++;
	}

	// Add new variable
	char **new_env = malloc(sizeof(char *) * (i + 2));
	for (int j = 0; j < i; j++)
		new_env[j] = env->env[j];
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	free(env->env);
	env->env = new_env;
}

