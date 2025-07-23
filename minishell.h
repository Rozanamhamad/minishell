/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 11:19:10 by cdiab             #+#    #+#             */
/*   Updated: 2025/07/21 17:53:29 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft/libft.h"
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <stddef.h>
# include "libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <termios.h>
# include <termcap.h>     
# include <errno.h>
# include <limits.h>

typedef struct s_tokennode {
	char				*token;
	int					type;
	struct s_tokennode	*next;
	struct s_tokennode	*prev;
}	t_tokennode;

typedef struct s_tokenlist {
	t_tokennode	*head;
	t_tokennode	*tail;
}	t_tokenlist;

typedef struct s_ws {
	int	i;
	int	j;
	int	sq;
	int	dq;
	int	space;
}	t_ws;

typedef enum s_type
{
	T_PIPE,
	T_SCMD,
	T_REDIR_IN,    // <
	T_REDIR_OUT,   // >
	T_APPEND,      // >>
	T_HEREDOC      // <<
} t_type;

typedef struct s_myenv
{
	char	**env;
	int		exit_code;
}	t_myenv;

typedef struct s_ast_node {
	t_type				type;
	char				**arr;
	char				*in;
	char				*out;
	int					app;
	int					ex_heredoc;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

void		handle_pipe(int *i, t_tokenlist *token_list);
void		handle_redirection(int *i, t_tokenlist *token_list,
				const char *input);
void		handle_envariable(int *i, t_tokenlist *tokenlist,
				const char *input);
t_tokennode	*create_tokennode(const char *token, t_type type);
t_tokenlist	*create_tokenlist(void);
void		addtoken(t_tokenlist *list, const char *token, t_type type);
void		printTokens(const t_tokenlist *list);
void		free_tokenlist(t_tokenlist *list);
char		*get_token(const char *input, int *i);
t_tokenlist	*tokenizeInput(const char *input); //parser is treating export= as a standalone command not export with an argument
t_ast_node	*build_ast(t_tokenlist *tokens);
t_ast_node	*create_node(char **arr);
void		print_ast(t_ast_node *node, int depth);
void		in_out(t_tokennode *token, t_ast_node *node);
void		app_heredoc(t_tokennode *token, t_ast_node *node);
int			find_pipe_index(t_tokenlist *list);
t_tokenlist	*copy_range(t_tokenlist *original, int start, int end);
void		split_by_pipe(t_tokenlist *original, t_tokenlist **left,
				t_tokenlist **right);
int		list_size(t_tokenlist *lst);
char	**copy_env(char **env);
int		env_size(char **env);
void	free_env(char **env);

void	test(t_ast_node *node);
void	print_double_pointer(char **strs);
char	*process_quotes(const char *input);
char	*ft_strndup(const char *start, size_t length);
char	*expand_string(const char *input, t_myenv *env);
char	*str_append(char *s1, const char *s2);
char	*expand_variable(const char *str, int *i, t_myenv *env);
char	*ft_getenv(const char *name, t_myenv *env);
char	*get_var_name(const char *str, int *i);
int		get_var_name_len(const char *str);
int		is_var_char(char c, int first);
char	*clean_quotations(const char *input);
void	execute_ast(t_ast_node	*root, t_myenv *myenv);

//builtsin
int	is_builtin(char *cmd);
int	exec_builtin(t_ast_node *node, t_myenv *env);
// int	handle_builtins(t_ast_node *node, t_myenv *env);

//exit
int	exec_exit(t_ast_node *node, t_myenv *env);
//pwd 
int	exec_pwd(t_ast_node *node, t_myenv *env);
//echo
int	exec_echo(t_ast_node *node, t_myenv *env);   
//env
int	exec_env(t_ast_node *node, t_myenv *env);
//unset
int	exec_unset(t_ast_node *node, t_myenv *env);
//cd
int		exec_cd(t_ast_node *n, t_myenv *e);
//export //env
int	exec_export(t_ast_node *node, t_myenv *env);
int	handle_export_argument(const char *arg, t_myenv *env);
int	is_valid_export_key(const char *str);
char	**ft_realloc_double(char **env, char *new_entry);
int		get_env_key_and_path(int opt, t_myenv *e, const char **key, char **path);
void	save_oldpwd(t_myenv *e);
void	cd_perr(const char *msg, const char *path, t_myenv *e);
char	**realloc_env(char **env, char *new_entry);
void	remove_env_var(const char *key, t_myenv *e);
int		go_env(int opt, t_myenv *e);
void set_env_var(const char *key, const char *value, t_myenv *e);
int	exec_export(t_ast_node *node, t_myenv *env);

//execution
void execute_ast(t_ast_node *node, t_myenv *env);
// Executes a simple command with redirections and forking
void execute_simple_cmd(t_ast_node *node, t_myenv *env);

// // Called inside the child process to exec the command
void exec_child_process(t_ast_node *node, t_myenv *env, int fd_in, int fd_out);
//execution
void execute_simple_cmd(t_ast_node *node, t_myenv *env);
int setup_redirections(t_ast_node *node, int *fd_in, int *fd_out, t_myenv *env);
void exec_child_process(t_ast_node *node, t_myenv *env, int fd_in, int fd_out);
void execute_pipe(t_ast_node *node, t_myenv *env);
void	handle_heredoc(t_ast_node *node);
char *find_path(const char *cmd, t_myenv *env);
void free_split(char **arr);
char *get_env_value(t_myenv *env, const char *key);
void handle_redirection(int *i, t_tokenlist *token_list, const char *input);
int handle_builtins(t_ast_node *node, t_myenv *env);

#endif