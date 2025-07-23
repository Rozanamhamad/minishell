/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-moha <ral-moha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 16:07:10 by cdiab             #+#    #+#             */
/*   Updated: 2025/07/15 16:30:13 by ral-moha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <unistd.h>
# include <string.h>
# include <stdlib.h>

int			ft_isdigit(int arg);
int			ft_isalpha(int arg);
int			ft_isalnum(int arg);
int			ft_isascii(int arg);
int			ft_isprint(int arg);

int			ft_toupper(int ch);
int			ft_tolower(int ch);

size_t		ft_strlen(const char *str);

void		*ft_memset(void *dest, int x, size_t n);
void		ft_bzero(void *dest, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_memmove(void *dest, const void *src, size_t n);
void		*ft_memchr(const void *str, int c, size_t n);
int			ft_memcmp(const void *str1, const void *str2, size_t n);
void		*ft_calloc(size_t num, size_t size);

size_t		ft_strlcat(char *dest, const char *src, size_t size);
size_t		ft_strlcpy(char *dest, const char *src, size_t n);
int			ft_strncmp(const char *str1, const char *str2, size_t n);
char		*ft_strdup(const char *src);
char		*ft_strnstr(const char *str, const char *target, size_t len);
char		*ft_strchr(const char *str, int c);
char		*ft_strrchr(const char *str, int c);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char const *s1, char const*s2);
char		*ft_strtrim(char const *s, char const *set);
char		**ft_split(char const *s, char c);
void		ft_striteri(char *s, void (*f)(unsigned int, char *));
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));

void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(const char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);

int			ft_atoi(const char *str);
char		*ft_itoa(int n);

#endif
