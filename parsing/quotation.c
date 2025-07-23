/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdiab <cdiab@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 15:48:38 by cdiab             #+#    #+#             */
/*   Updated: 2025/07/06 16:08:29 by cdiab            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*process_quotes(const char *input)
{
	char		*result;
	int			i;
	int			j;
	int			single_q;
	int			double_q;

	i = 0;
	j = 0;
	single_q = 0;
	double_q = 0;
	result = malloc(strlen(input)+ 1);
	if (!result)
		return (NULL);
	while (input[i])
	{
		if (input[i] == '"' && !single_q)
		{
			double_q = !double_q;
			i++;
			continue ;
		}
		else if (input[i] == '\'' && !double_q)
		{
			single_q = !single_q;
			i++;
			continue ;
		}
		result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}

static void	update_quotes(char c, t_ws *s)
{
	if (c == '"' && !s->sq)
		s->dq = !s->dq;
	else if (c == '\'' && !s->dq)
		s->sq = !s->sq;
}

static void	copy_char_or_space(const char *str, char *res, t_ws *s)
{
	if ((str[s->i] == ' ' || str[s->i] == '\t') && !s->sq && !s->dq)
		s->space = 1;
	else
	{
		if (s->space && s->j > 0)
			res[(s->j)++] = ' ';
		s->space = 0;
		res[(s->j)++] = str[s->i];
	}
	s->i++;
}

char	*clean_whitespace(const char *str)
{
	t_ws	s;
	char	*res;

	s.i = 0;
	s.j = 0;
	s.sq = 0;
	s.dq = 0;
	s.space = 0;
	res = malloc(strlen(str) + 1);
	if (!res)
		return (NULL);
	while (str[s.i] && (str[s.i] == ' ' || str[s.i] == '\t'))
		s.i++;
	while (str[s.i])
	{
		update_quotes(str[s.i], &s);
		copy_char_or_space(str, res, &s);
	}
	res[s.j] = '\0';
	return (res);
}


char	*clean_quotations(const char *input)
{
	char	*result;
	char	*no_spaces;

	no_spaces = clean_whitespace(input);
	if (!no_spaces)
		return (NULL);
	result = process_quotes(no_spaces);
	free(no_spaces);
	return (result);
}
// int	main(void)
// {
// 	const char *input = "\"hello world\"\"asd\"         \"lol       'test1       test2'      $HOME    \"$HOME\"     '$HOME'   carla       123\" '$HOME' 'pip \"poop\" pippoop \"$USER\"kkk'";
// 	char *output;

// 	printf("Original input:\n%s\n\n", input);

// 	output = clean_quotations(input);
// 	if (!output)
// 	{
// 		fprintf(stderr, "Error: memory allocation failed.\n");
// 		return (1);
// 	}

// 	printf("Processed input:\n%s\n", output);
// 	free(output);
// 	return (0);
// }
