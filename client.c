#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ADDR "192.168.0.2"
#define PORT 8888
#define multicast "225.0.1.1"

int main(int argc, char** argv) {

    char* buffer = malloc(64);
    socklen_t addr_size;

    struct sockaddr_in client, sender;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == fd) {
        perror("socket ERR");
        exit(EXIT_FAILURE);
    }

    memset(&sender, '\0', sizeof(struct sockaddr_in));
    memset(&client, '\0', sizeof(struct sockaddr_in));
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    client.sin_addr.s_addr = INADDR_ANY;

    if (-1 == bind(fd, (struct sockaddr*)&client, sizeof(struct sockaddr_in))) {
        perror("bind ERR");
        exit(EXIT_FAILURE);
    }

    struct in_addr imr_multiaddr;
    struct ip_mreqn op;
    op.imr_address.s_addr = INADDR_ANY; // local address
    op.imr_multiaddr.s_addr = inet_addr(multicast);
    op.imr_ifindex = if_nametoindex("enp0s8");

    memset(&imr_multiaddr, '\0', sizeof(imr_multiaddr));

    if(-1 == inet_pton(AF_INET, multicast, &imr_multiaddr)){
	perror("pton ERR");
	exit(EXIT_FAILURE);
    }

    if( -1 == setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &op, sizeof(op))){
	perror("membership ERR");
	exit(EXIT_FAILURE);
    }

    bzero(buffer, 64);
    addr_size = sizeof(client);
    recvfrom(fd, buffer, 64, 0, (struct sockaddr*)&sender, &addr_size);
    printf("[+]Data recv: %s\n", buffer);

    close(fd);

    exit(EXIT_SUCCESS);
}