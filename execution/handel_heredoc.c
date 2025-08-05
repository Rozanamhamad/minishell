/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handel_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:48:44 by ral-moha          #+#    #+#             */
/*   Updated: 2025/08/05 20:51:20 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	handle_heredoc(t_ast_node *node)
{
	int		pipefd[2];
	char	*line;

	// printf("HERE!\n");
	if (pipe(pipefd) == -1)
		return (perror("pipe"), (void)0);

	while (1)
	{
		line = readline("> ");

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
	node->heredoc_fd = pipefd[0];
}
