/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 12:15:37 by cdiab             #+#    #+#             */
/*   Updated: 2025/08/05 20:50:06 by ral-moha         ###   ########.fr       */
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
	int				input_comes_first = 0;
	int				found_input = 0;
	int				found_output = 0;

	arr = ft_calloc(n + 1, sizeof(char *)); 
	if (!arr)
		return (NULL);
	i = 0;
	cur = lst->head;
	(void)node;
	
	// First pass: determine redirection order (only for multiple redirections)
	t_tokennode *temp = cur;
	while (temp)
	{
		if (strcmp(temp->token, "<") == 0) found_input = 1;
		if (strcmp(temp->token, ">") == 0 || strcmp(temp->token, ">>") == 0) found_output = 1;
		temp = temp->next;
	}
	
	// Only apply ordering logic if we have BOTH input AND output redirections
	if (found_input && found_output) {
		temp = cur;
		int seen_output = 0, seen_input = 0;
		while (temp) {
			if (strcmp(temp->token, "<") == 0 && !seen_output) {
				input_comes_first = 1;
				seen_input = 1;
			} else if ((strcmp(temp->token, ">") == 0 || strcmp(temp->token, ">>") == 0) && !seen_input) {
				input_comes_first = 0;
				seen_output = 1;
			} else if (strcmp(temp->token, ">") == 0 || strcmp(temp->token, ">>") == 0) {
				seen_output = 1;
			} else if (strcmp(temp->token, "<") == 0) {
				seen_input = 1;
			}
			temp = temp->next;
		}
	}
	
	// Second pass: process tokens with order awareness
	while (cur)
	{
		if ((strcmp(cur->token, "<") == 0
			|| strcmp(cur->token, ">") == 0) && cur->next)
		{
			// For ALL input redirections, check if file exists
			if (strcmp(cur->token, "<") == 0)
			{
				// Check if input file exists - fail on first missing file
				char *clean_filename = clean_quotations(cur->next->token);
				int fd = open(clean_filename, O_RDONLY);
				if (fd == -1)
				{
					// Input file missing - don't process any subsequent redirections
					free(node->in);
					node->in = ft_strdup(cur->next->token);
					free(clean_filename);
					arr[i] = NULL;
					return (arr);
				}
				close(fd);
				free(clean_filename);
			}
			in_out(cur, node);
			cur = cur->next->next;
		}
		else if ((strcmp(cur->token, "<<") == 0
			|| strcmp(cur->token, ">>") == 0)  && cur->next)
		{
			// For append redirections, check if this is the first redirection
			if (strcmp(cur->token, ">>") == 0 && !input_comes_first && !node->out)
			{
				// First redirection is >>, check for permission errors
				int fd = open(cur->next->token, O_WRONLY | O_CREAT | O_APPEND, 0644);
				if (fd == -1)
				{
					// First append failed - don't process subsequent redirections
					node->out = ft_strdup(cur->next->token);
					node->app = 1;
					arr[i] = NULL;
					return (arr);
				}
				close(fd);
			}
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
    int fd;
    char *clean_filename;

    if (strcmp(cur->token, "<") == 0 && cur->next && cur->next->token) {
        free(node->in);
        node->in = ft_strdup(cur->next->token);
    } else if (strcmp(cur->token, ">") == 0 && cur->next && cur->next->token) {
        // Handle mixed redirections: >> followed by >
        if (node->out && node->app == 1) {
            // We had >> before, now we have > - need to process >> first during execution
            // Open the append file first to check for errors
            clean_filename = clean_quotations(node->out);
            fd = open(clean_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) {
                // First redirection failed - this should stop execution entirely
                // Don't show error here, let execution stage handle it
                free(clean_filename);
                // Don't process the second redirection
                return;
            } else {
                close(fd);  // Just check, don't use
            }
            free(clean_filename);
            free(node->out);
        } else if (node->out) {
            // Multiple > redirections - process first redirection and stop on failure
            clean_filename = clean_quotations(node->out);
            fd = open(clean_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                // First redirection failed - stop processing subsequent redirections
                // Don't show error here, let execution stage handle it
                free(clean_filename);
                // Don't process the second redirection
                return;
            } else {
                close(fd);
            }
            free(clean_filename);
            free(node->out);
        }
        node->out = ft_strdup(cur->next->token);
        node->app = 0;  // '>'
    }
}

void	app_heredoc(t_tokennode *token, t_ast_node *node)
{
    t_tokennode *cur = token;

    if (strcmp(cur->token, ">>") == 0 && cur->next && cur->next->token) {
        // Handle mixed redirections: > followed by >> (for Test 62)
        if (node->out && node->app == 0) {
            // We had > before, now we have >> - create the > file first (bash behavior)
            char *clean_filename = clean_quotations(node->out);
            int fd = open(clean_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd != -1) {
                close(fd);  // Successfully created first file
            }
            // Continue processing - let execution handle any errors
            free(clean_filename);
            free(node->out);
        } else if (node->out && node->app) {
            // For multiple append redirections: open all files but keep only the last one
            char *clean_filename = clean_quotations(node->out);
            int fd = open(clean_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd == -1) {
                // First append redirection failed - stop processing subsequent redirections
                // Don't show error here, let execution stage handle it
                free(clean_filename);
                // Don't process the second redirection
                return;
            } else {
                close(fd);  // Just create, don't use
            }
            free(clean_filename);
            free(node->out);
        }
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
	node->heredoc_fd = -1;
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
		root->heredoc_fd = -1;
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
		root->heredoc_fd = -1;
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
