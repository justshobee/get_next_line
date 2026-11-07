/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochoubi <mochoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/05 11:55:12 by mochoubi          #+#    #+#             */
/*   Updated: 2026/11/05 12:18:35 by mochoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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

char	*line(char *buf, int nbyte, char *find, char **stash)
{
	char	*newstr;
	char	*free_s;
	int		index;

	index = find - buf;
	index++;
	newstr = ft_substr(buf, 0, index);
	if (ft_strlen(*stash) > 0)
	{
		free_s = newstr;
		newstr = ft_strjoin(stash, newstr);
		free(free_s);
	}
	if (ft_strlen(buf + index) > 0)
	{
		if (!(*stash))
			*stash = ft_strdup(buf + index);
		else
			*stash = ft_strjoin(stash, buf + index);
	}
	return (newstr);
}

char	*get_line(char *buf, int nbyte, char **stash)
{
	char	*find;

	if (nbyte == 0)
	{
		if (!(*stash))
			return (ft_strdup(buf));
		else
			return (ft_strjoin(stash, buf));
	}
	find = ft_strchr(buf, '\n');
	if (find != NULL)
		return (line(buf, nbyte, find, stash));
	else
	{
		if (!(*stash))
			*stash = ft_strdup(buf);
		else
			*stash = ft_strjoin(stash, buf);
		return (NULL);
	}
}

char	*get_next_line(int fd)
{
	static char	*stash;
	int			nbyte;
	char		buf[BUFFER_SIZE + 1];
	char		*s;

	if (fd <= 0 || BUFFER_SIZE <= 0)
		return (NULL);
	nbyte = read(fd, buf, BUFFER_SIZE);
	buf[nbyte] = '\0';
	if (nbyte < 0 || (nbyte == 0 && !stash))
		return (NULL);
	s = get_line(buf, nbyte, &stash);
	while (s == NULL)
	{
		nbyte = read(fd, buf, BUFFER_SIZE);
		buf[nbyte] = '\0';
		if (nbyte < 0 || (nbyte == 0 && !stash))
			return (NULL);
		s = get_line(buf, nbyte, &stash);
	}
	return (s);
}

int		main()
{
	int fd = open("test.txt", O_RDONLY);
	if (fd == -1)
		printf("failing to open this file ==> [fd = %d]\n", fd);
	char *s = NULL;
	while ((s = get_next_line(fd)))
	{
		printf("%s", s);
		free(s);
	}
	close(fd);
	return (0);
}
