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

#include "libft.h"
#include "get_next_line.h"


char	*get_next_line(int fd)
{
	int		nbyte;
	char	*buf;

	if (fd <= 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buf = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (buf == NULL)
		return (NULL);
	nbyte = read(fd, buf, BUFFER_SIZE);
	if (nbyte <= 0)
		return (NULL);
	return (buf);
}

int		main()
{
	int fd = open("test.txt", O_RDONLY);
	char *s = get_next_line(fd);
	printf("[%s]", s);
	free(s);
	close(fd);
	return (0);
}
