#include <netdb.h>
#include <netinet/in.h>

#include "irc.h"

void server_ipv4(const t_options *options, t_env *e)
{
    int                sock;
    struct sockaddr_in sin;
    struct protoent *  pe;
    int                reuseaddr;
    struct hostent *hp;

    pe = (struct protoent *)XPSAFE((void *)0, getprotobyname("tcp"),
                                   "getprotobyname");
    sock = XSAFE(-1, socket(PF_INET, SOCK_STREAM, pe->p_proto), "server_ipv4::socket");

    reuseaddr = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

    memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    hp = (struct hostent *)XPSAFE((void*)0, gethostbyname(options->host), "server_ipv4::gethostbyname");
    memcpy(&sin.sin_addr, hp->h_addr_list[0], hp->h_length);
    sin.sin_port = htons(options->port);

    XSAFE(-1, bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)),
          "server_ipv4::ipv4::bind");
    XSAFE(-1, listen(sock, options->backlog), "server_ipv4::ipv4::listen");

    e->fds[sock].type = FD_SERV;
    e->fds[sock].read = on_connect;
}
