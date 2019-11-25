#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <server/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

#include <server/ssl.h>


int on_connect(t_env *e, size_t s)
{
    int                cs;
    struct sockaddr_in csin;
    socklen_t          csin_len;

    csin_len = sizeof(csin);
    if ((cs = accept(s, (struct sockaddr *)&csin, &csin_len)) == -1)
        return (logerrno("on_connect::accept"));

    loginfo("New client #%d from %s:%d\n", cs, inet_ntoa(csin.sin_addr),
            ntohs(csin.sin_port));

    logdebug("e->fds[cs].host: %s\n", e->fds[cs].host);

    // if (getnameinfo((struct sockaddr *)&csin, csin_len, e->fds[cs].host,
    //                 NI_MAXHOST, e->fds[cs].serv, NI_MAXSERV, NI_NAMEREQD) < 0)
    // {
    //     logerrno("on_connect::getnameinfo");
    //     return;
    // }

    // Clear file descriptor
    memset(&e->fds[cs], 0, sizeof(t_fd));

    if (e->ssl_ctx && ssl_on_connect(e->ssl_ctx, &e->fds[cs], cs) < 0)
        return (-1);

    // init the new client
    e->fds[cs].type = FD_CLIENT;
    e->fds[cs].read = client_read;
    e->fds[cs].write = client_write;

    return (0);
}
