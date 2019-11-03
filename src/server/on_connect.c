
#include "irc.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>

void on_connect(t_env *e, size_t s)
{
  int cs;
  struct sockaddr_in csin;
  socklen_t csin_len;

  csin_len = sizeof(csin);
  cs = XSAFE(-1, accept(s, (struct sockaddr *)&csin, &csin_len), "accept");
  
  printf("New client #%d from %s:%d\n", cs, inet_ntoa(csin.sin_addr),
         ntohs(csin.sin_port));
  
  clear_fd(&e->fds[cs]);
  e->fds[cs].type = FD_CLIENT;
  e->fds[cs].read = client_read;
  e->fds[cs].write = client_write;
}