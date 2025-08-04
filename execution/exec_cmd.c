/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 22:05:25 by ral-moha          #+#    #+#             */
/*   Updated: 2025/07/28 23:33:20 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void execute_simple_cmd(t_ast_node *node, t_myenv *env)
{
    int   fd_in   = -1;
    int   fd_out  = -1;
    int   saved_in  = -1;
    int   saved_out = -1;
    pid_t pid;
    int   status;

    if (!node || !node->arr || !node->arr[0])
        return;

    // Heredoc is now handled in preprocessing phase
    // if (node->ex_heredoc)
    //     handle_heredoc(node);

    /* Only open files here; do NOT dup2 inside setup_redirections */
    if (!setup_redirections(node, &fd_in, &fd_out, env))
        return;

    /* --- Builtin: run in parent with redirs applied, then restore --- */
    if (is_builtin(node->arr[0])) {
        if (fd_in != -1) {
            saved_in = dup(STDIN_FILENO);
            if (saved_in == -1 || dup2(fd_in, STDIN_FILENO) == -1) {
                perror("dup2");
                if (saved_in != -1) close(saved_in);
                if (fd_in  != -1) close(fd_in);
                if (fd_out != -1) close(fd_out);
                env->exit_code = 1;
                return;
            }
            close(fd_in); fd_in = -1;
        }
        if (fd_out != -1) {
            saved_out = dup(STDOUT_FILENO);
            if (saved_out == -1 || dup2(fd_out, STDOUT_FILENO) == -1) {
                perror("dup2");
                if (saved_in  != -1) { dup2(saved_in, STDIN_FILENO);  close(saved_in); }
                if (fd_out != -1) close(fd_out);
                env->exit_code = 1;
                return;
            }
            close(fd_out); fd_out = -1;
        }

        /* run the builtin now that fds are redirected */
        exec_builtin(node, env);

        /* restore */
        if (saved_in  != -1) { dup2(saved_in,  STDIN_FILENO);  close(saved_in); }
        if (saved_out != -1) { dup2(saved_out, STDOUT_FILENO); close(saved_out); }
        return;
    }

    /* --- External command path --- */
    pid = fork();
    if (pid < 0) {
        perror("fork");
        if (fd_in  != -1) close(fd_in);
        if (fd_out != -1) close(fd_out);
        env->exit_code = 1;
        return;
    } else if (pid == 0) {
        if (fd_in  != -1) { if (dup2(fd_in,  STDIN_FILENO)  == -1) { perror("dup2"); _exit(1);} close(fd_in); }
        if (fd_out != -1) { if (dup2(fd_out, STDOUT_FILENO) == -1) { perror("dup2"); _exit(1);} close(fd_out); }
        execve(find_path(node->arr[0], env), node->arr, env->env);
        perror("execve");
        _exit(127);
    }

    if (fd_in  != -1) close(fd_in);
    if (fd_out != -1) close(fd_out);

    if (waitpid(pid, &status, 0) == -1) {
        perror("waitpid");
        env->exit_code = 1;
    } else if (WIFEXITED(status)) {
        env->exit_code = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        env->exit_code = 128 + WTERMSIG(status);
    }
}

int setup_redirections(t_ast_node *node, int *fd_in, int *fd_out, t_myenv *env)
{
    int fd;
    int input_failed = 0;
    int output_failed = 0;

    if (!node) return 0;
    if (fd_in)  *fd_in  = -1;
    if (fd_out) *fd_out = -1;

    /* input: '< file' or heredoc */
    if (node->heredoc_fd != -1) {
        // Use heredoc file descriptor
        if (fd_in) *fd_in = node->heredoc_fd;
    } else if (node->in) {
        char *clean_filename = clean_quotations(node->in);
        fd = open(clean_filename, O_RDONLY);
        if (fd == -1) {
            perror(clean_filename);
            env->exit_code = 1;
            input_failed = 1;
        } else {
            if (fd_in) *fd_in = fd;
        }
        free(clean_filename);
    }

    /* output: '>' or '>>' */
    if (node->out) {
        char *clean_filename = clean_quotations(node->out);
        int flags = O_WRONLY | O_CREAT | (node->app ? O_APPEND : O_TRUNC);
        fd = open(clean_filename, flags, 0644);
        if (fd == -1) {
            perror(clean_filename);
            if (fd_in && *fd_in != -1) close(*fd_in);
            env->exit_code = 1;
            output_failed = 1;
        } else {
            if (fd_out) *fd_out = fd;
        }
        free(clean_filename);
    }
    
    // Return success only if both redirections succeeded
    return !(input_failed || output_failed);
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
