/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:15:37 by cdiab             #+#    #+#             */
/*   Updated: 2025/07/29 13:05:34 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int  list_size(t_tokenlist *lst)
{
	t_tokennode *cur = lst->head;
	int          n   = 0;

	while (cur)
	{
		n++;
		cur = cur->next;
	}
	return (n);
}

char    **list_to_arr(t_tokenlist *lst, t_ast_node *node)
{
	int           	n = list_size(lst);
	char			**arr;
	t_tokennode		*cur;
	int				i;

	arr = ft_calloc(n + 1, sizeof(char *)); 
	if (!arr)
		return (NULL);
	i = 0;
	cur = lst->head;
	(void)node;
	while (cur)
	{
		if ((strcmp(cur->token, "<") == 0
			|| strcmp(cur->token, ">") == 0) && cur->next)
		{
			in_out(cur, node);
			cur = cur->next->next;
		}
		else if ((strcmp(cur->token, "<<") == 0
			|| strcmp(cur->token, ">>") == 0)  && cur->next)
		{
			app_heredoc(cur, node);
			cur = cur->next->next;
		}
		else
		{
			arr[i++] = ft_strdup(cur->token);
			cur = cur->next;
		}
	}
	arr[i] = NULL;
	return (arr);
}

// char **list_to_arr(t_tokenlist *lst, t_ast_node *node)
// {
// 	int n = list_size(lst), i = 0;
// 	t_tokennode *cur = lst->head;
// 	char **arr = ft_calloc(n + 1, sizeof(char *));
// 	if (!arr) return (NULL);
// 	while (cur)
// 	{
// 		char *tok = cur->token;
// 		if ((strcmp(tok, "<") == 0 || strcmp(tok, ">") == 0) && cur->next)
// 			in_out(cur, node), cur = cur->next->next;
// 		else if ((strcmp(tok, "<<") == 0 || strcmp(tok, ">>") == 0) && cur->next)
// 			app_heredoc(cur, node), cur = cur->next->next;
// 		else
// 			arr[i++] = ft_strdup(tok), cur = cur->next;
// 	}
// 	arr[i] = NULL;
// 	return (arr);
// }

void	in_out(t_tokennode *token, t_ast_node *node)
{
    t_tokennode *cur = token;

    if (strcmp(cur->token, "<") == 0 && cur->next && cur->next->token) {
        free(node->in);
        node->in = ft_strdup(cur->next->token);
    } else if (strcmp(cur->token, ">") == 0 && cur->next && cur->next->token) {
        free(node->out);
        node->out = ft_strdup(cur->next->token);
        node->app = 0;  // '>'
    }
}

void	app_heredoc(t_tokennode *token, t_ast_node *node)
{
    t_tokennode *cur = token;

    if (strcmp(cur->token, ">>") == 0 && cur->next && cur->next->token) {
        free(node->out);
        node->out = ft_strdup(cur->next->token);
        node->app = 1;  // '>>'
    } else if (strcmp(cur->token, "<<") == 0 && cur->next && cur->next->token) {
        free(node->in);
        node->in = ft_strdup(cur->next->token);   // store limiter
        node->ex_heredoc = 1;
    }
}
t_ast_node	*create_node(char **arr)
{
	t_ast_node	*node;

	node = ft_calloc(1, sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->arr = arr;
	return (node);
}

// void    print_ast(t_ast_node *node, int level)
// {
// 	int i;
// 	if (node == NULL)
// 		return;
// 	// Indent based on level
// 	for (i = 0; i < level; i++)
// 		printf("    ");
// 	// Print current node info (you can customize this)
// 	if (node->type == T_SCMD)
// 		printf("Node (Type: SC)\n");
// 	else if (node->type == T_PIPE)
// 		printf("Node (Type: PIPE)\n");
// 	// Print arr if it exists
// 	if (node->arr)
// 	{
// 		for (i = 0; node->arr[i]; i++)
// 		{
// 			int indent;
// 			for (indent = 0; indent <= level; indent++)
// 				printf("    ");
// 			printf("arr[%d]: %s\n", i, node->arr[i]);
// 		}
// 	}
// 	// Print other node attributes if you want
// 	if (node->in)
// 	{
// 		for (i = 0; i <= level; i++)
// 			printf("    ");
// 		printf("Input Redirection: %s\n", node->in);
// 	}
// 	if (node->out)
// 	{
// 		for (i = 0; i <= level; i++)
// 			printf("    ");
// 		printf("Output Redirection: %s\n", node->out);
// 	}
// 	if (node->ex_heredoc)
// 	{
// 		for (i = 0; i <= level; i++)
// 			printf("    ");
// 		printf("Heredoc: %d\n", node->ex_heredoc);
// 	}
// 	if (node->app)
// 	{
// 		for (i = 0; i <= level; i++)
// 			printf("    ");
// 		printf("app: %d\n", node->app);
// 	}
// 	// Recursively print left and right children
// 	if (node->left || node->right)
// 	{
// 		for (i = 0; i <= level; i++)
// 			printf("    ");
// 		printf("Left:\n");
// 		print_ast(node->left, level + 1);
// 		for (i = 0; i <= level; i++)
// 			printf("    ");
// 		printf("Right:\n");
// 		print_ast(node->right, level + 1);
// 	}
// }

t_ast_node	*build_ast(t_tokenlist *tokens)
{
	t_ast_node	*root;
	t_tokenlist	*left;
	t_tokenlist	*right;

	left = NULL;
	right = NULL;
	split_by_pipe(tokens, &left, &right);
	if (right)
	{
		root = ft_calloc(1, sizeof(t_ast_node));
		if (!root)
			return (NULL);
		root->type = T_PIPE;
		root->in = NULL;
		root->out = NULL;
		root->arr = NULL;
		root->app = 0;
		root->ex_heredoc = 0;
		root->left = build_ast(left);
		root->right = build_ast(right);
	}
	else
	{
		root = ft_calloc(1, sizeof(t_ast_node));
		root->type = T_SCMD;
		root->arr = list_to_arr(tokens, root);
	}
	return (root);
}

// t_ast_node *parse_simple_command(t_tokenlist *tokens)
// {
//     t_ast_node *node = create_ast_node(T_SCMD);
//     int argc = 0;

//     node->arr = malloc(sizeof(char *) * (tokens->count + 1));
//     node->in = NULL;
//     node->out = NULL;
//     node->append = NULL;
//     node->heredoc = NULL;
//     node->ex_heredoc = false;

//     for (int i = 0; i < tokens->count; i++)
//     {
//         t_tokennode *curr = tokens->nodes[i];

//         if (curr->type == T_REDIR_IN || curr->type == T_REDIR_OUT ||
//             curr->type == T_APPEND || curr->type == T_HEREDOC)
//         {
//             if (i + 1 >= tokens->count || tokens->nodes[i + 1]->type != T_WORD)
//             {
//                 fprintf(stderr, "minishell: syntax error near redirection\n");
//                 return NULL;
//             }

//             char *filename = strdup(tokens->nodes[i + 1]->token);

//             if (curr->type == T_REDIR_IN)
//                 node->in = filename;
//             else if (curr->type == T_REDIR_OUT)
//                 node->out = filename;
//             else if (curr->type == T_APPEND)
//                 node->append = filename;
//             else if (curr->type == T_HEREDOC) {
//                 node->heredoc = filename;
//                 node->ex_heredoc = true;
//             }

//             i++; // skip filename
//         }
//         else if (curr->type == T_WORD)
//         {
//             node->arr[argc++] = strdup(curr->token);
//         }
//     }

//     node->arr[argc] = NULL;
//     return node;
// }
