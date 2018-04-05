
#ifndef SOCKET_PROGRAMMING_PCOMMON_H
#define SOCKET_PROGRAMMING_PCOMMON_H

#include<stdio.h>
#include<stdlib.h>
#include<netdb.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT 9011

void printError(char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void printAddresses(struct addrinfo *infos) {
    char ipstr[INET6_ADDRSTRLEN];

    for(struct addrinfo* p = infos; p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;

        if(p->ai_family == AF_INET) { //IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { //IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6*)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        //convert IP to string and print
        inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
        printf("  %s: %s\n", ipver, ipstr);
    }
}


#endif //SOCKET_PROGRAMMING_PCOMMON_H
