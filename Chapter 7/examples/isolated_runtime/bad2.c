// This program tries to open file, which can cause a harm to the system.
// as open(2) call is blocked by LD_PRELOAD, it tries to call 
// system call directly by using SYSCALL instruction.

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(void)
{
	int fd = open("test.txt", O_RDONLY);	// this will fail because of LD_PRELOAD
	if (fd < 0)
		perror("open");

	printf("-----------\n");
	printf("now trying to open using syscall..\n");
	__asm__ __volatile__(
		"movq $2, %%rax;\n"		// OPEN
		"movq %1, %%rdi;\n"		// const char *pathname
		"movq %2, %%rsi;\n"		// int flags
		"syscall;\n"
		"movl %%eax, %0;\n"
		: "=r" (fd)
		: "r" ( "test.txt"), "r" ((long long) O_RDONLY)
		: "rdi", "rsi", "rdx", "rax"
	);
	if (fd < -0)
		errno = -fd;

	if (fd < 0)
	{
		perror("open");
		return 1;
	}
	perror("open");
	printf("opened %d\n", fd);
	
	char buf[100];
	while (read(fd, buf, sizeof(buf)) > 0)
	{
		printf("reading...\n");
		printf("%s\n", buf);
	}

	return 0;
}
