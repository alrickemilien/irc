#include <client/irc.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void server_write(t_env *e, size_t cs)
{
    // printf("There is data to sen to the server\n");
    // printf("buf_write has %ld bytes\n", strlen(e->fds[cs].buf_write));
    // printf("buf_write: %s\n", e->fds[cs].buf_write);

    XSAFE(-1, send(cs, e->fds[cs].buf_write, strlen(e->fds[cs].buf_write), 0),
          "server_write::send");
    memset(e->fds[cs].buf_write, 0, BUF_SIZE);
}