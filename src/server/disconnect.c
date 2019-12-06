#include <server/irc.h>

void disconnect(t_env *e, size_t cs)
{
    // Clear the channel
    e->channels[e->fds[cs].channel].clients--;
    if (e->fds[cs].channel != 0 && e->channels[e->fds[cs].channel].clients == 0)
        memset(&e->channels[e->fds[cs].channel], 0, sizeof(t_channel));

    close(cs);
    clear_fd(&e->fds[cs]);
    loginfo("Client #%ld gone away", cs);
    e->fds[cs].last_activity = 0;
    FD_CLR(cs, &e->fd_read);
    FD_CLR(cs, &e->fd_write);
}
