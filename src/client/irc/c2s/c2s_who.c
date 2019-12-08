#include <client/irc.h>

int	c2s_who(t_env *e, t_token *tokens)
{
    (void)tokens;
    if (e->sock == -1)
        return (irc_error(e, ERR_NOT_CONNECTED));
    if (cbuffer_putcmd(&e->self->buf_write, "NAMES %s\x0D\x0A",
                       e->self->channelname) < 0)
        return (-1);
    return (IRC_C2S_WHO);
}
