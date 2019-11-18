#include <ctype.h>
#include <client/irc.h>

int c2s_who(t_env *e, int cs, t_token *tokens)
{
    (void)tokens;

    cbuffer_putstr(&e->fds[cs].buf_write, "NAMES ");
    cbuffer_putstr(&e->fds[cs].buf_write, e->fds[cs].channelname);
    cbuffer_putstr(&e->fds[cs].buf_write, "\x0D\x0A");

    return (IRC_WHO);
}
