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
			*stash = ft_substr(buf + index, 0, ft_strlen(buf + index));
		else
			*stash = ft_strjoin(stash, buf + index);
	}
	return (newstr);
}

char	*check_stash(char **stash)
{
	char	*find;
	char	*newstr;
	int		index;

	if (!(*stash))
		return (NULL);
	find = ft_strchr(*stash, '\n');
	if (find != NULL)
	{
		index = find - (*stash);
		newstr = ft_substr(*stash, 0, index + 1);
		if (find[1] == '\0')
		{
			free(*stash);
			return (*stash = NULL, newstr);
		}
		find = *stash;
		*stash = ft_substr(find + index + 1, 0, ft_strlen(find + index + 1));
		return (free(find), newstr);
	}
	return (NULL);
}

char	*get_line(char *buf, int nbyte, char **stash)
{
	char	*find;

	if (nbyte == 0)
	{
		if (!(*stash))
			return (NULL);
		else
			return (ft_strjoin(stash, buf));
	}
	find = ft_strchr(buf, '\n');
	if (find != NULL)
		return (line(buf, find, stash));
	else
	{
		if (!(*stash) && nbyte > 0)
			*stash = ft_substr(buf, 0, ft_strlen(buf));
		else
			*stash = ft_strjoin(stash, buf);
		return (NULL);
	}
}

char	*get_next_line(int fd)
{
	static char	*stash;
	int			nbyte;
	char		*buf;
	char		*s;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	s = check_stash(&stash);
	if (s != NULL)
		return (s);
	buf = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
		return (NULL);
	while (s == NULL)
	{
		nbyte = read(fd, buf, BUFFER_SIZE);
		if (nbyte < 0 || (nbyte == 0 && !stash))
			return (free(buf), buf = NULL, free(stash), stash = NULL, NULL);
		buf[nbyte] = '\0';
		s = get_line(buf, nbyte, &stash);
	}
	return (free(buf), buf = NULL, s);
}
// int	main(int argc, char **argv)
// {
// 	int		fd;
// 	int		n;
// 	char	*l;
// 	char	*p;

// 	if (argc < 2)
// 		return (1);
// 	fd = open(argv[1], O_RDONLY);
// 	n = 0;
// 	while ((l = get_next_line(fd)))
// 	{
// 		printf("%d: ", ++n);
// 		for (p = l; *p; p++)
// 			putchar(*p == '\n' ? '$' : *p);
// 		putchar('\n');
// 		free(l);
// 	}
// 	return (0);
// }
