#include <server/irc.h>

void disconnect(t_env *e, size_t cs)
{
    close(cs);
    clear_fd(&e->fds[cs]);
    loginfo("Client #%ld gone away\n", cs);

    FD_CLR(cs, &e->fd_read);
    FD_CLR(cs, &e->fd_write);
}
