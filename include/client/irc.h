#ifndef CLIENT_IRC_H
#define CLIENT_IRC_H

#include <stdlib.h>
#include <sys/select.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <irc.h>
#include <client/options.h>

#define FD_FREE 0
#define FD_SERV 1
#define FD_CLIENT 2

typedef struct  s_env
{
    t_fd        fd;
    int         port;
    int         r;
    fd_set      fd_read;
    fd_set      fd_write;
    char        isotime[ISOTIMESTRSIZE];
    int         is_tty;
    int         sock;
}               t_env;

void             client_ipv4(const t_options *options, t_env *e);
void             client_ipv6(const t_options *options, t_env *e);

#endif
