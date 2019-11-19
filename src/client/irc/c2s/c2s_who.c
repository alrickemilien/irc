#include <client/irc.h>
#include <ctype.h>

int c2s_who(t_env *e, int cs, t_token *tokens)
{
    (void)tokens;

    if (e->sock == -1)
        return logerror(
            "%s\n",
            "You nee to be logged in before any command. Use /connect [server] "
            "?[port]");

    cbuffer_putstr(&e->fds[cs].buf_write, "NAMES ");
    cbuffer_putstr(&e->fds[cs].buf_write, e->fds[cs].channelname);
    cbuffer_putstr(&e->fds[cs].buf_write, "\x0D\x0A");

    return (IRC_WHO);
}
