#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "client/irc.h"

void ping(int s, char *message)
{
    char buf[8192];

    printf("sending: %s\n", message);

    strncpy(buf, message, sizeof(buf));
    send(s, buf, strlen(buf), 0);
    recv(s, buf, 8192, 0);
    strtok(buf, "\n");
    puts(buf);
}

int main(int argc, char **argv)
{
    (void)argv;
    (void)argc;

    client_ipv4();
    return 0;
}
