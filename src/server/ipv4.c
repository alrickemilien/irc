#include <netdb.h>
#include <netinet/in.h>

#include "server/irc.h"

void server_ipv4(const t_options *options, t_env *e)
{
    int                sock;
    struct sockaddr_in sin;
    struct protoent *  pe;
    int                reuseaddr;
    // struct hostent *hp;

    pe = (struct protoent *)XPSAFE((void *)0, getprotobyname("tcp"),
                                   "getprotobyname");

    /********************************************************************/
    /* The socket() function returns a socket descriptor, which represents   */
    /* an endpoint.  Get a socket for address family AF_INET6 to        */
    /* prepare to accept incoming connections on.                       */
    /********************************************************************/
    sock = XSAFE(-1, socket(PF_INET, SOCK_STREAM, pe->p_proto),
                 "server_ipv4::socket");

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
    memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    // hp = (struct hostent *)XPSAFE((void*)0, gethostbyname(options->bind),
    // "server_ipv4::gethostbyname");
    // memcpy(&sin.sin_addr, hp->h_addr_list[0], hp->h_length);
    sin.sin_port = htons(options->port);

    XSAFE(-1, bind(sock, (struct sockaddr *)&sin, sizeof(struct sockaddr_in)),
          "server_ipv4::ipv4::bind");

    /********************************************************************/
    /* The listen() function allows the server to accept incoming       */
    /* client connections.  In this example, the backlog is set to 42.  */
    /* This means that the system will queue 42 incoming connection     */
    /* requests before the system starts rejecting the incoming         */
    /* requests.                                                        */
    /********************************************************************/
    XSAFE(-1, listen(sock, options->backlog), "server_ipv4::ipv4::listen");

    e->fds[sock].type = FD_SERV;
    e->fds[sock].read = on_connect;
}
