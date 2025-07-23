/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handling.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 12:30:30 by cdiab             #+#    #+#             */
/*   Updated: 2025/07/21 00:58:21 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_pipe(int *i, t_tokenlist *token_list)
{
	t_tokennode	*token;

	token = create_tokennode("|", T_PIPE);
	addtoken(token_list, token->token, T_PIPE);
	(*i)++;
}

// void	handle_redirection(int *i, t_tokenlist *token_list, const char *input)
// {
// 	t_tokennode	*token;

// 	if (input[*i] == '<')
// 	{
// 		if (strncmp(input + *i, "<<", 2) == 0)
// 			token = create_tokennode("<<", T_SCMD);
// 		else
// 			token = create_tokennode("<", T_SCMD);
// 	}
// 	else if (input[*i] == '>')
// 	{
// 		if (strncmp(input + *i, ">>", 2) == 0)
// 			token = create_tokennode(">>", T_SCMD);
// 		else
// 			token = create_tokennode(">", T_SCMD);
// 	}
// 	addtoken(token_list, token->token, T_SCMD);
// 	(*i)++;
// }

void handle_redirection(int *i, t_tokenlist *token_list, const char *input)
{
	t_tokennode *token;

	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		token = create_tokennode("<<", T_HEREDOC);
		*i += 2;
	}
	else if (input[*i] == '<')
	{
		token = create_tokennode("<", T_REDIR_IN);
		(*i)++;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		token = create_tokennode(">>", T_APPEND);
		*i += 2;
	}
	else if (input[*i] == '>')
	{
		token = create_tokennode(">", T_REDIR_OUT);
		(*i)++;
	}
	else
		return;
	addtoken(token_list, token->token, token->type);
}


// char	*ft_strndup(const char *start, size_t length)
// {
// 	char	*dup;
// 	char	*dst;

// 	dup = (char *)malloc(length + 1);
// 	if (dup == NULL)
// 		return (NULL);
// 	dst = dup;
// 	while (length-- && *start)
// 		*dst++ = *start++;
// 	*dst = '\0';
// 	return (dup);
// }

void	handle_envariable(int *i, t_tokenlist *tokenlist, const char *input)
{
	const char	*start;
	char		*env_str;
	t_tokennode	*token;

	start = input + *i;
	if (input[*i] != '\0')
		(*i)++;
	if (isalnum(input[*i]) || input[*i] == '_')
	{
		while (isalnum(input[*i]) || input[*i] == '_')
			(*i)++;
	}
	else if (input[*i] != '\0')
		(*i)++;
	env_str = ft_strndup(start, *i - (start - input));
	token = create_tokennode(env_str, T_SCMD);
	addtoken(tokenlist, token->token, T_SCMD);
	free(env_str);
}

void	handle_tilde(int *i, t_tokenlist *token_list, const char *input)
{
	char		tilde_str[3];
	t_tokennode	*token;

	if (input[*i + 1] && (input[*i + 1] == '-'
			|| input[*i + 1] == '$' || input[*i + 1] == '/'))
	{
		tilde_str[0] = input[*i];
		tilde_str[1] = input[*i + 1];
		tilde_str[2] = '\0';
		(*i) += 2;
	}
	else
	{
		tilde_str[0] = input[*i];
		tilde_str[1] = '\0';
		(*i)++;
	}
	token = create_tokennode(tilde_str, T_SCMD);
	addtoken(token_list, token->token, T_SCMD);
}

// int main() {
//     const char *input = "ls -l > output.txt | cat < input.txt $HOME";
//     t_tokenlist token_list = {NULL, NULL};
//     int i = 0; 

//     while (input[i] != '\0') {
//         if (input[i] == '|') {
//             handle_pipe(&i, &token_list, input);
//         } else if (input[i] == '<' || input[i] == '>') {
//             handle_redirection(&i, &token_list, input);
//         } else if (input[i] == '$') {
//             handle_envariable(&i, &token_list, input);
//         } else {
//             i++;
//         }
//     }

//     t_tokennode *node = token_list.head;
//     while (node) {
//         printf("Token: '%s', Type: %d\n", node->token, node->type);
//         node = node->next; 
//     }

//     node = token_list.head;
//     while (node) {
//         t_tokennode *temp = node;
//         node = node->next;
//         free(temp->token);
//         free(temp);
//     }

//     return 0;
// }
