/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:48:44 by ral-moha          #+#    #+#             */
/*   Updated: 2025/07/21 00:53:09 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	handle_heredoc(t_ast_node *node)
{
	int		pipefd[2];           // Will hold file descriptors: pipefd[0] for reading, pipefd[1] for writing
	char	*line;               // To store each line the user inputs

	if (pipe(pipefd) == -1)      // Create a pipe. If it fails, print error and return.
		return (perror("pipe"), (void)0);

	while (1)                    // Infinite loop to read heredoc lines
	{
		line = readline("> ");   // Prompt the user with "> " and read input line

		if (!line || strcmp(line, node->in) == 0) // If user presses Ctrl+D or input matches limiter
		{
			free(line);         // Free the input line memory
			break;              // Exit the loop
		}
		write(pipefd[1], line, strlen(line)); // Write input line to pipe
		write(pipefd[1], "\n", 1);            // Add newline to simulate actual user input
		free(line);            // Free after writing to pipe
	}

	close(pipefd[1]);           // Close the write end of the pipe (we're done writing)
	dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to read from pipe (heredoc content)
	close(pipefd[0]);           // Close the read end after dup2 (no longer needed)
}
