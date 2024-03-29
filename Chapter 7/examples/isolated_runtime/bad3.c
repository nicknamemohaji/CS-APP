// This program tries to open file, which can cause a harm to the system.
// But seccomp filters are applied to block open(2) system call.

// gcc bad3.c -lseccomp

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <seccomp.h>

int main(void)
{
	/*
	[seccomp setting]
	in strict mode, only read, write, exit are allowed.
	but it will also block malloc(3), as it internally uses mmap(2) and other calls.

	so we need to use blacklist rules.
	following filter blocks use of open(2).
	*/

    int blacklist[] = {
        SCMP_SYS(open),
		SCMP_SYS(openat),
    };

	scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
    if (ctx == NULL)
        exit(-1);
    for (unsigned i = 0; i < sizeof(blacklist) / sizeof(blacklist[0]); i++) {
        if (seccomp_rule_add(ctx, SCMP_ACT_KILL, blacklist[i], 0))
            exit(-1);
    }
    if (seccomp_load(ctx))
        exit(-1);
	seccomp_release(ctx);
	// done...

	printf("opening....\n");
	int fd = -1;
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
