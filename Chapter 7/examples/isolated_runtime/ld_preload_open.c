// gcc ld_preload_open.c -o ld_preload_open.o -shared -fPIC
// LD_PRELOAD=./ld_preload_open.so ./a.out

#include <stdio.h>
#include <errno.h>

int open(const char *pathname, int flags)
{
	errno = EPERM;
	printf("open(%s, %d) blocked\n", pathname, flags);
	return -1;
}
