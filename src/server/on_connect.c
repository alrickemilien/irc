#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <server/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

void on_connect(t_env *e, size_t s)
{
    int                cs;
    struct sockaddr_in csin;
    socklen_t          csin_len;

    csin_len = sizeof(csin);
    cs = XSAFE(-1, accept(s, (struct sockaddr *)&csin, &csin_len),
               "on_connect::accept");

    loginfo("New client #%d from %s:%d\n", cs, inet_ntoa(csin.sin_addr),
            ntohs(csin.sin_port));

    if (getnameinfo((struct sockaddr *)&csin, csin_len, e->fds[cs].host,
                    NI_MAXHOST, e->fds[cs].serv, NI_MAXSERV, NI_NAMEREQD) < 0)
    {
        logerrno("on_connect::getnameinfo");
        return;
    }

    // init the new client
    e->fds[cs].type = FD_CLIENT;
    e->fds[cs].read = client_read;
    e->fds[cs].write = client_write;
    e->fds[cs].registered = 0;
    e->fds[cs].channel = 0;
    e->fds[cs].away = 0;
    
    memset(e->fds[cs].nickname, 0, NICKNAMESTRSIZE + 1);
    memcpy(e->fds[cs].nickname, DEFAULT_NICKNAME, sizeof(DEFAULT_NICKNAME));
    
    memset(e->fds[cs].username, 0, USERNAMESTRSIZE + 1);
    
    memset(e->fds[cs].awaymessage, 0, sizeof(e->fds[cs].awaymessage));

    return;
}
