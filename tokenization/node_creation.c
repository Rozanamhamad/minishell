/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdiab <cdiab@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 12:12:51 by cdiab             #+#    #+#             */
/*   Updated: 2025/06/25 01:13:48 by cdiab            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_tokennode	*create_tokennode(const char *token, t_type type)
{
	t_tokennode	*new_node;

	new_node = (t_tokennode *)malloc(sizeof(t_tokennode));

	if (new_node == NULL)
		return (NULL);
	new_node->token = strdup(token);
	if (!new_node->token)
	{
		free(new_node);
		return (NULL);
	}
	new_node->type = type;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_tokenlist	*create_tokenlist(void)
{
	t_tokenlist	*list;

	list = (t_tokenlist *)malloc(sizeof(t_tokenlist));
	if (!list)
		return (NULL);
	list->head = NULL;
	return (list);
}

void addtoken(t_tokenlist *list, const char *token, t_type type)
{
	t_tokennode *newtoken = create_tokennode(token, type);
	if (!newtoken)
		return;
	if (list->head == NULL)
	{
		list->head = newtoken;
		list->tail = newtoken;
	}
	else
	{
		list->tail->next = newtoken;
		list->tail = newtoken;
	}
}

void	printTokens(const t_tokenlist *list)
{
	t_tokennode	*current;

	current = list->head;
	while (current != NULL)
	{
		printf("%s\n", current->token);
		current = current->next;
	}
}

void	free_tokenlist(t_tokenlist *list)
{
	t_tokennode	*current;
	t_tokennode	*newnode;

	current = list->head;
	while (current)
	{
		newnode = current->next;
		free(current->token);
		free(current);
		current = newnode;
	}
	free(list);
}

t_tokenlist	*tokenizeInput(const char *input)
{
	t_tokenlist	*list;
	int			i;

	i = 0;
	list = create_tokenlist();
	if (!list)
		return (NULL);
	
	while (input[i])
	{
		// Skip whitespace
		while (input[i] && (input[i] == ' ' || input[i] == '\t'))
			i++;
		if (!input[i])
			break;

		// Handle redirection operators
		if (input[i] == '<' || input[i] == '>')
		{
			handle_redirection(&i, list, input);
		}
		// Handle pipe operators
		else if (input[i] == '|')
		{
			handle_pipe(&i, list);
		}
		// Handle regular tokens
		else
		{
			char *token = get_token(input, &i);
			if (token)
			{
				addtoken(list, token, T_SCMD);
				free(token);
			}
		}
	}
	return (list);
}


// int main(void)
// {
//     const char *input = "\"hello asd\" 'zxc lol' qwe 66";       // your input string
//     t_tokenlist *tokenlist = tokenizeinput(input);

//     if (tokenlist)
//     {
//         printtoken(tokenlist);
//         free_tokenlist(tokenlist);
//     }

//     return 0;
// }