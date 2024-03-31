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
	pid_t pid;
	
	if ((pid = fork()) < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (pid == 0)
	{
		int blacklist[] = {
			SCMP_SYS(open),
			SCMP_SYS(openat),
		};
		
		scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
		if (ctx == NULL)
		{
			perror("seccomp_init");
			exit(EXIT_FAILURE);
		}
		for (unsigned i = 0; i < sizeof(blacklist) / sizeof(blacklist[0]); i++) {
			if (seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), blacklist[i], 0))
			{
				perror("seccomp_rule_add");
				exit(EXIT_FAILURE);
			}
		}
		if (seccomp_load(ctx))
		{
			perror("seccomp_load");
			exit(EXIT_FAILURE);
		}
		seccomp_release(ctx);

		execve(argv[1], argv + 1, NULL);
		perror("execve");
		exit(EXIT_FAILURE);
	}

	int status;
	if (waitpid(pid, &status, 0) < 0)
	{
		perror("waitpid");
		exit(EXIT_FAILURE);
	}
	return status;
}