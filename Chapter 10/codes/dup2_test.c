#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int fd = open(argv[1], O_WRONLY | O_SYNC);
    if (fd == -1)
    {
        perror("open");
        return 1;
    }
    printf("file %s fd %d\n", argv[1], fd);
    int fd2 = open(argv[1], O_WRONLY | O_SYNC);
    if (fd2 == -1)
    {
        perror("open");
        return 1;
    }
    printf("file %s fd %d\n", argv[1], fd2);

    int fd1_new = dup2(fd, 1);
    if (fd1_new == -1)
    {
        perror("dup2");
        return 1;
    }

    printf("[fd1]: 1111111111111111111111111\n");
    dprintf(fd, "[fd%d]: 22222\n", fd);
    dprintf(fd2, "--[fd%d]overwrite--", fd2);
    close(fd);
    return 0;
}