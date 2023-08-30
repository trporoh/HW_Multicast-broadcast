#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ADDR "192.168.1.5"
#define PORT 2244
#define multicast "239.0.0.10"

int main(int argc, char** argv) {

    char* buffer = malloc(64);
    socklen_t addr_size;

    struct sockaddr_in client;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == fd) {
        perror("socket ERR");
        exit(EXIT_FAILURE);
    }

    memset(&client, '\0', sizeof(struct sockaddr_in));
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    client.sin_addr.s_addr = inet_addr(ADDR);

    if (-1 == bind(fd, (struct sockaddr*)&client, sizeof(struct sockaddr_in))) {
        perror("bind ERR");
        exit(EXIT_FAILURE);
    }

    struct ip_mreqn op;
    op.imr_address.s_addr = INADDR_ANY; // local address
    op.imr_multiaddr.s_addr = inet_addr(multicast);
    op.imr_ifindex = 3;

    setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &op, sizeof(op));


    bzero(buffer, 64);
    addr_size = sizeof(client);
    recvfrom(fd, buffer, 64, 0, (struct sockaddr*)&client, &addr_size);
    printf("[+]Data recv: %s\n", buffer);

    close(fd);

    exit(EXIT_SUCCESS);
}