#include <netdb.h>
#include <netinet/in.h>

#include "irc.h"

void server_create(t_env *e, int port)
{
    int                sock;
    struct sockaddr_in sin;
    struct protoent *  pe;

    pe = (struct protoent *)XPSAFE((void *)0, getprotobyname("tcp"),
                                   "getprotobyname");
    sock = XSAFE(-1, socket(PF_INET, SOCK_STREAM, pe->p_proto), "socket");

    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(port);

    XSAFE(-1, bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)),
          "create_server::ipv4::bind");
    XSAFE(-1, listen(sock, 42), "create_server::ipv4::listen");

    e->fds[sock].type = FD_SERV;
    e->fds[sock].read = on_connect;
}