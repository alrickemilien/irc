#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <server/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

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
    cs = XSAFE(-1, accept(s, (struct sockaddr *)&csin, &csin_len),
               "on_connect::accept");

    loginfo("New client #%d from %s:%d\n", cs, inet_ntoa(csin.sin_addr),
            ntohs(csin.sin_port));

    // if (gethostname(e->fds[cs].hostname, sizeof(e->fds[cs].hostname) < 0))
    //   return (logerrno("on_connect::gethostname"));

    if (getnameinfo((struct sockaddr *)&csin, csin_len, e->fds[cs].host,
                    NI_MAXHOST, e->fds[cs].serv, NI_MAXSERV, NI_NAMEREQD) < 0)
    {
        logerrno("on_connect::getnameinfo");
        return;
    }

    e->fds[cs].type = FD_CLIENT;
    e->fds[cs].read = client_read;
    e->fds[cs].write = client_write;
    memcpy(e->fds[cs].channel, DEFAULT_CHANNEL, sizeof(DEFAULT_CHANNEL));
    memcpy(e->fds[cs].nickname, DEFAULT_NICKNAME, sizeof(DEFAULT_NICKNAME));
    memset(e->fds[cs].username, 0, USERNAMESTRSIZE + 1);

    memcpy(e->fds[cs].buf_write, HELLO, strlen(HELLO) * sizeof(char));

    return ;
}
