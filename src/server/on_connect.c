
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include "server/irc.h"

const char *HELLO = "Welcome to the irc server !\n";

void on_connect(t_env *e, size_t s)
{
    int                cs;
    struct sockaddr_in csin;
    socklen_t          csin_len;

    csin_len = sizeof(csin);
    cs = XSAFE(-1, accept(s, (struct sockaddr *)&csin, &csin_len), "accept");

    printf(e->is_tty ? "\x1b[31m"
                       "New client #%d from %s:%d"
                       "\x1b[0m\n"
                     : "New client #%d from %s:%d\n",
           cs, inet_ntoa(csin.sin_addr), ntohs(csin.sin_port));

    clear_fd(&e->fds[cs]);
    e->fds[cs].type = FD_CLIENT;
    e->fds[cs].read = client_read;
    e->fds[cs].write = client_write;
    memcpy(e->fds[cs].channel, DEFAULT_CHANNEL, sizeof(DEFAULT_CHANNEL));
    memcpy(e->fds[cs].nickname, DEFAULT_NICKNAME, sizeof(DEFAULT_NICKNAME));

    memcpy(e->fds[cs].buf_write, HELLO, strlen(HELLO));
}
