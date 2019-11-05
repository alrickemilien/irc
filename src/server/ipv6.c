#include <netdb.h>
#include <netinet/in.h>

#include "server/irc.h"

int server_ipv6(const t_options *options, t_env *e)
{
    int                 sock;
    int                 reuseaddr;
    struct sockaddr_in6 sin;
    struct protoent     *pe;
    // struct hostent      *hp;

    pe = (struct protoent *)XPSAFE((void *)0, getprotobyname("tcp"),
                                   "server_ipv6::getprotobyname");
    /********************************************************************/
    /* The socket() function returns a socket descriptor, which represents   */
    /* an endpoint.  Get a socket for address family AF_INET6 to        */
    /* prepare to accept incoming connections on.                       */
    /********************************************************************/
    sock = XSAFE(-1, socket(AF_INET6, SOCK_STREAM, pe->p_proto), "server_ipv6::socket");

    /********************************************************************/
    /* The setsockopt() function is used to allow the local address to  */
    /* be reused when the server is restarted before the required wait  */
    /* time expires.                                                    */
    /********************************************************************/
    reuseaddr = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));

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
    /********************************************************************/
    /* Note: applications use in6addr_any similarly to the way they use */
    /* INADDR_ANY in IPv4.  A symbolic constant IN6ADDR_ANY_INIT also   */
    /* exists but can only be used to initialize an in6_addr structure  */
    /* at declaration time (not during an assignment).                  */
    /********************************************************************/
    // hp = (struct hostent *)XPSAFE((void*)0, gethostbyname(options->bind), "server_ipv6::gethostbyname");
    // memcpy(&sin.sin6_addr, hp->h_addr_list[0], hp->h_length);
    sin.sin6_addr = in6addr_any;
    sin.sin6_port = htons(options->port);
    XSAFE(-1, bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in6)),
          "server_ipv6::bind");

    /********************************************************************/
    /* The listen() function allows the server to accept incoming       */
    /* client connections.  In this example, the backlog is set to 42.  */
    /* This means that the system will queue 42 incoming connection     */
    /* requests before the system starts rejecting the incoming         */
    /* requests.                                                        */
    /********************************************************************/
    XSAFE(-1, listen(sock, options->backlog), "server_ipv6::listen");

    e->fds[sock].type = FD_SERV;
    e->fds[sock].read = on_connect;

    return (0);
}
