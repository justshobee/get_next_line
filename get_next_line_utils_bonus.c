/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochoubi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/10 10:40:22 by mochoubi          #+#    #+#             */
/*   Updated: 2026/11/10 10:40:29 by mochoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return (i);
	while (s[i] != '\0')
		i++;
	return (i);
}

void	*ft_memcpy(void *dest, void *src, size_t n)
{
	size_t	i;

	if (src == NULL || dest == NULL)
		return (dest);
	i = 0;
	while (i < n)
	{
		*((char *)dest + i) = *((char *)src + i);
		i++;
	}
	return (dest);
}

char	*ft_strjoin(char **s1, char *s2)
{
	char	*s;
	size_t	s1len;
	size_t	s2len;
	size_t	size;

	if (!*s1 || !s2)
		return (NULL);
	s = NULL;
	s1len = ft_strlen(*s1);
	s2len = ft_strlen(s2);
	size = s1len + s2len;
	s = malloc(sizeof(char) * (size + 1));
	if (!s)
		return (NULL);
	ft_memcpy(s, *s1, s1len);
	ft_memcpy(s + s1len, s2, s2len + 1);
	return (free(*s1), *s1 = NULL, s);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char	*str;
	size_t	i;
	size_t	slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (start > slen)
		start = slen;
	slen = ft_strlen(s + start);
	if (len > slen)
		len = slen;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (i < len && s[start + i])
	{
		str[i] = s[start + i];
		i++;
	}
	return (str[i] = '\0', str);
}

char	*ft_strchr(const char *s, int c)
{
	int		i;

	if (!s)
		return (NULL);
	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)(s + i));
	return (NULL);
}
