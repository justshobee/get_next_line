#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int		main()
{
	int fd = open("test.txt", O_RDONLY);
	char buf[1024] = {0};

	while (read(fd, buf, 50))
	{
		int i = 0;
		while (buf[i] != '\n')
			printf("%c", buf[i++]);
		break ;
	}
	printf("\n\n%s\n", buf);
}
