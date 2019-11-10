#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client/irc.h"

int client_ipv6()
{
    int                 sock;
    struct sockaddr_in6 sin;

    sock = socket(AF_INET6, SOCK_STREAM, 0);
    sin.sin6_family = AF_INET6;
    sin.sin6_port = htons(5556);
    inet_pton(AF_INET6, "::1", &sin.sin6_addr);
    connect(sock, (struct sockaddr *)&sin, sizeof(sin));

    return (sock);
}
