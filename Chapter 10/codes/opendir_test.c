#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char **argv)
{
    int fd = open(argv[1], O_RDONLY | __O_DIRECTORY);
    if (fd == -1)
    {
        printf("Error %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("dir %s fd %d\n", argv[1], fd);

    char buf[1024];
    int nread = read(fd, buf, 1024);
    if (nread == -1)
    {
        printf("Error %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("read %d bytes\n", nread);
    // print buffer as hexademical string
    for (int i = 0; i < nread; i += 8)
    {
        for (int j = 0; j < 8; j++)
        {
            printf("%02x ", buf[i + j]);
        }
        fflush(stdout);
        for (int j=0; j<8; j++)
        {
            if (buf[i+j] >= 32 && buf[i+j] <= 126)
                printf("%c", buf[i+j]);
            else
                printf(".");
        }
        printf("\n");
    }

    close(fd);
    return 0;
}