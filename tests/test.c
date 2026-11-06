#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
void	print_line(char *buf)
{
		int i = 0;
		while (buf[i] != '\n' && buf[i])
			printf("%c", buf[i++]);
		printf("\n");
}
int		main()
{
	int fd = open("test.txt", O_RDONLY);
	char buf[1024] = {0};
	int	nbyte = 0;

	while (nbyte = read(fd, buf, 10 ))
	{
		buf[10] = '\0';
		print_line(buf);
	}
	printf("\n\n%s\n", buf);
}
