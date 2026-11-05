/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mochoubi <mochoubi@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/03 14:07:16 by mochoubi          #+#    #+#             */
/*   Updated: 2026/11/04 11:39:11 by mochoubi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

# define SIZE 42
//1 - file
//2 - read from this file and store in buffer[1024]
//3 - procces this buffer until find \n

int		ft_strlen(char *buf, int nbyte)
{
	int		i;

	i = 0;
	while (i < nbyte && buf[i])
	{
		if (buf[i] == '\n')
			return (i + 1);
		i++;
	}
	return (0);
}

char	*ft_strncpy(char *dest, char *src, int n)
{
	int		i;

	i = 0;
	while (i < n && src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*procc_buf(char *buf, int nbyte)
{
	char	*s;
	int		len;

	s = NULL;
	len = ft_strlen(buf, nbyte);
	if (len != 0)
	{
		s = malloc(sizeof(char) * (len + 1));
		return (ft_strncpy(s, buf, len));
	}
	return (NULL);
}

char	*gnl(int fd)
{
	static char buf[42];
	int		nbyte;

	//buf = malloc(sizeof(char) * SIZE)
	nbyte = 0;
	nbyte = read(fd, buf, SIZE);
	return (procc_buf(buf, nbyte));
}

int		main()
{
	int fd = open("test.txt", O_RDONLY);
	if (fd == -1)
		return 0;
	int i = 0;
	char *s = gnl(fd);
	printf("s = %s", s);
	free(s);
	close(fd);

}

