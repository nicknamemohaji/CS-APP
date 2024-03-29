// This program tries to open file, which can cause a harm to the system.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	printf("opening....\n");
	int fd = open("test.txt", O_RDONLY);
	if (fd < 0)
	{
		perror("open");
		return 1;
	}
	printf("opened\n");
	
	char buf[100];
	while (read(fd, buf, sizeof(buf)) > 0)
	{
		printf("reading...\n");
		printf("%s\n", buf);
	}

	return 0;
}
