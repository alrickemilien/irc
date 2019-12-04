#include <client/irc.h>
#include <ctype.h>

int c2s_who(t_env *e, t_token *tokens)
{
    (void)tokens;

    if (e->sock == -1)
        return logerror(
            "You nee to be logged in before any command. Use /connect [server] "
            "?[port]");

    if (cbuffer_putcmd(&e->self->buf_write, "NAMES %s\x0D\x0A",
                       e->self->channelname) < 0)
        return (-1);

    return (IRC_WHO);
}
