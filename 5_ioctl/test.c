#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "ioctl_header.h"

int main(int argc, char **argv)
{
    int answer;
    int fd = open("/dev/chrdev", O_WRONLY);
    if (fd < 0) {
        perror("open");
        return fd;
    }
    ioctl(fd, RD_VALUE, &answer);
    printf("Answer read from driver: %d\n",answer);

    answer = 236;
    ioctl(fd, WR_VALUE, &answer);
    ioctl(fd, RD_VALUE, &answer);
    printf("Now answer is : %d\n", answer);

    struct my_struct test = {4, "Mohan"};
    ioctl(fd, GREETER, &test);
    printf("opening was success\n");
    close(fd);
    return 0;
}
