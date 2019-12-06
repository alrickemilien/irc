#include <client/irc.h>

int c2s_list(t_env *e, t_token *tokens)
{
    if (e->sock == -1)
        return (irc_error(e, ERR_NOT_CONNECTED));

    if (!tokens[1].addr)
        cbuffer_putcmd(&e->self->buf_write, "LIST\x0D\x0A");
    else
        cbuffer_putcmd(&e->self->buf_write, "LIST %s\x0D\x0A",
                       tokens[1].addr);
    return (IRC_C2S_LIST);
}
