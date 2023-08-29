#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/udp.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/ip.h>

#define broadcast "192.168.0.255"
#define ADDR "192.168.0.2"
#define SPORT 7777
#define DPORT 8888


int
main(int argc, char* argv[])
{
   
    char* ip = malloc(64);
    int size = 1;
    strcpy(ip, "Hello everyone!\n");
    printf("%s\n", ip);

    int fd = socket(AF_INET, SOCK_DGRAM, 0); 

    if (fd == -1)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serv;
    memset(&serv, '\0', sizeof(struct sockaddr_in));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(DPORT);
    serv.sin_addr.s_addr = inet_addr(broadcast);
    socklen_t addrlen = sizeof(serv);

    if(-1 == setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &size, sizeof(size))){
	perror("socket option ERR");
	exit(EXIT_FAILURE);
    }	
    for(int i = 0; i < 100; i++){
    	if (-1 == sendto(fd, ip, strlen(ip), 0, (struct sockaddr*)&serv, addrlen))
    	{
    	    perror("send ERR");
    	    exit(EXIT_FAILURE);
    	}
    }
    close(fd);

    exit(EXIT_SUCCESS);
}