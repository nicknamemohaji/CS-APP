// gcc seccomp.c -o seccomp.o -lseccomp

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <seccomp.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

int main(int argc, char* argv[])
{
	(void) argc;
	pid_t pid = fork();
	if (pid == 0)
	{
		printf("[c] loading seccomp\n");
		int blacklist[] = {
			SCMP_SYS(open),
			SCMP_SYS(openat),
		};
		scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
		if (ctx == NULL)
			exit(EXIT_FAILURE);
		for (unsigned i = 0; i < sizeof(blacklist) / sizeof(blacklist[0]); i++) {
			if (seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), blacklist[i], 0))
				exit(EXIT_FAILURE);
		}
		if (seccomp_load(ctx))
			exit(EXIT_FAILURE);
		seccomp_release(ctx);
		printf("[c] seccomp loaded\n");
		if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0))
			exit(EXIT_FAILURE);
		printf("[c] prctl loaded\n");

		printf("[c] child %d opening %s------\n", getpid(), argv[1]);
		execve(argv[1], argv + 1, NULL);
		perror("[c] execve failed");
		exit(EXIT_FAILURE);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		printf("-----child %d exited %d\n", pid, status);
	}

	return 0;
}