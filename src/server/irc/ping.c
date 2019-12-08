#include <server/irc.h>

int ping(t_env *e, int cs)
{
    return (cbuffer_putcmd(&e->fds[cs].buf_write , "PING %s\x0D\x0A", e->fds[cs].nickname));
}
