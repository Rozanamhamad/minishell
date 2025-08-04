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


static int is_in_pipe_context = 0;

void set_pipe_context(int in_pipe)
{
    is_in_pipe_context = in_pipe;
}

void	handle_heredoc(t_ast_node *node)
{
	int		pipefd[2];
	char	*line;
	char	*prompt;

	if (pipe(pipefd) == -1)
		return (perror("pipe"), (void)0);

	// Use context-aware prompt
	prompt = is_in_pipe_context ? "pipe heredoc> " : "heredoc> ";

	while (1)
	{
		line = readline(prompt);

		if (!line || strcmp(line, node->in) == 0)
		{
			free(line);
			break;
		}
		write(pipefd[1], line, strlen(line));
		write(pipefd[1], "\n", 1);
		free(line);
	}

	close(pipefd[1]);
	node->heredoc_fd = pipefd[0];  // Store fd instead of immediate dup2
}
