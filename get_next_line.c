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

char	*line(char *buf, char *find, char **stash)
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

char	*check_stash(char **stash)
{
	char	*find;
	char	*newstr;
	int	index;

	if (!(*stash))
		return (NULL);
	find = ft_strchr(*stash, '\n');
	if (find != NULL)
	{
		index = find - (*stash);
		newstr = ft_substr(*stash, 0, index + 1);
		if (find[index + 1] == '\0')
		{
			free(*stash);
			return (*stash = NULL, newstr);
		}
		find = *stash;
		*stash = ft_strdup(find + index + 1);
		return (free(find), newstr);
	}
	return (NULL);
}

char	*get_line(char *buf, int nbyte, char **stash)
{
	char	*find;
	char	*line_stash;

	line_stash = check_stash(stash);
	if (nbyte == 0 && line_stash == NULL)
	{
		if (!(*stash))
			return (NULL);
		else
			return (ft_strjoin(stash, buf));
	}
	find = ft_strchr(buf, '\n');
	if (find != NULL && line_stash == NULL)
		return (line(buf, find, stash));
	else
	{
		if (!(*stash) && nbyte > 0)
			*stash = ft_strdup(buf);
		else
			*stash = ft_strjoin(stash, buf);
		if (line_stash != NULL)
			return (line_stash);
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

// int		main()
// {
// 	int fd1 = open("test.txt", O_RDONLY);
// 	if (fd1 == -1)
// 		printf("failing to open this file ==> [fd = %d]\n", fd1);
// 	char *s = NULL;
// 	s = get_next_line(fd1);
// 	printf("[%s]",s);
// 	free(s);
// 	s = get_next_line(fd1);
// 	printf("[%s]",s);
// 	free(s);
// 	s = get_next_line(fd1);
// 	printf("[%s]",s);
// 	free(s);
// 	s = get_next_line(fd1);
// 	if (s == NULL)
// 		printf("NULL\n");
// 	else
// 	{
// 		printf("[%s]",s);
// 		free(s);
// 	}
// 	close(fd1);
// 	return (0);
// }
