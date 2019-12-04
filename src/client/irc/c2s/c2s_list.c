#include <client/irc.h>

int c2s_list(t_env *e, t_token *tokens)
{
    (void)tokens;

    if (e->sock == -1)
        return logerror(
            "You nee to be logged in before any command. Use /connect [server] "
            "?[port]");

    if (!tokens[1].addr)
        cbuffer_putcmd(&e->self->buf_write, "LIST\x0D\x0A");
    else
        cbuffer_putcmd(&e->self->buf_write, "LIST %s\x0D\x0A",
                       tokens[1].addr);
    return (IRC_LIST);
}
