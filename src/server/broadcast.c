#include <server/irc.h>

void broadcast(t_env *e, const char *msg, int msg_type, size_t cs)
{
    size_t i;

    i = 0;
    while (i <= e->max)
    {
        if (i != cs && e->fds[i].type == FD_CLIENT &&
            e->fds[i].channel == e->fds[cs].channel)
        {
            if (msg_type == IRC_NOTICE)
                cbuffer_putstr(&e->fds[cs].buf_write, msg);
            else
            {
                cbuffer_putstr(&e->fds[cs].buf_write, "ALERT: ");
                cbuffer_putstr(&e->fds[cs].buf_write, e->fds[cs].nickname);
                cbuffer_putstr(&e->fds[cs].buf_write, msg);
            }
        }
        i++;
    }
}
