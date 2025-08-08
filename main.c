/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:22:33 by cdiab             #+#    #+#             */
/*   Updated: 2025/08/08 23:30:55 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int ac, char **av, char **env)
{
	t_tokenlist	*tokenlist; //holds split words from input
	char		*input; //user's command string
	t_ast_node	*root; //AST tree
	t_myenv		*myenv;

	(void)av;
	(void)ac;
	tokenlist = create_tokenlist();//make empty token list 
	myenv = malloc(sizeof(t_myenv));
	myenv->env = copy_env(env);
	myenv->exit_code = 0;
	
	prep_signals(); //Set Ctrl+C, Ctrl
	
	input = NULL;
	while (1)
	{
		input = readline("minishell>");//Show prompt and wait for user input

		ft_check_signal(myenv);// Check if Ctrl+C happened before input
		
		// Handle Ctrl-D (EOF)
		if (input == NULL)
		{
			write(STDOUT_FILENO, "exit\n", 5);
			break;
		}
		
		// Skip empty input //press Enter
		if (input[0] == '\0')
		{
			free(input);
			continue;
		}
		
		// Add to history
		add_history(input);
		
		// Parse and execute
		tokenlist = tokenizeInput(input);//break input into tokens
		root = build_ast(tokenlist);//Build AST tree from tokens (parser)
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
