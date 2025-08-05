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
    
    // Filter out empty arguments (e.g., from $EMPTY expansion)
    char **filtered_args = NULL;
    int arg_count = 0;
    int i = 0;
    
    // Count non-empty arguments
    while (node->arr[i])
    {
        if (node->arr[i][0] != '\0')
            arg_count++;
        i++;
    }
    
    // If no non-empty arguments, handle empty command
    if (arg_count == 0)
    {
        if (!setup_redirections(node, &fd_in, &fd_out, env))
            return;
        if (fd_in != -1) close(fd_in);
        if (fd_out != -1) close(fd_out);
        env->exit_code = 0;
        return;
    }
    
    // Create filtered array with non-empty args
    filtered_args = malloc(sizeof(char*) * (arg_count + 1));
    if (!filtered_args)
    {
        env->exit_code = 1;
        return;
    }
    
    i = 0;
    int j = 0;
    while (node->arr[i])
    {
        if (node->arr[i][0] != '\0')
            filtered_args[j++] = node->arr[i];
        i++;
    }
    filtered_args[j] = NULL;
    
    // Replace the original array temporarily
    char **original_arr = node->arr;
    node->arr = filtered_args;

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
        
        /* cleanup and restore original array */
        free(filtered_args);
        node->arr = original_arr;
        return;
    }

    /* --- External command path --- */
    pid = fork();
    if (pid < 0) {
        perror("fork");
        if (fd_in  != -1) close(fd_in);
        if (fd_out != -1) close(fd_out);
        free(filtered_args);
        node->arr = original_arr;
        env->exit_code = 1;
        return;
    } else if (pid == 0) {
        // Set up default signal behavior for child process
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        
        if (fd_in  != -1) { if (dup2(fd_in,  STDIN_FILENO)  == -1) { perror("dup2"); _exit(1);} close(fd_in); }
        if (fd_out != -1) { if (dup2(fd_out, STDOUT_FILENO) == -1) { perror("dup2"); _exit(1);} close(fd_out); }
        
        char *cmd_path = find_path(node->arr[0], env);
        if (!cmd_path) {
            // Command not found in PATH or NULL command
            write(STDERR_FILENO, node->arr[0], strlen(node->arr[0]));
            write(STDERR_FILENO, ": command not found\n", 20);
            _exit(127);
        }
        
        // Check if path exists and get its status
        struct stat st;
        if (stat(cmd_path, &st) == -1) {
            perror(cmd_path);
            free(cmd_path);
            _exit(127);  // File doesn't exist
        }
        
        // Check if it's a directory
        if (S_ISDIR(st.st_mode)) {
            write(STDERR_FILENO, cmd_path, strlen(cmd_path));
            write(STDERR_FILENO, ": Is a directory\n", 17);
            free(cmd_path);
            _exit(126);
        }
        
        // Try to execute
        execve(cmd_path, node->arr, env->env);
        
        // If execve failed, check why
        if (access(cmd_path, F_OK) != 0) {
            perror(cmd_path);
            free(cmd_path);
            _exit(127);  // File doesn't exist
        } else if (access(cmd_path, X_OK) != 0) {
            write(STDERR_FILENO, cmd_path, strlen(cmd_path));
            write(STDERR_FILENO, ": Permission denied\n", 20);
            free(cmd_path);
            _exit(126);  // Permission denied
        } else {
            perror(cmd_path);
            free(cmd_path);
            _exit(126);  // Other execution error
        }
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
    
    /* cleanup and restore original array */
    free(filtered_args);
    node->arr = original_arr;
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
    /* Note: In a pipeline, stdin might already be connected to a pipe.
       Only use heredoc if stdin is not already redirected */
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
	// Set up default signal behavior for child process
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	
	if (fd_in != -1)
		dup2(fd_in, STDIN_FILENO);
	if (fd_out != -1)
		dup2(fd_out, STDOUT_FILENO);
	if (fd_in != -1) close(fd_in);
	if (fd_out != -1) close(fd_out);

	char *cmd_path = find_path(node->arr[0], env);
	if (!cmd_path) {
		write(STDERR_FILENO, node->arr[0], strlen(node->arr[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		exit(127);
	}
	
	struct stat st;
	if (stat(cmd_path, &st) == -1) {
		perror(cmd_path);
		free(cmd_path);
		exit(127);
	}
	
	if (S_ISDIR(st.st_mode)) {
		write(STDERR_FILENO, cmd_path, strlen(cmd_path));
		write(STDERR_FILENO, ": Is a directory\n", 17);
		free(cmd_path);
		exit(126);
	}
	
	execve(cmd_path, node->arr, env->env);
	
	if (access(cmd_path, F_OK) != 0) {
		perror(cmd_path);
		free(cmd_path);
		exit(127);
	} else if (access(cmd_path, X_OK) != 0) {
		write(STDERR_FILENO, cmd_path, strlen(cmd_path));
		write(STDERR_FILENO, ": Permission denied\n", 20);
		free(cmd_path);
		exit(126);
	} else {
		perror(cmd_path);
		free(cmd_path);
		exit(126);
	}
}
