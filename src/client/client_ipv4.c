#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client/irc.h"

int client_ipv4()
{
    int                sock;
    struct sockaddr_in sin;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    sin.sin_family = AF_INET;
    sin.sin_port = htons(5555);
    inet_pton(AF_INET, "127.0.0.1", &sin.sin_addr);
    connect(sock, (struct sockaddr *)&sin, sizeof(sin));

    return (sock);
}
