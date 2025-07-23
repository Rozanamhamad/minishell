/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 15:18:16 by cdiab             #+#    #+#             */
/*   Updated: 2025/07/15 23:12:39 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_var_char(char c, int first)
{
	if (first)
		return (isalpha(c) || c == '_');
	return (isalnum(c) || c == '_');
}

int	get_var_name_len(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	if (!is_var_char(str[0], 1))
		return (0);
	while (str[i] && is_var_char(str[i], i == 0))
		i++;
	return (i);
}

char	*get_var_name(const char *str, int *i)
{
	size_t		len;
	char	*name;

	if (!str[*i])
		return (NULL);
	if (str[*i] == '?')
	{
		(*i)++;
		return (strdup("?"));
	}
	if (!is_var_char(str[*i], 1))
		return (NULL);
	len = get_var_name_len(&str[*i]);
	name = ft_strndup(&str[*i], len);
	*i += len;
	return (name);
}

char	*ft_strndup(const char *start, size_t length)
{
	char	*dup;
	char	*dst;

	dup = (char *)malloc(length + 1);
	if (dup == NULL)
		return (NULL);
	dst = dup;
	while (length-- && *start)
		*dst++ = *start++;
	*dst = '\0';
	return (dup);
}

char	*ft_getenv(const char *name, t_myenv *env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (env->env[i])
	{
		if (ft_strncmp(env->env[i], name, len) == 0 && env->env[i][len] == '=')
			return (env->env[i] + len + 1);
		i++;
	}
	return (NULL);
}

char	*expand_variable(const char *str, int *i, t_myenv *env)
{
	char	*name;
	char	*value;
	char	*result;

	name = get_var_name(str, i);
	if (!name)
		return (ft_strdup(""));
	if (strcmp(name, "?") == 0)
	{
		char *code = ft_itoa(env->exit_code);
		free(name);
		return (code);
	}
	value = ft_getenv(name, env);
	free(name);
	if (!value)
		return (ft_strdup(""));
	result = ft_strdup(value);
	return (result);
}

char *str_append(char *s1, const char *s2)
{
	size_t	len1 = s1 ? strlen(s1) : 0;
	size_t	len2 = strlen(s2);
	char	*new = malloc(len1 + len2 + 1);
	if (!new)
	{
		free(s1);
		return NULL;
	}
	if (s1)
		strcpy(new, s1);
	else
		new[0] = '\0';
	strcpy(new + len1, s2);
	free(s1);
	return new;
}

char	*expand_string(const char *input, t_myenv *env)
{
	int		i;
	char	tmp[2];
	char	*result;
	char	*expand;

	i = 0;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	tmp[1] = '\0';
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] && (ft_isalnum(input[i + 1]) || input[i + 1] == '?'))
		{
			i++;
			if (input[0] != '\'')
				expand = expand_variable(input, &i, env);
			else
				return (free(result), ft_strdup(input));
			if (ft_isdigit(input[i]))
				i++;
			if (!expand)
				return (free(result), NULL);
			result = str_append(result, expand);
			if (!result)
				return (NULL);
			free(expand);
		}
		else
		{
			tmp[0] = input[i];
			result = str_append(result, tmp);
			i++;
		}
	}
	return (result);
}

// int	main(void)
// {
// 	t_myenv env;
// 	char *input;
// 	char *expanded;
// 	char *fake_env[] = {
// 		"USER=cdiab",
// 		"HOME=/home/cdiab",
// 		"PATH=/usr/bin:/bin",
// 		"EMPTY=",
// 		NULL
// 	};

// 	// Setup fake environment
// 	env.env = fake_env;
// 	env.exit_code = 42;

// 	// Test cases
// 	char *tests[] = {
// 		"Hello $USER",
// 		"Home is $HOME",
// 		"Exit was $?",
// 		"Nothing: $NOTFOUND",
// 		"Empty: $EMPTY",
// 		"Mixed $USER-$HOME-$PATH",
// 		"$?+$?",
// 		"$? is the last exit code",
// 		"Text with no vars",
// 		"$",
// 		NULL
// 	};

// 	for (int i = 0; tests[i]; i++)
// 	{
// 		input = tests[i];
// 		expanded = expand_string(input, &env);
// 		if (expanded)
// 		{
// 			printf("Input   : %s\n", input);
// 			printf("Expanded: %s\n\n", expanded);
// 			free(expanded);
// 		}
// 		else
// 			printf("Expansion failed for: %s\n", input);
// 	}
// 	return (0);
// }