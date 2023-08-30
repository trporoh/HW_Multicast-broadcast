#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/udp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/ip.h>

#define multicast "225.0.1.1"
#define ADDR "192.168.0.1"
#define SPORT 7777
#define DPORT 8889

int main(int argc, char** argv) {

    int ttl = 1;
    char buf[128] = "Hello clients";

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
    {
        perror("socket ERR");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(DPORT);
    cliaddr.sin_addr.s_addr = inet_addr(multicast);

    sendto(fd, buf, strlen(buf), 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
    printf("Multicast data: %s\n", buf);
    close(fd);

    exit(EXIT_SUCCESS);

}