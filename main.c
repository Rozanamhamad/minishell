/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 11:22:33 by cdiab             #+#    #+#             */
/*   Updated: 2025/07/22 23:24:00 by ral-moha         ###   ########.fr       */
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
	// print_double_pointer(myenv->env);
	input = NULL;
	while (1)
	{
		input = readline("minishell>");
		if (input == NULL)
			return (0);
		if (input)
			add_history(input);
		tokenlist = tokenizeInput(input);
		// printTokens(tokenlist);
		// printTokens(tokenlist);
		root = build_ast(tokenlist);
		free_tokenlist(tokenlist);
		// print_ast(root, 0);
		execute_ast(root, myenv);
		// test(root);
		// print_ast(root, 0);
		(void)tokenlist;
	}
	free_env(myenv->env);
}

