#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ADDR "192.168.0.3"
#define PORT 8888

int main(int argc, char** argv) {
    int size = 1;

    char* buffer = malloc(64);
    socklen_t addr_size;

    struct sockaddr_in client, receiver;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (-1 == fd) {
        perror("socket ERR");
        exit(EXIT_FAILURE);
    }

    memset(&client, '\0', sizeof(struct sockaddr_in));
    memset(&receiver, '\0', sizeof(struct sockaddr_in));
    client.sin_family = AF_INET;
    client.sin_port = htons(PORT);
    client.sin_addr.s_addr = INADDR_ANY;
    //setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &size, sizeof(size));

    if (-1 == bind(fd, (struct sockaddr*)&client, sizeof(struct sockaddr_in))) {
        perror("bind ERR");
        exit(EXIT_FAILURE);
    }

    bzero(buffer, 64);
    addr_size = sizeof(client);
    recvfrom(fd, buffer, 64, 0, (struct sockaddr*)&receiver, &addr_size);
    printf("[+]Data recv: %s\n", buffer);

    close(fd);

    exit(EXIT_SUCCESS);
}