#include <server/irc.h>

int irc_ping(t_env *e, int cs, t_token *tokens)
{
    (void)tokens;
    cbuffer_putstr(&e->fds[cs].buf_write, "PONG\x0D\x0A");
    return (IRC_PING);
}
