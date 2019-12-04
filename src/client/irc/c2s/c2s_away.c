#include <client/irc.h>

int _c2s_away(t_fd *fd, const char *msg, size_t msg_len)
{
    return (cbuffer_putcmd(&fd->buf_write, "AWAY %.*s\x0D\x0A", msg_len, msg));
}

int c2s_away(t_env *e, t_token *tokens)
{
    if (e->sock == -1)
        return logerror(
            "You need to be logged in before any command. Use "
            "/connect [server] ?[port]");

    if (tokens[1].addr == (void *)0)
        return (irc_error(e, ERR_NEEDMOREPARAMS, tokens[0].addr));

    _c2s_away(e->self, tokens[1].addr, strlen(tokens[1].addr));

    return (IRC_AWAY);
}

int _c2s_unaway(t_fd *fd)
{
    return cbuffer_putstr(&fd->buf_write, "AWAY\x0D\x0A");
}

int c2s_unaway(t_env *e, t_token *tokens)
{
    (void)tokens;

    if (e->sock == -1)
        return logerror(
            "You need to be logged in before any command. Use "
            "/connect [server] ?[port]");

    _c2s_unaway(e->self);

    return (IRC_UNAWAY);
}
