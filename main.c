/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:22:33 by cdiab             #+#    #+#             */
/*   Updated: 2025/07/29 13:07:40 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **env)
{
	t_tokenlist	*tokenlist;
	char		*input;
	t_ast_node	*root;
	t_myenv		*myenv;

	(void)av;
	(void)ac;
	tokenlist = create_tokenlist();
	myenv = malloc(sizeof(t_myenv));
	myenv->env = copy_env(env);
	myenv->exit_code = 0;
	
	// Set up signal handlers for interactive mode
	prep_signals();
	
	input = NULL;
	while (1)
	{
		input = readline("minishell>");
		
		// Check for signals immediately after readline (like Ctrl-C at prompt)
		ft_check_signal(myenv);
		
		// Handle Ctrl-D (EOF)
		if (input == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		
		// Skip empty input
		if (input[0] == '\0')
		{
			free(input);
			continue;
		}
		
		// Add to history
		add_history(input);
		
		// Parse and execute
		tokenlist = tokenizeInput(input);
		root = build_ast(tokenlist);
		free_tokenlist(tokenlist);
		execute_ast(root, myenv);
		
		// Check if any signals were received during execution
		ft_check_signal(myenv);
		
		free(input);
	}
	
	// Cleanup
	free_env(myenv->env);
	free(myenv);
	return (myenv->exit_code);
}

