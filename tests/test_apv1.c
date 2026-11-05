#include "libft.h"
#include <fcntl.h>

#define SIZE 1;











char	*ft_strncpy(cha *dest, char *src, int n)
{
	int		i;

	i = 0;
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*check_line(char *buf, int nbyte)
{
	static char	*s_old;
	char	*s_new;
	int		i;
	int		len;

	s_new = NULL;
	i = 0;
	s_new = ft_memchr(buf, '\n', nbyte);
	if (s_new != NULL)
	{
		len = s_new - buf;
		s_new = malloc(sizeof(char) * ((len + 1) + 1));
		if (!s_new)
			return (NULL);
		ft_strncpy(s_new, buf, len + 1);
		if (s_old != NULL)
			return (ft_strjoin(s_old, s_new), free(s_old), free(s_new));
		return (s_new);
	}
	else
	{
		if (s_old == NULL)
		{
			s_old = malloc(sizeof(char) * ft_strlen(buf));
			ft_strncpy(s_old, buf);
			return (NULL);
		}
		s_new = ft_strjoin(s_old, buf);
		free(s_old);
		s_old = s_new;
		return (NULL);
	}
	if ()
}


char	*get_line(int fd)
{
	static char		*buf;
	char			*s;
	int				nbyte;

	buf = malloc(sizeof(char) * SIZE + 1);
	ft_bzero(buf, SIZE + 1);
	s = NULL;
	nbyte = read(fd, buf, SIZE);
	while (nbyte != -1)
	{
		s = check_line(buf, nbyte);

	}

}

















int		main()
{

}
