#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/udp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/ip.h>

#define multicast "239.0.0.10"
#define ADDR "192.168.0.1"
#define SPORT 7777
#define DPORT 8888

int main(int argc, char** argv) {

    int num = 0;
    char buf[128] = "Hello clients";

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1)
    {
        perror("socket ERR");
        exit(EXIT_FAILURE);
    }

    struct in_addr imr_multiaddr;
    imr_multiaddr.s_addr = inet_addr(multicast);
    imr_multiaddr.
        setsockopt(fd, IPPROTO_IP, IP_MULTICAST_IF, &imr_multiaddr, sizeof(imr_multiaddr));

    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(8888);
    cliaddr.sin_addr.s_addr = inet_addr(multicast);

    for (int i = 0; i < 100; i++) {
        sendto(fd, buf, strlen(buf) + 1, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        printf("Multicast data: %s\n", buf);
        sleep(1);
    }
    close(fd);

    exit(EXIT_SUCCESS);

}