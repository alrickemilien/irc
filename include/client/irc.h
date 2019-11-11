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

typedef struct  s_env
{
    t_fd        *fds;
    int         port;
    size_t      maxfd;
    size_t      max;
    int         r;
    fd_set      fd_read;
    fd_set      fd_write;
    char        isotime[ISOTIMESTRSIZE];
    int         is_tty;
    int         sock;
}               t_env;

/*
** fd 
*/

void            clear_fd(t_fd *fd);
void            init_fd(t_env *e);
void            check_fd(t_env *e);

void            client_ipv4(const t_options *options, t_env *e);
void            client_ipv6(const t_options *options, t_env *e);

void            server_read(t_env *e, size_t cs);
void            server_write(t_env *e, size_t cs);

void            do_select(const t_options *options, t_env *e);
#endif
