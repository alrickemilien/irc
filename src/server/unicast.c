#include <server/irc.h>

void unicast(t_env *e, const char *msg, int msg_type, size_t cs)
{
    if (msg_type == IRC_NOTICE)
    {
        cbuffer_putstr(&e->fds[cs].buf_write, "NOTICE ");
        cbuffer_putstr(&e->fds[cs].buf_write, msg);
    }
    if (msg_type == IRC_PRIVMSG)
    {
        cbuffer_putstr(&e->fds[cs].buf_write, "PRIVMSG ");
        cbuffer_putstr(&e->fds[cs].buf_write, msg);
    }
}
