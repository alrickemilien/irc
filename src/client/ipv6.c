#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <client/irc.h>

void client_ipv6(t_env *e)
{
    int                 cs;
    struct sockaddr_in6 sin;
    struct protoent *   pe;

    pe = (struct protoent *)XPSAFE((void *)0, getprotobyname("tcp"),
                                   "ipv6::getprotobyname");

    /********************************************************************/
    /* The socket() function returns a socket descriptor, which represents   */
    /* an endpoint.  Get a socket for address family AF_INET6 to        */
    /* prepare to accept incoming connections on.                       */
    /********************************************************************/

    cs = XSAFE(-1, socket(AF_INET6, SOCK_STREAM, pe->p_proto), "ipv6::socket");

    /*********************************************************************/
    /* After the socket descriptor is created, a bind() function gets a  */
    /* unique name for the socket.  In this example, the user sets the   */
    /* address to in6addr_any, which (by default) allows connections to  */
    /* be established from any IPv4 or IPv6 client that specifies port   */
    /* 3005. (that is, the bind is done to both the IPv4 and IPv6 TCP/IP */
    /* stacks).  This behavior can be modified using the IPPROTO_IPV6    */
    /* level socket option IPV6_V6ONLY if required.                      */
    /*********************************************************************/
    memset(&sin, 0, sizeof(sin));

    sin.sin6_family = AF_INET6;
    sin.sin6_port = htons(e->options.port);
    inet_pton(AF_INET6, e->options.host, &sin.sin6_addr);

    /********************************************************************/
    /* The listen() function allows the server to accept incoming       */
    /* client connections.  In this example, the backlog is set to 42.  */
    /* This means that the system will queue 42 incoming connection     */
    /* requests before the system starts rejecting the incoming         */
    /* requests.                                                        */
    /********************************************************************/
    XSAFE(-1, connect(cs, (struct sockaddr *)&sin, sizeof(sin)),
          "ipv6::connect");

    e->sock = cs;
    e->fds[cs].type = FD_CLIENT;
    e->fds[cs].read = server_read;
    e->fds[cs].write = server_write;
}
