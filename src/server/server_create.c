
#include "irc.h"
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>

void  server_create(t_env *e, int port)
{
  int s;
  struct sockaddr_in sin;
  struct protoent *pe;

  pe = (struct protoent *)PSAFE(NULL, getprotobyname("tcp"), "getprotobyname");
  s = XSAFE(-1, socket(PF_INET, SOCK_STREAM, pe->p_proto), "socket");
  sin.sin_family = AF_INET;
  sin.sin_addr.s_addr = INADDR_ANY;
  sin.sin_port = htons(port);
  XSAFE(-1, bind(s, (struct sockaddr *)&sin, sizeof(sin)), "bind");
  XSAFE(-1, listen(s, 42), "listen");
  e->fds[s].type = FD_SERV;
  e->fds[s].fct_read = accept_connection;
}