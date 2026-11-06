/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochoubi <mochoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 11:56:39 by mochoubi          #+#    #+#             */
/*   Updated: 2026/11/05 12:17:59 by mochoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef get_net_line_h
# define get_next_line_h

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif



#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdint.h>
size_t	ft_strlen(char *s);
void	*ft_memcpy(void *dest, void *src, size_t n);
char	*ft_strjoin(char **s1, char *s2);
char	*ft_strdup(char *s);
char	*ft_strchr(const char *s, int c);
char	*ft_substr(char *s, unsigned int start, size_t len);
void	*ft_memset(void *s, int c, size_t n);
void	*ft_calloc(size_t n, size_t size);

#endif
