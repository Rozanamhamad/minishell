/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:48:44 by ral-moha          #+#    #+#             */
/*   Updated: 2025/08/07 15:53:53 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


// void	handle_heredoc(t_ast_node *node)
// {
// 	int		pipefd[2];
// 	char	*line;
// 	pid_t	pid;
// 	int		status;

// 	if (pipe(pipefd) == -1)
// 		return (perror("pipe"), (void)0);

// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		close(pipefd[0]);
// 		close(pipefd[1]);
// 		return;
// 	}

// 	if (pid == 0)
// 	{
// 		// Child process: handle heredoc input
// 		close(pipefd[0]); // Child doesn't read
		
// 		// Set up heredoc-specific signal handler in child
// 		signal(SIGINT, ft_sigint_heredoc);

// 		while (1)
// 		{
// 			line = readline("> ");

// 			// Check for signal interruption or EOF (Ctrl-D) or delimiter
// 			if (!line || g_signal_exit_status == SIGINT)
// 			{
// 				if (line)
// 					free(line);
// 				close(pipefd[1]);
// 				exit(130); // Exit child with interrupted status
// 			}

// 			if (strcmp(line, node->in) == 0)
// 			{
// 				free(line);
// 				break;
// 			}
			
// 			write(pipefd[1], line, strlen(line));
// 			write(pipefd[1], "\n", 1);
// 			free(line);
// 		}

// 		close(pipefd[1]);
// 		exit(0); // Child completed successfully
// 	}
// 	else
// 	{
// 		// Parent process: wait for child
// 		close(pipefd[1]); // Parent doesn't write
		
// 		waitpid(pid, &status, 0);
		
// 		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
// 		{
// 			// Child was interrupted by Ctrl-C
// 			close(pipefd[0]);
// 			node->heredoc_fd = -1;
// 			return;
// 		}
// 		else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
// 		{
// 			// Child completed successfully
// 			node->heredoc_fd = pipefd[0];
// 		}
// 		else
// 		{
// 			// Some other error
// 			close(pipefd[0]);
// 			node->heredoc_fd = -1;
// 		}
// 	}
// }

void	handle_heredoc(t_ast_node *node)
{
	int		pipefd[2];
	char	*line;
	pid_t	pid;
	int		status;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), (void)0);

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		close(pipefd[0]);
		close(pipefd[1]);
		return;
	}

	if (pid == 0)
	{
		// Child process: handle heredoc input
		close(pipefd[0]); // Child doesn't read
		signal(SIGINT, ft_sigint_heredoc); // Set heredoc-specific signal

		while (1)
		{
			line = readline("> ");

			// Ctrl+D (EOF)
			if (!line)
			{
				fprintf(stderr,
					"minishell: warning: here-document delimited by end-of-file (wanted `%s')\n",
					node->in);
				close(pipefd[1]);
				exit(0); // Considered successful
			}

			// Ctrl+C
			if (g_signal_exit_status == SIGINT)
			{
				free(line);
				close(pipefd[1]);
				exit(130); // Interrupted
			}

			// Delimiter matched
			if (strcmp(line, node->in) == 0)
			{
				free(line);
				break;
			}

			write(pipefd[1], line, strlen(line));
			write(pipefd[1], "\n", 1);
			free(line);
		}

		close(pipefd[1]);
		exit(0); // Success
	}
	else
	{
		// Parent process
		close(pipefd[1]); // Parent doesn't write
		waitpid(pid, &status, 0);

		if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
		{
			// Child interrupted by Ctrl+C
			close(pipefd[0]);
			node->heredoc_fd = -1;
			return;
		}
		else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
		{
			// Heredoc input completed
			node->heredoc_fd = pipefd[0];
		}
		else
		{
			// Error case
			close(pipefd[0]);
			node->heredoc_fd = -1;
		}
	}
}
