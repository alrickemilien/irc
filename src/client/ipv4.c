#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <client/irc.h>
#include <client/ssl.h>

int client_ipv4(t_env *e)
{
    int                cs;
    struct sockaddr_in sin;
    struct protoent *  pe;
    struct hostent *   hostnm;

    //     int                reuseaddr;
    // #ifdef __APPLE__
    //     int reuseport;
    // #endif  // __APPLE__

    logdebug("Connecting to %s:%d through ipv4\n", e->options.host,
             e->options.port);

    if ((hostnm = gethostbyname(e->options.host)) == NULL)
        return (logerrno("ipv4::gethostbyname\n"));

    pe = (struct protoent *)XPSAFE((void *)0, getprotobyname("tcp"),
                                   "ipv4::getprotobyname");

    /********************************************************************/
    /* The socket() function returns a socket descriptor, which represents   */
    /* an endpoint.  Get a socket for address family AF_INET6 to        */
    /* prepare to accept incoming connections on.                       */
    /********************************************************************/
    if ((cs = socket(AF_INET, SOCK_STREAM, pe->p_proto)) < 0)
        return (logerrno("ipv4::socket\n"));

    //     /********************************************************************/
    //     /* The setsockopt() function is used to allow the local address to */
    //     /* be reused when the server is restarted before the required wait */
    //     /* time expires. */
    //     /********************************************************************/
    //     reuseaddr = 1;
    //     setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr,
    //     sizeof(reuseaddr));

    // #ifdef __APPLE__
    //     reuseport = 1;
    //     setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, &reuseport,
    //     sizeof(reuseport));
    // #endif  // __APPLE__

    /*********************************************************************/
    /* After the socket descriptor is created, a bind() function gets a  */
    /* unique name for the socket.  In this example, the user sets the   */
    /* address to in6addr_any, which (by default) allows connections to  */
    /* be established from any IPv4 or IPv6 client that specifies port   */
    /* 3005. (that is, the bind is done to both the IPv4 and IPv6 TCP/IP */
    /* stacks).  This behavior can be modified using the IPPROTO_IPV6    */
    /* level socket option IPV6_V6ONLY if required.                      */
    /*********************************************************************/
    memset(&sin, 0, sizeof(struct sockaddr_in));

    sin.sin_family = AF_INET;
    sin.sin_port = htons(e->options.port);
    sin.sin_addr.s_addr = *((unsigned long *)hostnm->h_addr);

    /********************************************************************/
    /* The listen() function allows the server to accept incoming       */
    /* client connections.  In this example, the backlog is set to 42.  */
    /* This means that the system will queue 42 incoming connection     */
    /* requests before the system starts rejecting the incoming         */
    /* requests.                                                        */
    /********************************************************************/

    if (connect(cs, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        return (logerrno("ipv4::connect"));

    if (e->options.ssl && ssl_connect(e, &e->fds[cs], cs) < 0)
        return (logerror("ipv4::ssl_connect\n"));

    e->sock = cs;
    e->fds[cs].type = FD_CLIENT;
    e->fds[cs].read = server_read;
    e->fds[cs].write = server_write;

    return (0);
}
