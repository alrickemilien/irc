#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "server/irc.h"

const char *HELLO = "Welcome to the irc server !\n";

// struct ucred
// {
//     pid_t pid; /* process ID of the sending process */
//     uid_t uid; /* user ID of the sending process */
//     gid_t gid; /* group ID of the sending process */
// };

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

    e->fds[cs].type = FD_CLIENT;
    e->fds[cs].read = client_read;
    e->fds[cs].write = client_write;
    memcpy(e->fds[cs].channel, DEFAULT_CHANNEL, sizeof(DEFAULT_CHANNEL));
    memcpy(e->fds[cs].nickname, DEFAULT_NICKNAME, sizeof(DEFAULT_NICKNAME));
    memset(e->fds[cs].username, 0, sizeof(USERNAMESTRSIZE + 1));

    XSAFE(-1, gethostname(e->fds[cs].hostname, sizeof(e->fds[cs].hostname)),
          "on_connect::gethostname");

    // memcpy(e->fds[cs].buf_write, HELLO, strlen(HELLO) * sizeof(char));

    // Say hello to new user
    // cbuffer_pflush(e->fds[cs].buf_write, HELLO, strlen(HELLO) * sizeof(char));
}
