#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <server/irc.h>
#include <server/ssl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int on_connect(t_env *e, size_t s)
{
    int                cs;
    struct sockaddr_in csin;
    socklen_t          csin_len;

    csin_len = sizeof(csin);
    if ((cs = accept(s, (struct sockaddr *)&csin, &csin_len)) == -1)
        return (logerrno("on_connect::accept"));

    loginfo("New client #%d from %s:%d", cs, inet_ntoa(csin.sin_addr),
            ntohs(csin.sin_port));

    // Clear file descriptor
    memset(&e->fds[cs], 0, sizeof(t_fd));

    if (e->ssl_ctx && ssl_on_connect(e->ssl_ctx, &e->fds[cs], cs) < 0)
        return (-1);

    // init the new client
    e->fds[cs].type = FD_CLIENT;
    e->fds[cs].read = client_read;
    e->fds[cs].write = client_write;
    e->fds[cs].last_activity = (unsigned long)time(NULL);

    return (0);
}
