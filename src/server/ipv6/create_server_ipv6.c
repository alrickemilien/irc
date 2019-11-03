#include <netdb.h>
#include <netinet/in.h>

#include "irc.h"

int create_server_ipv6(t_env *e, int port)
{
    int                 sock;
    int                 reuseaddr;
    struct sockaddr_in6 sin;
    struct protoent *   pe;

    pe = (struct protoent *)XPSAFE((void *)0, getprotobyname("tcp"),
                                   "getprotobyname");
    reuseaddr = 1;
    sock = socket(AF_INET6, SOCK_STREAM, pe->p_proto);
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    sin.sin6_family = AF_INET6;
    sin.sin6_addr = in6addr_any;
    sin.sin6_port = htons(port);

    XSAFE(-1, bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in6)),
          "create_server::ipv6::bind");
    XSAFE(-1, listen(sock, 42), "create_server::ipv6::listen");

    e->fds[sock].type = FD_SERV;
    e->fds[sock].read = on_connect;

    return (0);
}