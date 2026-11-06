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

//ft_strlen
//ft_strjoin
//ft_strdup
//ft_substr
//ft_strchr

char	*line(char *buf, int nbyte, char *find, char **stash)
{
	char	*newstr;
	int		index;

	index = find - buf;
	index++;
	newstr = ft_substr(buf, 0, index + 1);
	if (ft_strlen(*stash) > 0)
		newstr = ft_strjoin(stash, newstr);
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
	char	*s;
	char	*find;
	int		len;
	int		index;

	s = NULL;
	len = 0;
	if (nbyte == 0)
	{
		if (!(*stash))
			return ft_strdup(buf);
		else
			return (ft_strjoin(stash, buf));
	}
	find = ft_strchr(buf, '\n');
	if (find != NULL)
		return (line(buf, nbyte, find, stash));
		// check stash if not empty join with the new string and return otherwise return newstr
	else
	{
		if (!(*stash))
			*stash = ft_strdup(buf);
		else
			*stash = ft_strjoin(stash, buf);
		return (NULL);
	}
		// check stash if not empty jjoinstr with newstr otherwise fill stash with newstr

}
char	*get_next_line(int fd)
{
	static char	*stash;
	int		nbyte;
	char	*buf;
	char	*s;

	if (fd <= 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (buf == NULL)
		return (NULL);
	nbyte = read(fd, buf, BUFFER_SIZE);
	if (nbyte < 0)
		return (NULL);
	s = get_line(buf, nbyte, &stash);
	while (s == NULL)
	{
		nbyte = read(fd, buf, BUFFER_SIZE);
		if (nbyte < 0)
			return (NULL);
		s = get_line(buf, nbyte, &stash);
	}
	//get line
	return (s);
}

int		main()
{
	int fd = open("test.txt", O_RDONLY);
	
	char *s;
	while (s = get_next_line(fd))
	{
		printf("%s", s);
		free(s);
	}
	close(fd);
	return (0);
}
