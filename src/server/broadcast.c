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
                cbuffer_putstr(&e->fds[i].buf_write, msg);
        }
        i++;
    }
}

void broadcast_all(t_env *e, const char *msg, int msg_type, size_t cs)
{
    size_t i;

    i = 0;
    while (i <= e->max)
    {
        if (e->fds[i].type == FD_CLIENT &&
            e->fds[i].channel == e->fds[cs].channel)
        {
            if (msg_type == IRC_NOTICE)
                cbuffer_putstr(&e->fds[i].buf_write, msg);
        }
        i++;
    }
}

void broadcast_all_in_channel(t_env *     e,
                              const char *msg,
                              int         msg_type,
                              size_t      cs)
{
    size_t i;

    i = 0;
    while (i <= e->max)
    {
        if (e->fds[i].type == FD_CLIENT &&
            e->fds[i].channel == e->fds[cs].channel && msg_type == IRC_NOTICE)
            cbuffer_putstr(&e->fds[i].buf_write, msg);
        i++;
    }
}
